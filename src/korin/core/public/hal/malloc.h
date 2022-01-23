#pragma once


#ifndef KORIN_USE_CUSTOM_GMALLOC
# define KORIN_USE_CUSTOM_GMALLOC 0
#endif


/* The minimum required alignment */
enum { MIN_ALIGNMENT = sizeof(void*) };


/**
 * @brief Base class for allocators.
 *
 * Unless said otherwise, memory allocated
 * must be reallocated/freed using the same
 * allocator.
 */
class MallocBase
{
public:
	/**
	 * @brief Allocate the required memory.
	 *
	 * If for some reason memory could
	 * not be allocated, a null ptr is
	 * returned.
	 *
	 * @param size required allocation size
	 * @param alignment allocation alignment
	 * @return ptr to allocated memory
	 * @return nullptr if memory could not be
	 * allocated
	 */
	virtual void* malloc(sizet size, sizet alignment = MIN_ALIGNMENT) = 0;

	/**
	 * @brief Free memory which was allocated
	 * by this instance.
	 *
	 * @param mem ptr to allocated memory
	 */
	virtual void free(void* mem) = 0;

	/**
	 * @brief Return the amount of memory currently
	 * allocated by this allocator.
	 *
	 * The value returned may not be accurate
	 * or may be zero in debug mode.
	 */
	virtual sizet getUsedMemory() const = 0;
};


/* The global allocator. Other allocators may rely on it to
   work, thus an instance must always exist. If not
   specified, an instance of MallocAnsi is created. */
extern MallocBase* gMalloc;
