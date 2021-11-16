#pragma once

#include "gtest/gtest.h"
#include "testing.h"

using namespace Korin;

#include "hal/malloc_pool.h"

// Include valgrind for memory testing
#if __has_include(<valgrind/valgrind.h>)
# include <valgrind/valgrind.h>
#else
# define RUNNING_ON_VALGRIND (0)
# define VALGRIND_COUNT_ERRORS (0)
#endif

TEST(memory, MallocPool)
{
	constexpr uint32 numObjects = 32;
	constexpr uint32 numPools = 16;
	MallocPool* allocator = new MallocPool{{
		blockSize: sizeof(Testing::Object),
		blockAlignment: alignof(Testing::Object),
		numBlocks: numObjects / numPools
	}};
	
	Testing::Object* objs[numObjects] = {};

	for (int32 i = 0; i < numObjects; ++i)
	{
		void* mem = allocator->malloc(sizeof(Testing::Object), alignof(Testing::Object));
		objs[i] = new (mem) Testing::Object{(sizet)rand() & 0xff};
	}
	
	ASSERT_EQ(allocator->getNumPools(), numPools);
#if !KORIN_RELEASE
	ASSERT_EQ(allocator->getUsedMemory(), numObjects * sizeof(Testing::Object));
#endif

	// TODO: Shuffle objects

	for (int32 i = 0; i < numObjects; ++i)
	{
		objs[i]->~Object();
		allocator->free(objs[i]);
	}

	ASSERT_EQ(allocator->getNumPools(), 0);
	ASSERT_EQ(allocator->getUsedMemory(), 0);

	// Destroy pool
	delete allocator;

	if (RUNNING_ON_VALGRIND)
	{
		// Check number of memory errors is zero
		ASSERT_EQ(VALGRIND_COUNT_ERRORS, 0);
	}

	SUCCEED();
}
