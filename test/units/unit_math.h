#pragma once

#include "gtest/gtest.h"

#include "math/math.h"

using namespace Math;

TEST(math, vec2)
{
	Vec2<int32> a, b, c;

	ASSERT_EQ(a.x, 0);
	ASSERT_EQ(a.y, 0);
	
	a = {3, 4};

	ASSERT_EQ(a.x, 3);
	ASSERT_EQ(a.y, 4);
	ASSERT_EQ(a[0], a.x);
	ASSERT_EQ(a[1], a.y);
	ASSERT_EQ(a.dot(a), 25);

	b = 4;

	ASSERT_EQ(b.x, 4);
	ASSERT_EQ(b.y, 4);
	ASSERT_EQ(b.dot(b), 32);

	c = -a;

	ASSERT_EQ(c.x, -a.x);
	ASSERT_EQ(c.y, -a.y);

	a += b;

	ASSERT_EQ(a.x, 7);
	ASSERT_EQ(a.y, 8);

	a -= b;

	ASSERT_EQ(a.x, 3);
	ASSERT_EQ(a.y, 4);

	a *= c;

	ASSERT_EQ(a.x, -9);
	ASSERT_EQ(a.y, -16);

	a /= c;

	ASSERT_EQ(a.x, 3);
	ASSERT_EQ(a.y, 4);

	a += 3;
	a -= 3;
	a *= 3;
	a /= 3;

	b = 9 + a;
	c = 3 * a;

	ASSERT_EQ(b.x, 12);
	ASSERT_EQ(b.y, 13);
	ASSERT_EQ(c.x, 9);
	ASSERT_EQ(c.y, 12);

	SUCCEED();
}
