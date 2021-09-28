#pragma once

#include "gtest/gtest.h"

#include "hal/malloc_pool.h"

// Include valgrind for memory testing
#if __has_include(<valgrind/valgrind.h>)
# include <valgrind/valgrind.h>
#else
# define RUNNING_ON_VALGRIND (false)
#endif

// TODO: Move to shared header
struct Object
{
	struct Find
	{
		int32 operator()(Object const& lhs, Object const& rhs) const
		{
			return GreaterThan{}(lhs.size, rhs.size);
		}
	};

	Object(sizet inSize)
		: size{inSize}
	{
		mem = ::malloc(size);
	}

	Object()
		: Object(100)
	{
		//
	}

	Object(Object const& other)
		: size{other.size}
	{
		mem = ::malloc(size);
		::memcpy(mem, other.mem, size);
	}

	Object(Object&& other)
		: mem{other.mem}
		, size{other.size}
	{
		other.mem = nullptr;
		other.size = 0;
	}

	Object& operator=(Object const& other)
	{
		if (other.size != size)
		{
			mem = ::realloc(mem, other.size);
			size = other.size;
		}
		
		::memcpy(mem, other.mem, size);

		return *this;
	}

	Object& operator=(Object&& other)
	{
		::free(mem);
		mem = other.mem;
		size = other.size;
		other.mem = nullptr;
		other.size;
		return *this;
	}

	~Object()
	{
		if (mem)
		{
			::free(mem);
		}

		size = 0;
	}

	sizet getSize() const
	{
		return size;
	}

	void test()
	{
		char* it = reinterpret_cast<char*>(mem);
		for (int32 i = 0; i < size; ++i)
		{
			*it = i & 0xff;
		}
	}

protected:
	void* mem;
	sizet size;
};

TEST(memory, MallocPool)
{
	constexpr uint32 numObjects = 32;
	constexpr uint32 numPools = 16;
	MallocPool* allocator = new MallocPool{{
		blockSize: sizeof(Object),
		blockAlignment: alignof(Object),
		numBlocks: numObjects / numPools
	}};
	
	Object* objs[numObjects] = {};

	for (int32 i = 0; i < numObjects; ++i)
	{
		void* mem = allocator->malloc(sizeof(Object), alignof(Object));
		objs[i] = new (mem) Object{(sizet)rand() & 0xff};
	}
	
	ASSERT_EQ(allocator->getNumPools(), numPools);
#if !KORIN_RELEASE
	ASSERT_EQ(allocator->getUsedMemory(), numObjects * sizeof(Object));
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
