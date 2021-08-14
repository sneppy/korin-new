#pragma once

#include "gtest/gtest.h"

#include "hal/malloc_pool.h"

namespace
{
	template<typename ItT>
	static void shuffle(ItT begin, ItT end)
	{
		// TODO
	}
} // namespace


TEST(memory, MallocPooled)
{
	constexpr uint64 numRequestedBlocks = 0x1000;
	constexpr uint64 numBlocks = 0x100;
	constexpr sizet blockSize = 0x40;
	constexpr sizet blockAlignment = 0x20;
	MallocPooled* x = new MallocPooled{{numBlocks, blockSize, blockAlignment}};

	void* blocks[numRequestedBlocks] = {};

	for (uint64 i = 0; i < numRequestedBlocks; ++i)
	{
		// Allocate
		blocks[i] = x->malloc(blockSize, blockAlignment);
	}
	
	shuffle(blocks, blocks + numRequestedBlocks);

	for (uint64 i = 0; i < numRequestedBlocks; ++i)
	{
		// Free blocks
		x->free(blocks[i]);
	}

	delete x;

	SUCCEED();
}
