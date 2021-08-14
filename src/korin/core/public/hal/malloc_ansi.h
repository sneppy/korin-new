#pragma once

#include "core_types.h"
#include "malloc.h"

/**
 * @brief Allocator that relies on the
 * operative system allocation routines.
 * 
 * Although not recommended, it is
 * usually safe to allocate and free
 * memory via multiple instances of
 * this allocator.
 */
class MallocAnsi : public MallocBase
{
public:
	virtual void* malloc(sizet, sizet = MIN_ALIGNMENT) override;
	virtual void free(void*) override;
	virtual sizet getUsedMemory() const override;
};
