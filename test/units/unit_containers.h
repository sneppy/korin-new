#pragma once

#include "gtest/gtest.h"

#include "containers/list.h"
#include "containers/tree.h"

using namespace Containers;

TEST(containers, list)
{
	List<int32> x, y, z;

	ASSERT_EQ(x.getNumNodes(), 0ull);
	ASSERT_EQ(x.getHead(), nullptr);
	ASSERT_EQ(x.getTail(), nullptr);

	x.pushBack(1);
	x.pushBack(5);

	ASSERT_EQ(x.getNumNodes(), 2ull);
	ASSERT_EQ(x.getHead()->value, x.getFirst());
	ASSERT_EQ(x.getTail()->value, x.getLast());
	ASSERT_EQ(x.getFirst(), 1);
	ASSERT_EQ(x.getLast(), 5);

	y.pushFront(2);
	y.pushFront(6);

	ASSERT_EQ(y.getNumNodes(), 2ull);
	ASSERT_EQ(y.getFirst(), 6);
	ASSERT_EQ(y.getLast(), 2);

	z.pushBack(1);

	ASSERT_EQ(z.getNumNodes(), 1ull);
	ASSERT_EQ(z.getHead(), z.getTail());
	ASSERT_EQ(z.getFirst(), 1);

	x.popBack();
	y.popFront();

	ASSERT_EQ(x.getNumNodes(), 1ull);
	ASSERT_EQ(x.getHead(), x.getTail());
	ASSERT_EQ(x.getFirst(), 1);
	ASSERT_EQ(y.getNumNodes(), 1ull);
	ASSERT_EQ(y.getHead(), y.getTail());
	ASSERT_EQ(y.getFirst(), 2);

	x.popFront();
	y.popBack();

	ASSERT_EQ(x.getNumNodes(), 0ull);
	ASSERT_EQ(x.getHead(), nullptr);
	ASSERT_EQ(x.getTail(), nullptr);
	ASSERT_EQ(y.getNumNodes(), 0ull);
	ASSERT_EQ(y.getHead(), nullptr);
	ASSERT_EQ(y.getTail(), nullptr);

	z.insertAfter(z.getHead(), 2);
	z.insertBefore(z.getHead(), 0);

	ASSERT_EQ(z.getNumNodes(), 3ull);
	ASSERT_EQ(z.getFirst(), 0);
	ASSERT_EQ(z.getLast(), 2);

	z.removeAt(z.getHead(), 2);

	ASSERT_EQ(z.getNumNodes(), 1ull);
	ASSERT_EQ(z.getFirst(), 2);

	for (int32 i = 0; i < 100; ++i)
	{
		x.pushBack(i);
	}
	
	ASSERT_EQ(x.getNumNodes(), 100ull);
	{
		int32 i = 0;
		for (auto it = x.begin(); it != x.end(); ++it)
		{
			ASSERT_EQ(*it, i++);
		}
	}

	x.reset();

	ASSERT_EQ(x.getNumNodes(), 0ull);
	ASSERT_EQ(x.getHead(), nullptr);
	ASSERT_EQ(x.getTail(), nullptr);

	x.pushBack(1);
	x.pushBack(2);
	x.pushBack(4);
	x.pushBack(1);

	List<int32> w{x};
	
	ASSERT_EQ(x.getNumNodes(), w.getNumNodes());
	for (auto xit = x.begin(), wit = w.begin(); xit != x.end(); ++xit, ++wit)
	{
		ASSERT_EQ(*xit, *wit);
	}

	x.pushBack(10);
	x.pushBack(6);
	y = x;
	
	ASSERT_EQ(x.getNumNodes(), y.getNumNodes());
	for (auto xit = x.begin(), yit = y.begin(); xit != x.end(); ++xit, ++yit)
	{
		ASSERT_EQ(*xit, *yit);
	}

	z.reset();
	z = move(y);

	ASSERT_EQ(y.getNumNodes(), 0ull);
	ASSERT_EQ(y.getHead(), nullptr);
	ASSERT_EQ(y.getTail(), nullptr);
	ASSERT_EQ(z.getNumNodes(), x.getNumNodes());
	for (auto xit = x.begin(), zit = z.begin(); xit != x.end(); ++xit, ++zit)
	{
		ASSERT_EQ(*xit, *zit);
	}

	x = w;

	ASSERT_EQ(x.getNumNodes(), w.getNumNodes());
	for (auto xit = x.begin(), wit = w.begin(); xit != x.end(); ++xit, ++wit)
	{
		ASSERT_EQ(*xit, *wit);
	}

	SUCCEED();
}

TEST(containers, tree)
{
	Tree<int32> x, y, z;

	ASSERT_EQ(x.getNumNodes(), 0ull);
	ASSERT_EQ(x.getRootNode(), nullptr);
	ASSERT_EQ(x.getMinNode(), nullptr);
	ASSERT_EQ(x.getMaxNode(), nullptr);

	x.insert(1);

	ASSERT_EQ(x.getNumNodes(), 1ull);
	ASSERT_NE(x.getRootNode(), nullptr);
	ASSERT_EQ(x.getRootNode(), x.getMinNode());
	ASSERT_EQ(x.getRootNode(), x.getMaxNode());
	ASSERT_EQ(x.getRootNode()->value, 1);

	x.insert(2);
	x.insert(3);

	ASSERT_EQ(x.getRootNode()->value, 2);
	ASSERT_EQ(x.getMinNode()->value, 1);
	ASSERT_EQ(x.getMaxNode()->value, 3);
	
	for (auto value : x)
	{
		ASSERT_EQ(x.findNode(value)->value, value);
	}

	x.removeNode(const_cast<BinaryNode<int32>*>(x.findNode(2)));
	x.removeNode(const_cast<BinaryNode<int32>*>(x.findNode(3)));

	ASSERT_EQ(x.getNumNodes(), 1ull);
	ASSERT_NE(x.getRootNode(), nullptr);
	ASSERT_EQ(x.getRootNode(), x.getMinNode());
	ASSERT_EQ(x.getRootNode(), x.getMaxNode());
	ASSERT_EQ(x.getRootNode()->value, 1);

	x.removeNode(const_cast<BinaryNode<int32>*>(x.findNode(1)));

	ASSERT_EQ(x.getNumNodes(), 0ull);
	ASSERT_EQ(x.getRootNode(), nullptr);
	ASSERT_EQ(x.getMinNode(), nullptr);
	ASSERT_EQ(x.getMaxNode(), nullptr);

	for (int32 i = 0; i < 100; ++i)
	{
		x.insert(rand() % 20);
	}

	ASSERT_EQ(x.getNumNodes(), 100ull);
	ASSERT_NE(x.getRootNode(), nullptr);

	for (auto it = x.begin(); it != x.end(); it = x.remove(it));

	ASSERT_EQ(x.getNumNodes(), 0ull);
	ASSERT_EQ(x.getRootNode(), nullptr);

	SUCCEED();
}
