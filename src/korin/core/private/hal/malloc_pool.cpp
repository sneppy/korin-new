#include "hal/platform_crt.h"
#include "hal/platform_math.h"
#include "hal/malloc_pool.h"
#include "hal/malloc_ansi.h"


#define NEXT_BLOCK(block, logSize) (*reinterpret_cast<void**>(reinterpret_cast<uintp>(block) + logSize))


namespace
{
	/* Initialize the block list of a memory pool. */
	FORCE_INLINE void initMemoryPool(MemoryPool* pool)
	{
		ASSERT(pool != nullptr)
		ASSERT(pool->buffer != nullptr)

		const uint32 numBlocks = pool->createInfo.numBlocks;
		const sizet blockLogSize = pool->createInfo.blockSize;
		const sizet blockPhySize = PlatformMath::align2Up(blockLogSize + sizeof(void*), pool->createInfo.blockAlignment);

		void* block = pool->blocks = pool->buffer;
		for (uint32 blockIdx = 0; blockIdx < numBlocks - 1; ++blockIdx)
		{
			void* next = reinterpret_cast<void*>(reinterpret_cast<uintp>(block) + blockPhySize);
			block = NEXT_BLOCK(block, blockLogSize) = next;
		}
		NEXT_BLOCK(block, blockLogSize) = nullptr;
	}

	/* Returns true if there are not free blocks in pool. */
	FORCE_INLINE bool isExhausted(MemoryPool const* pool)
	{
		return pool->blocks == nullptr;
	}

	/* Returns true if block belongs to the pool. */
	FORCE_INLINE bool isInPoolRange(MemoryPool const* pool, void const* block)
	{
		void const* begin = pool->buffer;
		void const* end = reinterpret_cast<void*>(reinterpret_cast<uintp>(begin) + pool->bufferSize);
		return block >= begin && block < end;
	}

	/* Pop a free block from the pool. Return nullptr if there are no free blocks. */
	FORCE_INLINE void* acquireBlock(MemoryPool* pool)
	{
		ASSERT(pool != nullptr)

		if (!pool->blocks)
		{
			// Pool is exhausted
			return nullptr;
		}

		// Pop block
		void* block = pool->blocks;
		pool->blocks = NEXT_BLOCK(block, pool->createInfo.blockSize);
		pool->numBlocksInUse++;

		return block;
	}

	/* Release a block to the pool. The block MUST belong to the pool. */
	FORCE_INLINE void releaseBlock(MemoryPool* pool, void* block)
	{
		ASSERT(pool != nullptr)
		ASSERT(block != nullptr)
		ASSERT(isInPoolRange(pool, block))

		NEXT_BLOCK(block, pool->createInfo.blockSize) = pool->blocks;
		pool->blocks = block;
		pool->numBlocksInUse--;
	}

	/* Checks that all the blocks in the pool have been released. */
	void debugPoolAllBlocksReleased(MemoryPool* pool)
	{
		ASSERT(pool != nullptr)

		const uint32 numBlocks = pool->createInfo.numBlocks;
		const sizet blockLogSize = pool->createInfo.blockSize;
		const sizet blockPhySize = PlatformMath::align2Up(blockLogSize + sizeof(void*), pool->createInfo.blockAlignment);

		void** blocks = new void*[numBlocks];
		memset(blocks, 0, numBlocks * sizeof(void*));

		// Traverse blocks list
		for (void* block = pool->blocks; block; NEXT_BLOCK(block, blockLogSize))
		{
			uint32 blockIdx = (reinterpret_cast<uintp>(block) - reinterpret_cast<uintp>(pool->buffer)) / blockPhySize;
			blocks[blockIdx] = block;
		}

		// Check none is null
		for (int32 i = 0; i < numBlocks; ++i)
		{
			ASSERT(blocks[i] != nullptr)
		}

		delete[] blocks;
	}
} // namespace


#undef NEXT_BLOCK


MallocPool::MallocPool(MemoryPool::CreateInfo const& inCreateInfo)
	: root{nullptr}
	, pools{nullptr}
	, numPools{0}
	, createInfo{inCreateInfo}
{
	//
}

MallocPool::~MallocPool()
{
	CHECKF(numPools == 0, "MallocPool @ %p has %u dangling pools", this, numPools)

	// Destroy all pools
	destroy();
}

void* MallocPool::malloc(sizet size, sizet alignment)
{
	CHECKF(size <= createInfo.blockSize,
	       "Requested allocation of size %llu, but block size is %llu",
		   size, createInfo.blockSize)
	CHECKF(alignment <= createInfo.blockAlignment,
	       "Requested allocation with alignment %llu, but block alignment is %llu",
		   alignment, createInfo.blockAlignment)

	if (!pools)
	{
		// Create a new pool in none available
		auto* pool = createPool();

		// Insert node in tree
		root = TreeNode::insert(root, pool, [pool](auto const* node) {

			// Order by buffer address
			return GreaterThan{}(pool->buffer, node->buffer);
		});
		numPools++;

		// Append to pools list
		pools = pool;
	}

	if (!pools)
	{
		// Unable to create a new pool
		return nullptr;
	}

	ASSERT(pools->blocks != nullptr)

	// Get free block from pool
	void* block = acquireBlock(pools);

	if (isExhausted(pools))
	{
		// Pool is exhausted, pop from list
		pools = pools->next;
	}

	return block;
}

void MallocPool::free(void* mem)
{
	// Locate pool
	auto* pool = TreeNode::find(root, [mem](auto const* node) {

		if (mem < node->buffer)
		{
			// Go left
			return -1;
		}
		else if (mem == node->buffer || isInPoolRange(node, mem))
		{
			// Found pool
			return 0;
		}
		else
		{
			// Go right
			return 1;
		}
	});

	CHECKF(pool != nullptr, "Block (%p) not found in MallocPool @ %p", mem, this)
	if (!pool) return;

	// Release block
	bool const moveToFront = pool->numBlocksInUse == pool->createInfo.numBlocks;
	releaseBlock(pool, mem);

	if (moveToFront)
	{
		// Push pool back to front
		pool->MemoryPoolHandle::prev = nullptr;
		pool->MemoryPoolHandle::next = pools;
		pools = pool;
	}
	else if (pool->numBlocksInUse == 0)
	{
		// Remove from list
		if (pool == pools)
		{
			// Pool is head
			pools = pools->next;
		}
		else
		{
			// Must have prev
			pool->MemoryPoolHandle::prev->next = pool->MemoryPoolHandle::next;

			if (pool->MemoryPoolHandle::next)
			{
				// Unlink also from next
				pool->MemoryPoolHandle::next->prev = pool->MemoryPoolHandle::prev;
			}
		}

		// Destroy pool and remove from tree
		numPools--;
		root = TreeNode::remove(pool);
		destroyPool(pool);
	}
}

sizet MallocPool::getUsedMemory() const
{
	sizet usedMemory = 0;
#if !KORIN_RELEASE
	if (!root)
	{
		return usedMemory;
	}

	for (auto* pool = TreeNode::getMin(root); pool; pool = pool->next)
	{
		// Return logical size
		usedMemory += pool->numBlocksInUse * pool->createInfo.blockSize;
	}
#endif
	return usedMemory;
}

FORCE_INLINE MallocPool::NodeT* MallocPool::createPool()
{
	// Compute size needed
	const sizet blockLogSize = createInfo.blockSize;
	const sizet blockPhySize = PlatformMath::align2Up(blockLogSize + sizeof(void*), createInfo.blockAlignment);
	const sizet bufferLogSize = createInfo.numBlocks * blockPhySize;
	const sizet poolHandleOffset = PlatformMath::align2Up(bufferLogSize, alignof(*root));
	const sizet bufferPhySize = poolHandleOffset + sizeof(*root);

	// Allocate buffer using global allocator
	void* buffer = gMalloc->malloc(PlatformMath::align2Up(bufferLogSize, alignof(*root)) + sizeof(*root), createInfo.blockAlignment);

	// Create node and init memory pool
	NodeT* node = new (reinterpret_cast<void*>(reinterpret_cast<uintp>(buffer) + poolHandleOffset)) NodeT{};
	node->buffer = buffer;
	node->bufferSize = bufferPhySize;
	node->createInfo = createInfo;
	initMemoryPool(node);

	return node;
}

FORCE_INLINE void MallocPool::destroyPool(NodeT* pool)
{
	ASSERT(pool != nullptr)
	ASSERT(pool->buffer != nullptr)
	ASSERT(pool->numBlocksInUse == 0)
	ASSERT(debugPoolAllBlocksReleased(pool)) // Check all blocks have been released

	// Dealloc buffer
	void* buffer = pool->buffer;
	pool->buffer = nullptr;
	gMalloc->free(buffer);
}

FORCE_INLINE void MallocPool::destroy()
{
	if (root)
	{
		// Destroy all pools
		for (auto* it = TreeNode::getMin(root); it;)
		{
			// Don't care about removing from tree
			auto* next = it->next;
			destroyPool(it);
			it = next;
		}
	}

	root = nullptr;
	pools = nullptr;
	numPools = 0;
}
