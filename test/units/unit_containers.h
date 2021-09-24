#pragma once

#include "gtest/gtest.h"

#include "hal/platform_crt.h"
#include "containers/containers.h"

template<typename ItT>
static void shuffle(ItT begin, ItT end)
{
	// Shuffle
}

using namespace Korin;

struct Object
{
	Object()
	{
		mem = ::malloc(100);
	}

	Object(Object const& other)
	{
		mem = ::malloc(100);
		::memcpy(mem, other.mem, 100);
	}

	Object(Object&& other)
	{
		mem = other.mem;
		other.mem = nullptr;
	}

	Object& operator=(Object const& other)
	{
		::memcpy(mem, other.mem, 100);
		return *this;
	}

	Object& operator=(Object&& other)
	{
		::free(mem);
		mem = other.mem;
		other.mem = nullptr;
		return *this;
	}

	~Object()
	{
		if (mem)
		{
			::free(mem);
		}
	}

	void test()
	{
		char* it = reinterpret_cast<char*>(mem);
		for (char i = 0; i < 100; ++i)
		{
			*it = i;
		}
	}

protected:
	void* mem;
};

TEST(containers, Optional)
{
	Optional<int32> x, y;

	ASSERT_FALSE(x.hasValue());
	ASSERT_FALSE(y.hasValue());
	ASSERT_FALSE(static_cast<bool>(x)); // Test operator

	x = 1;
	y = 3;

	ASSERT_TRUE(x.hasValue());
	ASSERT_TRUE(y.hasValue());
	ASSERT_EQ(x, 1);
	ASSERT_EQ(y, 3);

	x.reset();
	y.reset();

	ASSERT_FALSE(x.hasValue());
	ASSERT_FALSE(y.hasValue());

	Optional<Object> z{{}}, w;

	ASSERT_TRUE(z.hasValue());
	ASSERT_FALSE(w.hasValue());

	w = Object{};

	ASSERT_TRUE(w.hasValue());

	z->test();
	w->test();

	SUCCEED();
}

TEST(containers, Array)
{
	Array<int32> x, y;

	ASSERT_EQ(x.getNumItems(), 0ull);
	ASSERT_EQ(len(x), x.getNumItems());
	ASSERT_EQ(*x, nullptr);

	x.append(5, 1, 76);

	ASSERT_EQ(x.getNumItems(), 3ull);
	ASSERT_NE(*x, nullptr);
	ASSERT_EQ(*x, x.begin());
	ASSERT_EQ(*x.begin(), 5);
	ASSERT_EQ(*x.rbegin(), 76);
	ASSERT_EQ(x[0], 5);
	ASSERT_EQ(x[1], 1);
	ASSERT_EQ(x[2], 76);

	x.removeAt(0, 2);

	ASSERT_EQ(x.getNumItems(), 1ull);
	ASSERT_EQ(x.begin(), x.rbegin());
	ASSERT_EQ(x[0], 76);

	x.insert(0, 5, 6, 2);

	ASSERT_EQ(x.getNumItems(), 4ull);
	ASSERT_EQ(x[0], 5);
	ASSERT_EQ(x[1], 6);
	ASSERT_EQ(x[2], 2);
	ASSERT_EQ(x[3], 76);

	{
		Array<Array<int32>> z;

		z.append(x, y);
		z.append(Array<int32>{});

		ASSERT_EQ(z.getNumItems(), 3ull);
		ASSERT_EQ(z[0].getNumItems(), 4ull);
		ASSERT_EQ(z[1].getNumItems(), 0ull);
		ASSERT_EQ(z[0][2], 2);
		ASSERT_EQ(z[0][3], 76);
	}

	Array<int32> z;

	y.append(0, 3);
	z.append(5, 1, 2);
	x.concat(y, z);

	ASSERT_EQ(x.getNumItems(), 9ull);
	ASSERT_EQ(x[4], 0);
	ASSERT_EQ(x[5], 3);
	ASSERT_EQ(x[7], 1);

	y = x.slice(3);
	z = x.slice(0, 4);

	ASSERT_EQ(y.getNumItems(), 6);
	ASSERT_EQ(z.getNumItems(), 4);
	ASSERT_EQ(y[0], 76);
	ASSERT_EQ(y[3], x[6]);
	ASSERT_EQ(y[5], x[8]);
	ASSERT_EQ(z[3], 76);
	ASSERT_EQ(z[2], x[2]);

	SUCCEED();
}

TEST(containers, List)
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

TEST(containers, TreeNode)
{
	struct Node : public BinaryNodeBase<Node>
	{
		// Node value
		int32 value;
	};

	constexpr uint32 numNodes = 1024;
	Node* nodes[numNodes] = {};
	Node* root = new Node{};
	root->color = BinaryNodeColor::Color_BLACK;
	root->value = 0;
	nodes[0] = root;

	for (uint32 i = 1; i < numNodes; ++i)
	{
		nodes[i] = new Node{};
		nodes[i]->value = i;
		root = TreeNode::insert(root, nodes[i], [](auto* node, auto* other) {

			return GreaterThan{}(node->value, other->value);
		});
	}

	for (int32 i = 0; i < numNodes; ++i)
	{
		auto* node = TreeNode::find(root, [i](auto const* node) {

			return GreaterThan{}(i, node->value);
		});
	}

	// Shuffle nodes
	shuffle(nodes, nodes + numNodes);

	for (int32 i = 1; i < numNodes; ++i)
	{
		root = TreeNode::remove(nodes[i]);
		delete nodes[i];
	}

	SUCCEED();
}

TEST(containers, Tree)
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

TEST(containers, Set)
{
	Set<int32> x, y, z;

	ASSERT_EQ(x.getNumItems(), 0ull);

	x.insert(10);
	x.insert(1);
	x.insert(3);

	ASSERT_EQ(x.getNumItems(), 3ull);
	ASSERT_NE(x.get(1), x.end());
	ASSERT_EQ(x.get(0), x.end());
	ASSERT_EQ(*x.get(1), 1);
	ASSERT_EQ(*x.get(3), 3);
	ASSERT_EQ(*x.get(10), 10);
	ASSERT_EQ(x.begin(), x.get(1));
	ASSERT_EQ(x.rbegin(), x.get(10));
	ASSERT_TRUE(x.has(1));
	ASSERT_TRUE(x.has(10));
	ASSERT_FALSE(x.has(8));

	y.insert(2);
	y.insert(0);
	y.insert(9);
	y.insert(3);
	x |= y;

	ASSERT_EQ(x.getNumItems(), 6ull);
	ASSERT_TRUE(x.has(3));
	ASSERT_TRUE(x.has(10));
	ASSERT_TRUE(x.has(2));

	x &= y;

	ASSERT_EQ(x.getNumItems(), y.getNumItems());
	for (auto v : y)
	{
		ASSERT_TRUE(x.has(v));
		ASSERT_EQ(*x.get(v), v);
	}

	z.insert(1);
	z.insert(0);
	z.insert(9);
	z.insert(10);
	x ^= z;

	ASSERT_EQ(x.getNumItems(), 2ull);
	ASSERT_TRUE(x.has(2));
	ASSERT_TRUE(x.has(3));
	ASSERT_FALSE(x.has(0));
	ASSERT_FALSE(x.has(9));

	SUCCEED();
}
