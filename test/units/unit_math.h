#pragma once

#include "gtest/gtest.h"

#include "math/math.h"

using namespace Math;

TEST(math, Vec2)
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

TEST(math, Vec3)
{
	Vec3<int32> a, b, c;

	ASSERT_EQ(a.x, 0);
	ASSERT_EQ(a.y, 0);
	
	a = {3, 4, 5};

	ASSERT_EQ(a.x, 3);
	ASSERT_EQ(a.y, 4);
	ASSERT_EQ(a.z, 5);
	ASSERT_EQ(a[0], a.x);
	ASSERT_EQ(a[1], a.y);
	ASSERT_EQ(a[2], a.z);
	ASSERT_EQ(a.dot(a), 50);

	b = 4;

	ASSERT_EQ(b.x, 4);
	ASSERT_EQ(b.y, 4);
	ASSERT_EQ(b.z, 4);
	ASSERT_EQ(b.dot(b), 48);

	c = -a;

	ASSERT_EQ(c.x, -a.x);
	ASSERT_EQ(c.y, -a.y);
	ASSERT_EQ(c.z, -a.z);

	a += b;

	ASSERT_EQ(a.x, 7);
	ASSERT_EQ(a.y, 8);
	ASSERT_EQ(a.z, 9);

	a -= b;

	ASSERT_EQ(a.x, 3);
	ASSERT_EQ(a.y, 4);
	ASSERT_EQ(a.z, 5);

	a *= c;

	ASSERT_EQ(a.x, -9);
	ASSERT_EQ(a.y, -16);
	ASSERT_EQ(a.z, -25);

	a /= c;

	ASSERT_EQ(a.x, 3);
	ASSERT_EQ(a.y, 4);
	ASSERT_EQ(a.z, 5);

	a += 3;
	a -= 3;
	a *= 3;
	a /= 3;

	b = 9 + a;
	c = 3 * a;

	ASSERT_EQ(b.x, 12);
	ASSERT_EQ(b.y, 13);
	ASSERT_EQ(b.z, 14);
	ASSERT_EQ(c.x, 9);
	ASSERT_EQ(c.y, 12);
	ASSERT_EQ(c.z, 15);

	a = b.cross(c);

	ASSERT_EQ(a.x, 27);
	ASSERT_EQ(a.y, -54);
	ASSERT_EQ(a.z, 27);

	SUCCEED();
}
