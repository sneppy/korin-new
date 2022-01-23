#define KORIN_MALLOC_MIN_ALIGNMENT (sizeof(void*))

/* Low level, aligned memory allocation routine. */
static FORCE_INLINE void *korinMalloc(sizet size, sizet alignment)
{
#if PLATFORM_WINDOWS
	// TODO
#elif PLATFORM_POSIX
	if (void* mem; ::posix_memalign(&mem, alignment, size) == 0)
		return mem;
	return nullptr;
#else
	if (UNLIKELY(!size || alignment < MIN_ALIGNMENT || (alignment & (alignment - 1)) != 0))
		// Return early if size is zero, or alignment is smaller than minimum allowed, or alignment is not a power of 2
		return nullptr;

	// Compute physical size of allocation
	constexpr sizet descSize = sizeof(void*);
	sizet const phySize = size + alignment + descSize;

	// Allocate memory, which needs to be aligned
	void* mem = ::malloc(phySize);
	if (UNLIKELY(!mem))
		return nullptr;

	// Align address
	uintp addr = reinterpret_cast<uintp>(mem) + descSize;
	addr = (addr + alignment - 1) & ~(alignment - 1);

	// Set allocation descriptor
	*reinterpret_cast<void**>(addr - descSize) = mem;

	// Return aligned memory
	return reinterpret_cast<void*>(addr);
#endif
}

/* Low level, memory deallocation routine. */
static FORCE_INLINE void korinFree(void* mem)
{
#if PLATFORM_WINDOWS
	// TODO
#elif PLATFORM_POSIX
	::free(mem);
#else
	if (LIKELY(mem))
	{
		// Read allocation address from descriptor
		constexpr sizet descSize = sizeof(void*);
		uintp addr = reinterpret_cast<uintp>(mem);
		mem = *reinterpret_cast<void**>(addr - descSize);

		// Free memory
		::free(mem);
	}
#endif
}

#undef KORIN_MALLOC_MIN_ALIGNMENT
