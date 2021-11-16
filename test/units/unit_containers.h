#pragma once

#include "gtest/gtest.h"
#include "testing.h"

using namespace Korin;

#include "hal/platform_crt.h"
#include "containers/containers.h"

#define ARRAY_LEN(x) (sizeof(x) / sizeof(*x))

template<typename ItT>
static void shuffle(ItT begin, ItT end)
{
	// Shuffle
}

/* Set of names to use for testing. */
static constexpr char const* names[] = {
	"sneppy",
	"lpraat",
	"nondecibile",
	"nicofico",
	"lorecri96",
	"camram01",
	"SneppyRulez",
	"sneppy13",
	"korin",
	"ZerryBlack95",
	"sgherry",
	"SamN884",
	"set"
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

	Optional<Testing::Object> z{{}}, w;

	ASSERT_TRUE(z.hasValue());
	ASSERT_FALSE(w.hasValue());

	w = Testing::Object{};

	ASSERT_TRUE(w.hasValue());

	z->test();
	w->test();

	SUCCEED();
}

// TODO: Test tuples

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
	struct NodeData
	{
		int32 value;
	};

	using Node = BinaryNodeBase<NodeData>;

	constexpr uint32 numNodes = 1024;
	Node* nodes[numNodes] = {};
	Node* root = nullptr;

	for (int32 i = 0; i < numNodes; ++i)
	{
		auto* node = new Node{};
		node->value = i;
		root = TreeNode::insert(root, node, [node](auto const* other) {

			return GreaterThan{}(node->value, other->value);
		});
	}

	for (int32 i = 0; i < numNodes; ++i)
	{
		auto* node = TreeNode::find(root, [i](auto const* node) {

			return GreaterThan{}(i, node->value);
		});
		ASSERT_EQ(node->value, i);
	}

	for (int32 i = 0; i < numNodes; ++i)
	{
		auto* node = TreeNode::find(root, [i](auto const* node) {

			return GreaterThan{}(i, node->value);
		});
		root = TreeNode::remove(node);
		delete node;
	}

	int32 values[numNodes] = {};
	for (int32 i = 0; i < numNodes; ++i)
	{
		// Generate some collisions
		values[i] = rand() % (numNodes << 4);
	}

	for (int32 i = 0; i < numNodes; ++i)
	{
		auto* node = new Node{};
		node->value = values[i];
		root = TreeNode::insert(root, node, [node](auto const* other) {

			return GreaterThan{}(node->value, other->value);
		});
	}

	for (int32 i = 0; i < numNodes; ++i)
	{
		auto const value = values[i];
		auto* node = TreeNode::find(root, [value](auto const* node) {

			return GreaterThan{}(value, node->value);
		});
		ASSERT_NE(node, nullptr);
		ASSERT_EQ(node->value, value);
	}

	for (int32 i = 0; i < numNodes; ++i)
	{
		auto const value = values[i];
		auto* node = TreeNode::find(root, [value](auto const* node) {

			return GreaterThan{}(value, node->value);
		});
		root = TreeNode::remove(node);
		delete node;
	}

	for (int32 i = 0; i < numNodes; ++i)
	{
		auto const value = values[i];
		auto* node = TreeNode::find(root, [value](auto const* node) {

			return GreaterThan{}(value, node->value);
		});
		ASSERT_EQ(node, nullptr);
	}

	SUCCEED();
}

TEST(containers, Tree)
{
	struct FindObject
	{
		int32 operator()(Testing::Object const& lhs, Testing::Object const& rhs) const
		{
			return GreaterThan{}(lhs.getSize(), rhs.getSize());
		}
	};

	Tree<Testing::Object, FindObject> x, y, z;

	static constexpr int32 numValues = 1 << 10;
	int32 values[numValues] = {};
	for (int32 i = 0; i < numValues; ++i)
	{
		values[i] = 1 + (rand() & 0xf);
	}

	for (int32 i = 0; i < numValues; ++i)
	{
		auto it = x.emplace(values[i]);
		ASSERT_NE(it, x.end());
		ASSERT_EQ(it->getSize(), values[i]);
	}

	ASSERT_EQ(x.getNumNodes(), numValues);

	for (int32 i = 0; i < numValues; ++i)
	{
		auto it = x.find(values[i]);
		ASSERT_NE(it, x.end());
		ASSERT_EQ(it->getSize(), values[i]);
	}

	{
		sizet prev = 0;
		for (auto const& obj : x)
		{
			ASSERT_GE(obj.getSize(), prev);
			prev = obj.getSize();
		}
	}

	y = x;

	{
		auto it = x.begin();
		auto jt = y.begin();
		ASSERT_EQ(y.getNumNodes(), numValues);
		for (; it != x.end() && jt != y.end(); ++it, ++jt)
		{
			ASSERT_EQ(it->getSize(), jt->getSize());
		}
		ASSERT_EQ(it, x.end());
		ASSERT_EQ(jt, y.end());
	}

	z = move(x);

	ASSERT_EQ(z.getNumNodes(), numValues);
	ASSERT_EQ(x.getNumNodes(), 0);

	{
		auto kt = z.begin();
		auto jt = y.begin();
		ASSERT_EQ(y.getNumNodes(), numValues);
		for (; kt != z.end() && jt != y.end(); ++kt, ++jt)
		{
			ASSERT_EQ(kt->getSize(), jt->getSize());
		}
		ASSERT_EQ(kt, z.end());
		ASSERT_EQ(jt, y.end());
	}

	for (auto it = y.begin(); it != y.end();)
	{
		if (it->getSize() & 0x1)
		{
			it = y.remove(it);
		}
		else
		{
			++it;
		}
	}

	for (auto it = z.begin(); it != z.end();)
	{
		it = z.remove(it);
	}

	for (int32 i = 0; i < numValues; ++i)
	{
		auto it = z.insert(values[i]);
		ASSERT_NE(it, z.end());
		ASSERT_EQ(it->getSize(), values[i]);
	}

	for (int32 i = 0; i < numValues; ++i)
	{
		auto it = z.find(values[i]);
		ASSERT_NE(it, z.end());
		ASSERT_EQ(it->getSize(), values[i]);
		z.remove(it);
	}

	y.~Tree();
	z.~Tree();

	SUCCEED();
}

TEST(containers, Set)
{
	Set<int32> x, y, z;

	ASSERT_EQ(x.getSize(), 0ull);

	x.insert(10);
	x.insert(1);
	x.insert(3);

	ASSERT_EQ(x.getSize(), 3ull);
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

	ASSERT_EQ(x.getSize(), 6ull);
	ASSERT_TRUE(x.has(3));
	ASSERT_TRUE(x.has(10));
	ASSERT_TRUE(x.has(2));

	x &= y;

	ASSERT_EQ(x.getSize(), y.getSize());
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

	ASSERT_EQ(x.getSize(), 2ull);
	ASSERT_TRUE(x.has(2));
	ASSERT_TRUE(x.has(3));
	ASSERT_FALSE(x.has(0));
	ASSERT_FALSE(x.has(9));

	SUCCEED();
}

TEST(containers, Map)
{
	Map<String, Testing::Object> x, y, z;

	ASSERT_EQ(x.getSize(), 0);
	ASSERT_EQ(x.begin(), x.end());
	ASSERT_EQ(x.rbegin(), x.rend());
	ASSERT_EQ(x.find("sneppy"), x.end());

	static char const* names[] = {
		"sneppy",
		"fmonz",
		"lorecri96",
		"lpraat",
		"nondecidibile",
		"cam",
		"nicofico",
		"nonswiss",
		"turingcomplete",
		"nov4chip",
		"samn844",
		"zerry",
		"sgherry",
		"frabellazio",
		"gu",
		"xxx",
		"fryscan",
		"ananasso"
	};
	static sizet values[ARRAY_LEN(names)] = {};
	static sizet removeIdxs[] = {2, 5, 7, 9, 11, 13};

	for (int32 i = 0; i < ARRAY_LEN(names); ++i)
	{
		x.emplace(names[i], 0x1ull << (i & 0xf));
	}

	ASSERT_EQ(x.getSize(), ARRAY_LEN(names));
	for (int32 i = 0; i < ARRAY_LEN(names); ++i)
	{
		auto it = x.find(names[i]);
		ASSERT_NE(it, x.end());
		ASSERT_EQ(it->first, names[i]);
		ASSERT_EQ(it->second.getSize(), 0x1ull << (i & 0xf));

		const auto& obj = x[names[i]];
		ASSERT_EQ(obj.getSize(), it->second.getSize());
	}

	for (int32 i = 0; i < ARRAY_LEN(names); ++i)
	{
		values[i] = rand() & 0xff;
		x.emplace(names[i], values[i]);
	}

	ASSERT_EQ(x.getSize(), ARRAY_LEN(names));
	for (int32 i = 0; i < ARRAY_LEN(names); ++i)
	{
		ASSERT_EQ(x[names[i]].getSize(), values[i]);
	}

	for (int32 i = 0; i < ARRAY_LEN(removeIdxs); ++i)
	{
		Testing::Object obj;
		x.removeAt(names[removeIdxs[i]], obj);

		ASSERT_EQ(obj.getSize(), values[removeIdxs[i]]);
	}

	ASSERT_EQ(x.getSize(), ARRAY_LEN(names) - ARRAY_LEN(removeIdxs));
	for (int32 i = 0; i < ARRAY_LEN(removeIdxs); ++i)
	{
		auto it = x.find(names[removeIdxs[i]]);
		ASSERT_EQ(it, x.end());
	}

	// TODO: Test assignment and copy/move constructors

	for (auto it = x.begin(); it != x.end(); it = x.remove(it));

	SUCCEED();
}

TEST(containers, HashMap)
{
	HashMap<String, Testing::Object> m;

	ASSERT_EQ(m.getSize(), 0);

	for (int i = 0; i < ARRAY_LEN(names); ++i)
	{
		auto it = m.emplace(names[i], i);
		ASSERT_EQ(it->first, names[i]);
		ASSERT_EQ(it->second.getSize(), i);
	}
	ASSERT_EQ(m.getSize(), ARRAY_LEN(names));
	for (int i = 0; i < ARRAY_LEN(names); ++i)
	{
		auto it = m.find(names[i]);
		ASSERT_NE(it, m.end());
		ASSERT_EQ(it->first, names[i]);
		ASSERT_EQ(it->second.getSize(), i);
	}

	for (int i = 0; i < ARRAY_LEN(names); ++i)
	{
		auto it = m.find(names[i]);
		ASSERT_EQ(m.emplace(names[i], i), it);
	}
	ASSERT_EQ(m.getSize(), ARRAY_LEN(names));

	m.clear();

	ASSERT_EQ(m.getSize(), 0);
	for (int i = 0; i < ARRAY_LEN(names); ++i)
	{
		auto it = m.find(names[i]);
		ASSERT_EQ(it, m.end());
	}

	for (int i = 0; i < ARRAY_LEN(names); ++i)
	{
		ASSERT_EQ(m[names[i]].getSize(), Testing::Object{}.getSize());
	}

	m.clear();

	for (int i = 0; i < ARRAY_LEN(names); ++i)
	{
		auto it = m.insert({names[i], i});
		ASSERT_EQ(it->first, names[i]);
		ASSERT_EQ(it->second.getSize(), i);
	}
	ASSERT_EQ(m.getSize(), ARRAY_LEN(names));

	for (int i = 0; i < ARRAY_LEN(names); i += 2)
	{
		auto it = m.find(names[i]);
		m.remove(it);
	}

	ASSERT_EQ(m.getSize(), ARRAY_LEN(names) / 2);
	for (int i = 0; i < ARRAY_LEN(names); ++i)
	{
		auto it = m.find(names[i]);
		if (i & 0x1)
		{
			ASSERT_NE(it, m.end());
			m.remove(it);
		}
		else
		{
			ASSERT_EQ(it, m.end());
		}
	}
	ASSERT_EQ(m.getSize(), 0);

	for (int i = 0; i < ARRAY_LEN(names); ++i)
	{
		auto it = m.emplace(names[i], i);
	}

	{
		int i = 0;
		for (auto it = m.begin(); it != m.end(); it = m.remove(it)) i++;
		ASSERT_EQ(i, ARRAY_LEN(names));
	}
	ASSERT_EQ(m.getSize(), 0);

	for (int i = 0; i < ARRAY_LEN(names); ++i)
	{
		auto it = m.emplace(names[i], i);
	}
	auto n{m};

	ASSERT_EQ(n.getSize(), m.getSize());
	for (auto const& v : m)
	{
		auto it = n.find(v.first);
		ASSERT_NE(it, n.end());
		ASSERT_EQ(it->first, v.first);
		ASSERT_EQ(it->second.getSize(), v.second.getSize());
	}

	auto o{move(n)};

	ASSERT_EQ(o.getSize(), m.getSize());
	for (auto const& v : m)
	{
		auto it = o.find(v.first);
		ASSERT_NE(it, o.end());
		ASSERT_EQ(it->first, v.first);
		ASSERT_EQ(it->second.getSize(), v.second.getSize());
	}

	new (&n) decltype(m){m};
	n = o;

	ASSERT_EQ(n.getSize(), o.getSize());
	for (auto const& v : o)
	{
		auto it = n.find(v.first);
		ASSERT_NE(it, n.end());
		ASSERT_EQ(it->first, v.first);
		ASSERT_EQ(it->second.getSize(), v.second.getSize());
	}

	for (int i = 0; i < ARRAY_LEN(names); i += 2)
	{
		auto it = n.find(names[i]);
		n.remove(it);
	}

	o = move(n);

	ASSERT_EQ(o.getSize(), ARRAY_LEN(names) / 2);
	for (int i = 0; i < ARRAY_LEN(names); ++i)
	{
		auto it = o.find(names[i]);
		ASSERT_TRUE((i & 0x1 && it != o.end()) || (!(i & 0x1) && it == o.end()));
	}

	SUCCEED();
}

TEST(containers, HashSet)
{
	// TODO
	SUCCEED();
}
