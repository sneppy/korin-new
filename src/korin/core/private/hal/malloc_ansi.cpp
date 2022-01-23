#include "hal/malloc_ansi.h"
#include "hal/platform_crt.h"
#include "misc/assert.h"

#include "memalign.inl"

void* MallocAnsi::malloc(sizet size, sizet alignment)
{
	KORIN_CHECK(alignment >= MIN_ALIGNMENT)
	return korinMalloc(size, alignment);
}

void MallocAnsi::free(void* mem)
{
	KORIN_CHECK(mem != nullptr)
	korinFree(mem);
}

sizet MallocAnsi::getUsedMemory() const
{
	return {};
}

#if !KORIN_USE_CUSTOM_GMALLOC
// Create the global allocator
MallocAnsi _gMalloc;
MallocBase* gMalloc = &_gMalloc;
#endif
