#include "hal/platform_crt.h"
#include "hal/malloc_pool.h"
#include "hal/malloc_ansi.h"

#ifndef MATH
namespace Math
{
	static FORCE_INLINE bool isPowerOfTwo(uint64 n)
	{
		return (n & (n - 1)) == 0;
	}

	static FORCE_INLINE uint64 align2Up(uint64 n, uint64 m)
	{
		CHECK(isPowerOfTwo(m))
		const uint64 p = m - 1;
		return (n + p) & ~p;
	}
} // namespace Math
#endif

#define NEXT(block, blockSize) (reinterpret_cast<void**>(reinterpret_cast<byte*>(block) + blockSize))

namespace
{
	/**
	 * @brief Retrieve a block of memory from pool.
	 * 
	 * Make sure the pool is not exhausted
	 * before calling this function.
	 * 
	 * @param pool the pool to allocate from
	 * @return ptr to allocated memory if free block
	 */
	static FORCE_INLINE void* memoryPoolMalloc(MemoryPool* pool)
	{
		ASSERT(pool->blocks != nullptr)

		void* out = pool->blocks;
		pool->blocks = *NEXT(pool->blocks, pool->config.blockSize);
		return out;
	}

	/**
	 * @brief Free a previously allocated block of
	 * memory.
	 * 
	 * @param pool the pool this memory belongs to
	 * @param mem ptr to the memory to deallocate
	 */
	static FORCE_INLINE void memoryPoolFree(MemoryPool* pool, void* mem)
	{
		CHECK(pool->isInRange(mem))

		*NEXT(mem, pool->config.blockSize) = pool->blocks;
		pool->blocks = mem;
	}

	/**
	 * @brief Initialize a memory pool.
	 * 
	 * @param pool pool to initialize
	 */
	static FORCE_INLINE void initMemoryPool(MemoryPool* pool)
	{
		ASSERT(pool != nullptr)
		CHECK(pool->numBlocks > 0)

		const auto& config = pool->config;
		const sizet actualBlockSize = Math::align2Up(config.blockSize + sizeof(void*), config.blockAlignment);
		auto* block = pool->buffer;
		pool->blocks = block;

		for (uint64 blockIdx = 0; blockIdx < config.numBlocks - 1; ++blockIdx)
		{
			// Link with adjacent block
			void* nextBlock = (reinterpret_cast<byte*>(block) + actualBlockSize);
			block = *NEXT(block, config.blockSize) = nextBlock;
		}

		// Final next block should be null
		*NEXT(block, config.blockSize) = nullptr;
	}
} // namespace

#undef NEXT

FORCE_INLINE MallocPooled::Pool* MallocPooled::createPool()
{
	ASSERTF(gMalloc, "Invalid global allocator @%p", gMalloc);

	// TODO: This can be made class const
	// Allocate the buffer for the pool
	const sizet actualBlockSize = Math::align2Up(config.blockSize + sizeof(void*), config.blockAlignment);
	const sizet poolSize = actualBlockSize * config.numBlocks;
	const sizet bufferSize = poolSize + sizeof(Pool);
	void* buffer = gMalloc->malloc(bufferSize, config.blockAlignment);

	// Create the pool
	Pool* pool = new (reinterpret_cast<byte*>(buffer) + poolSize) Pool{};
	pool->buffer = buffer;
	pool->next = nullptr;
	pool->bufferSize = bufferSize;
	pool->config = config;

	// Create the pool structure
	initMemoryPool(pool);

	// Update root
	root = Containers::TreeNode::insert(root, pool, [](Pool* pool, Pool* other) {

		return GreaterThan{}(pool->buffer, other->buffer);
	});

	return pool;
}

FORCE_INLINE void MallocPooled::destroyPool(Pool* pool)
{
	// TODO: Remove from tree and deallocate
	root = Containers::TreeNode::remove(pool);

	// Dealloc pool
	gMalloc->free(pool->buffer);
}

MallocPooled::~MallocPooled()
{
	while (root)
	{
		// Destroy all pools
		destroyPool(root);
	}
}

void* MallocPooled::malloc(sizet size, sizet alignment)
{
	CHECK(alignment <= config.blockAlignment);

	// Make sure a free pool exists
	if (UNLIKELY(!pool))
	{
		// Create a new pool
		pool = createPool();
		
		if (UNLIKELY(!pool))
		{
			CHECKF(false, "Could not allocate new pool for allocator @ %p", this);
			return nullptr;
		}
	}
	CHECKF(pool->blocks, "pool @ %p has no free blocks", pool)
	
	// Take head and move to next free block
	void* out = memoryPoolMalloc(pool);
	pool->numUsedBlocks++;
	CHECK(out != nullptr)

	if (!pool->hasFreeBlocks())
	{
		// Pool is exhausted, pop from queue
		pool = pool->pools;
	}
	
	return out;
}

void MallocPooled::free(void* mem)
{
	CHECK(mem != nullptr)

	// Find pool this memory was allocated from
	auto* it = root;
	while (it)
	{
		if (it->isInRange(mem))
		{
			break;
		}
		else if (mem < it->buffer)
		{
			it = it->left;
		}
		else
		{
			it = it->right;
		}
	}

	if (!it)
	{
		// Could not find the memory pool this
		// block belongs to
		CHECKF(false, "Memory @ %p was not allocated by this allocator @ %p", mem, this);
		return;
	}

	if (it->numUsedBlocks == it->config.numBlocks)
	{
		// Pool was exhausted, move back to list of free pools
		it->pools = pool;
		pool = it;
	}

	// Relase block
	memoryPoolFree(it, mem);
	it->numUsedBlocks--;

	if (it->numUsedBlocks == 0)
	{
		// If pool is empty destroy it
		if (pool == it)
		{
			pool = it->pools;
		}
		else
		{
			// Find pool
			for (auto* jt = pool; jt; jt = jt->pools)
			{
				if (jt->pools == it)
				{
					jt->pools = it->pools;
					break;
				}
			}
		}
		destroyPool(it);
	}
}

sizet MallocPooled::getUsedMemory() const
{
	return {};
}
