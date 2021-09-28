#pragma once

#include "core_types.h"
#include "malloc.h"
#include "containers/tree_node.h"

using namespace Korin;

/**
 * @brief A memory pool consists of a list
 * of memory blocks with fixed size and
 * alignment.
 * 
 * Can be used to efficiently allocate memory
 * for lists, tree nodes and any other
 * non-contiguous container with fixed size
 * allocations.
 */
struct MemoryPool
{
	using BlockT = void*;

	/**
	 * @brief Information used to describe
	 * a pool configuration upon creation.
	 * 
	 * The alignment of the blocks must be
	 * at least MIN_ALIGNMENT (usually the
	 * size of a pointer).
	 */
	struct CreateInfo
	{
		/* Size in Bytes of a block. */
		sizet blockSize;

		/* Alignment in Bytes of a block. */
		sizet blockAlignment = MIN_ALIGNMENT;

		/* Number of blocks in pool. */
		uint32 numBlocks;
	};

	/* Buffer of the memory pool. */
	void* buffer = nullptr;

	/* Actual size of the buffer. */
	sizet bufferSize = 0;

	/* Head of the list of free blocks. */
	void* blocks = nullptr;

	/* Number of blocks in use. */
	uint32 numBlocksInUse = 0;

	/* Pool configuration. */
	CreateInfo createInfo;
};

/**
 * @brief Allocator that allocates from
 * memory pools.
 * 
 * When all pools are exhausted a new one
 * is created.
 */
class MallocPool : public MallocBase
{
public:
	/**
	 * @brief Construct a new pool.
	 * 
	 * The allocator is initialized with
	 * no pools.
	 * 
	 * @param inCreateInfo pool creation info
	 */
	MallocPool(MemoryPool::CreateInfo const& inCreateInfo);

	/**
	 * @brief Destroy the allocator along
	 * with all the pools.
	 * 
	 * Note that it's up to the user to
	 * destroy all objects using memory
	 * from the allocator.
	 * 
	 * Ideally, when called all pools should
	 * already have been destroyed.
	 */
	~MallocPool();

	/**
	 * @brief Returns the number of pools currently
	 * in use by the allocator.
	 */
	FORCE_INLINE uint32 getNumPools() const
	{
		return numPools;
	}

	virtual void* malloc(sizet, sizet) override;
	virtual void free(void*) override;
	virtual sizet getUsedMemory() const override;
	
protected:
	/**
	 * @brief Handle with pointers to next and
	 * prev available pools.
	 */
	struct MemoryPoolHandle : public MemoryPool
	{
		/* Next available pool. */
		MemoryPoolHandle* next = nullptr;

		/* Previous available pool. */
		MemoryPoolHandle* prev = nullptr;
	};

	using NodeT = BinaryNodeBase<MemoryPoolHandle>;

	// Always configure
	MallocPool() = delete;
	// TODO: Inherit noncopyable and nonmovable

	/**
	 * @brief Create a new pool.
	 * 
	 * Called when all pools are exhausted.
	 * 
	 * This does not insert the pool in the
	 * tree nor does it prepend it to the
	 * list of free pools.
	 * 
	 * @return ptr to new pool
	 */
	NodeT* createPool();

	/**
	 * @brief Destroy a pool.
	 * 
	 * This does not remove the pool from
	 * the tree nor does it remove it from
	 * the list of available pools.
	 * 
	 * @param pool pool to remove
	 */
	void destroyPool(NodeT* pool);

	/**
	 * @brief Destroy the allocator and all
	 * existing pools.
	 */
	void destroy();

	/* Root of the pool tree. */
	NodeT* root;

	/* Head of the list of available pools. */
	MemoryPoolHandle* pools;

	/* Number of existing pools (available and not). */
	uint32 numPools;

	/* Info used to create new pools. */
	MemoryPool::CreateInfo createInfo;
};
