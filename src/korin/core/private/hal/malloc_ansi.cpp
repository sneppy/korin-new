#include "hal/malloc_ansi.h"
#include "hal/platform_crt.h"

void* MallocAnsi::malloc(sizet size, sizet alignment)
{
	void* mem = nullptr;

#if PLATFORM_WINDOWS
	// TODO
#elif PLATFORM_POSIX
	::posix_memalign((void**)&mem, alignment, size);
#else
#	warning "Cannot impose memory alignment"
	mem = ::malloc(size);
#endif

	return mem;
}

void MallocAnsi::free(void* mem)
{
#if PLATFORM_WINDOWS
	// TODO
#elif PLATFORM_POSIX
	::free(mem);
#else
	::free(mem);
#endif
}

sizet MallocAnsi::getUsedMemory() const
{
	return {};
}

#ifndef USE_CUSTOM_GLOBAL_MALLOC
// Create the global allocator
MallocAnsi _gMalloc;
MallocBase* gMalloc = &_gMalloc;
#endif
