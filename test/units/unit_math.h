#pragma once

#include "gtest/gtest.h"

#include "math/math.h"
#include <math.h>

using namespace Math;

TEST(math, PlatformMath)
{
	constexpr float64 tol64 = 0.00001525878;
	constexpr float32 tol32 = 0.00390625f;
	constexpr int32 n = 1000;

#define TEST_FLOAT_FN(fn, ...) ASSERT_NEAR(PlatformMath::fn(__VA_ARGS__), ::fn##f(__VA_ARGS__), tol32);
#define TEST_DOUBLE_FN(fn, ...) ASSERT_NEAR(PlatformMath::fn(__VA_ARGS__), ::fn(__VA_ARGS__), tol64);

	for (int32 i = 1; i < n; ++i)
	{
		float32 x = rand() / (float32)RAND_MAX;
		float32 y = rand() / (float32)RAND_MAX;
		float32 xd = rand() / (float64)RAND_MAX;
		float32 yd = rand() / (float64)RAND_MAX;
		float32 z = x * i;

		ASSERT_NEAR(PlatformMath::abs(z), ::abs(z), tol32);
		TEST_FLOAT_FN(floor, x)
		TEST_FLOAT_FN(ceil, x)
		TEST_FLOAT_FN(round, x)
		TEST_DOUBLE_FN(abs, xd)
		TEST_DOUBLE_FN(floor, xd)
		TEST_DOUBLE_FN(ceil, xd)
		TEST_DOUBLE_FN(round, xd)

		TEST_FLOAT_FN(sin, x)
		TEST_FLOAT_FN(cos, x)
		TEST_FLOAT_FN(tan, x)
		TEST_FLOAT_FN(asin, x)
		TEST_FLOAT_FN(acos, x)
		TEST_FLOAT_FN(atan, x)
		TEST_FLOAT_FN(atan2, y, x)
		TEST_DOUBLE_FN(sin, xd)
		TEST_DOUBLE_FN(cos, xd)
		TEST_DOUBLE_FN(tan, xd)
		TEST_DOUBLE_FN(asin, xd)
		TEST_DOUBLE_FN(acos, xd)
		TEST_DOUBLE_FN(atan, xd)
		TEST_DOUBLE_FN(atan2, yd, xd)

		TEST_FLOAT_FN(sqrt, x)
		TEST_FLOAT_FN(pow, x, y)
		TEST_DOUBLE_FN(sqrt, xd)
		TEST_DOUBLE_FN(pow, xd, yd)

		ASSERT_NEAR(PlatformMath::finvsqrt(z), 1.f / ::sqrtf(z), tol32 * 64.f);
		ASSERT_NEAR(PlatformMath::fsqrt(z), ::sqrtf(z), tol32 * 64.f);

		TEST_FLOAT_FN(exp, x)
		TEST_FLOAT_FN(log, x)
		TEST_FLOAT_FN(log2, x)
		TEST_DOUBLE_FN(exp, xd)
		TEST_DOUBLE_FN(log, xd)
		TEST_DOUBLE_FN(log2, xd)
	}

	ASSERT_EQ(PlatformMath::powi(2, 11), 2048);
	ASSERT_NEAR(PlatformMath::powi(3., 7), ::pow(3., 7.), tol64);

	SUCCEED();
}

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
	ASSERT_EQ(a.z, 0);
	
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

TEST(math, Vec4)
{
	Vec4<int32> a, b, c;

	ASSERT_EQ(a.x, 0);
	ASSERT_EQ(a.y, 0);
	ASSERT_EQ(a.z, 0);
	ASSERT_EQ(a.w, 0);
	
	a = {3, 4, 5, 6};

	ASSERT_EQ(a.x, 3);
	ASSERT_EQ(a.y, 4);
	ASSERT_EQ(a.z, 5);
	ASSERT_EQ(a.w, 6);
	ASSERT_EQ(a[0], a.x);
	ASSERT_EQ(a[1], a.y);
	ASSERT_EQ(a[2], a.z);
	ASSERT_EQ(a[3], a.w);
	ASSERT_EQ(a.dot(a), 86);

	b = 4;

	ASSERT_EQ(b.x, 4);
	ASSERT_EQ(b.y, 4);
	ASSERT_EQ(b.z, 4);
	ASSERT_EQ(b.w, 4);
	ASSERT_EQ(b.dot(b), 64);

	c = -a;

	ASSERT_EQ(c.x, -a.x);
	ASSERT_EQ(c.y, -a.y);
	ASSERT_EQ(c.z, -a.z);
	ASSERT_EQ(c.w, -a.w);

	a += b;

	ASSERT_EQ(a.x, 7);
	ASSERT_EQ(a.y, 8);
	ASSERT_EQ(a.z, 9);
	ASSERT_EQ(a.w, 10);

	a -= b;

	ASSERT_EQ(a.x, 3);
	ASSERT_EQ(a.y, 4);
	ASSERT_EQ(a.z, 5);
	ASSERT_EQ(a.w, 6);

	a *= c;

	ASSERT_EQ(a.x, -9);
	ASSERT_EQ(a.y, -16);
	ASSERT_EQ(a.z, -25);
	ASSERT_EQ(a.w, -36);

	a /= c;

	ASSERT_EQ(a.x, 3);
	ASSERT_EQ(a.y, 4);
	ASSERT_EQ(a.z, 5);
	ASSERT_EQ(a.w, 6);

	a += 3;
	a -= 3;
	a *= 3;
	a /= 3;

	b = 9 + a;
	c = 3 * a;

	ASSERT_EQ(b.x, 12);
	ASSERT_EQ(b.y, 13);
	ASSERT_EQ(b.z, 14);
	ASSERT_EQ(b.w, 15);
	ASSERT_EQ(c.x, 9);
	ASSERT_EQ(c.y, 12);
	ASSERT_EQ(c.z, 15);
	ASSERT_EQ(c.w, 18);

	SUCCEED();
}

TEST(math, Quat)
{
	Quat<float32> q, r, p;
	Vec3<float32> i, j, k;
	float32 s, t;

	q = {M_PI_4, {0.f, 1.f, 0.f}};
	q.getAxisAndAngle(i, t);

	ASSERT_FLOAT_EQ(t, M_PI_4 * PlatformMath::signum(i.y)); // Just in case axis has opposite direction
	ASSERT_FLOAT_EQ(i.x, 0.f);
	ASSERT_FLOAT_EQ(i.y, PlatformMath::signum(t));
	ASSERT_FLOAT_EQ(i.z, 0.f);

	r = {M_PI_2, {0.f, 0.f, 1.f}};
	i = r.rotateVector({1.f, 0.f, 1.f});

	EXPECT_FLOAT_EQ(i.x, 0.f);
	EXPECT_FLOAT_EQ(i.y, 1.f);
	EXPECT_FLOAT_EQ(i.z, 1.f);

	p = q * r;
	j = p.rotateVector(i);
	k = q.rotateVector(r.rotateVector(i));
	
	EXPECT_FLOAT_EQ(j.x, k.x);
	EXPECT_FLOAT_EQ(j.y, k.y);
	EXPECT_FLOAT_EQ(j.z, k.z);

	j = (!p).rotateVector(k);

	EXPECT_FLOAT_EQ(j.x, i.x);
	EXPECT_FLOAT_EQ(j.y, i.y);
	EXPECT_FLOAT_EQ(j.z, i.z);

	SUCCEED();
}
