#pragma once

#include "core_types.h"
#include "malloc.h"
#include "containers/tree_node.h"

/**
 * @brief Memory pool configuration
 * struct.
 */
struct MemoryPoolConfig
{
	/* Max number of blocks in pool. */
	uint64 numBlocks;

	/* Size of the blocks in Bytes. */
	sizet blockSize;

	/* Alignment of the blocks. */
	sizet blockAlignment = MIN_ALIGNMENT;
};

/**
 * @brief A memory pool is a memory buffer
 * divided in blocks of fixed size.
 * 
 * Each block is aligned to the required
 * alignment. A list of free blocks is
 * mantained, as well as a list of allocated
 * blocks.
 */
struct MemoryPool
{
	/* Memory buffer. */
	void* buffer;

	/* Ptr to first free block. */
	void* blocks = nullptr;

	/* Size of the buffer in Bytes. */
	sizet bufferSize;

	/* Pool configuration. */
	MemoryPoolConfig config;

	/**
	 * @brief Returns true if pool has
	 * free blocks to allocate.
	 */
	FORCE_INLINE bool hasFreeBlocks() const
	{
		return blocks;
	}

	/**
	 * @brief Returns true if ptr is in
	 * buffer range.
	 * 
	 * @param ptr ptr to test
	 * @return true if ptr is in range
	 * @return false otherwise
	 */
	FORCE_INLINE bool isInRange(void* ptr) const
	{
		return ptr >= buffer && ptr < reinterpret_cast<byte*>(buffer) + bufferSize;
	}
};


/**
 * @brief Allocator that allocates memory
 * out of a a single memory pool.
 * 
 * When the pool is exhausted, further
 * allocation calls will return a null ptr.
 */
class MallocPool
{
	// TODO
};

/**
 * @brief This allocator allocates memory
 * out of one or more memory pools.
 * 
 * When a pool is exhausted, it looks for
 * a free pool. If it cannot find a free
 * pool, a new one is created.
 * 
 * When a pool is empty, it is deallocated.
 */
class MallocPooled : public MallocBase
{
	/**
	 * @brief Pool node class.
	 */
	struct Pool : public MemoryPool, public Korin::BinaryNodeBase<Pool>
	{
		/* Next free pools. */
		Pool* pools = nullptr;

		/* If true, pool is exhausted. */
		uint64 numUsedBlocks = 0;
	};

	/**
	 * @brief Create and return a new pool.
	 * 
	 * Updates list and tree.
	 */
	Pool* createPool();

	/**
	 * @brief Destroy a pool.
	 * 
	 * @param pool pool to destroy
	 */
	void destroyPool(Pool* pool);

public:
	/**
	 * @brief Construct a new pooled allocator
	 * with the given pool configuration.
	 * 
	 * @param inConfig the configuration of the
	 * pools
	 */
	FORCE_INLINE MallocPooled(MemoryPoolConfig const& inConfig)
		: pool{nullptr}
		, root{nullptr}
		, config{inConfig}
	{
		//
	}

	/**
	 * @brief Destroy all pools created.
	 */
	~MallocPooled();

	virtual void* malloc(sizet, sizet) override;
	virtual void free(void*) override;
	virtual sizet getUsedMemory() const override;

protected:
	/* Ptr to first free pool. */
	Pool* pool;

	/* Ptr to root pool. */
	Pool* root;

	/* Pool configuration. */
	MemoryPoolConfig const config;
};
