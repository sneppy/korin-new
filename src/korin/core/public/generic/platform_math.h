#pragma once

#include "core_types.h"

#if PLATFORM_USE_GCEM_LIB
#	include "gcem.hpp"
#	define MATH_FN_FLOAT_32(name) gcem::name
#	define MATH_FN_FLOAT_64(name) gcem::name
#else
#	include <math.h>
#	define MATH_FN_FLOAT_32(name) ::name##f
#	define MATH_FN_FLOAT_64(name) ::name
#endif

/**
 * @brief Platform-independent implementation of
 * common math functions.
 */
struct GenericPlatformMath
{
	/* Returns the absolute value of x. */
	/** @{ */
	static constexpr FORCE_INLINE float32 abs(float32 x) { return MATH_FN_FLOAT_32(abs)(x); }
	static constexpr FORCE_INLINE float64 abs(float64 x) { return MATH_FN_FLOAT_64(abs)(x); }
	/** @} */

	/**
	 * @brief Returns the sign of x.
	 *
	 * The sign of 0 is undefined.
	 *
	 * @param x value to compute the sign of
	 * @return sgn(x)
	 */
	template<typename T>
	static constexpr FORCE_INLINE T signum(T const& x)
	{
		return static_cast<T>(x > T{0}) - static_cast<T>(x < T{0});
	}

	/* Return floor of x. */
	/** @{ */
	static constexpr FORCE_INLINE float32 floor(float32 x) { return MATH_FN_FLOAT_32(floor)(x); }
	static constexpr FORCE_INLINE float64 floor(float64 x) { return MATH_FN_FLOAT_64(floor)(x); }
	/** @} */

	/* Return ceil of x. */
	/** @{ */
	static constexpr FORCE_INLINE float32 ceil(float32 x) { return MATH_FN_FLOAT_32(ceil)(x); }
	static constexpr FORCE_INLINE float64 ceil(float64 x) { return MATH_FN_FLOAT_64(ceil)(x); }
	/** @} */

	/* Round x to closest integer value. */
	/** @{ */
	static constexpr FORCE_INLINE float32 round(float32 x) { return MATH_FN_FLOAT_32(round)(x); }
	static constexpr FORCE_INLINE float64 round(float64 x) { return MATH_FN_FLOAT_64(round)(x); }
	/** @} */

	/* Compute the sine of x. */
	/** @{ */
	static constexpr FORCE_INLINE float32 sin(float32 x) { return MATH_FN_FLOAT_32(sin)(x); }
	static constexpr FORCE_INLINE float64 sin(float64 x) { return MATH_FN_FLOAT_64(sin)(x); }
	/** @} */

	/* Compute the cosine of x. */
	/** @{ */
	static constexpr FORCE_INLINE float32 cos(float32 x) { return MATH_FN_FLOAT_32(cos)(x); }
	static constexpr FORCE_INLINE float64 cos(float64 x) { return MATH_FN_FLOAT_64(cos)(x); }
	/** @} */

	/* Compute the tangent of x. */
	/** @{ */
	static constexpr FORCE_INLINE float32 tan(float32 x) { return MATH_FN_FLOAT_32(tan)(x); }
	static constexpr FORCE_INLINE float64 tan(float64 x) { return MATH_FN_FLOAT_64(tan)(x); }
	/** @} */

	/* Compute the arc sine of x. */
	/** @{ */
	static constexpr FORCE_INLINE float32 asin(float32 x) { return MATH_FN_FLOAT_32(asin)(x); }
	static constexpr FORCE_INLINE float64 asin(float64 x) { return MATH_FN_FLOAT_64(asin)(x); }
	/** @} */

	/* Compute the arc cosine of x. */
	/** @{ */
	static constexpr FORCE_INLINE float32 acos(float32 x) { return MATH_FN_FLOAT_32(acos)(x); }
	static constexpr FORCE_INLINE float64 acos(float64 x) { return MATH_FN_FLOAT_64(acos)(x); }

	/** Returns the principal arc tangent of x. */
	/** @{ */
	static constexpr FORCE_INLINE float32 atan(float32 x) { return MATH_FN_FLOAT_32(atan)(x); }
	static constexpr FORCE_INLINE float64 atan(float64 x) { return MATH_FN_FLOAT_64(atan)(x); }
	/** @} */

	/**
	 * @brief Compute the arc tangent given
	 * the sine and cosine of the angle.
	 *
	 * @param y the sine of the angle
	 * @param x the cosine of the angle
	 * @return atan(z) where sin(z) == y and
	 * cos(z) == x
	 * @{
	 */
	static constexpr FORCE_INLINE float32 atan2(float32 y, float32 x) { return MATH_FN_FLOAT_32(atan2)(y, x); }
	static constexpr FORCE_INLINE float64 atan2(float64 y, float64 x) { return MATH_FN_FLOAT_64(atan2)(y, x); }
	/** @} */

	/** Compute the square root of x. */
	/** @{ */
	static constexpr FORCE_INLINE float32 sqrt(float32 x) { return MATH_FN_FLOAT_32(sqrt)(x); }
	static constexpr FORCE_INLINE float64 sqrt(float64 x) { return MATH_FN_FLOAT_64(sqrt)(x); }
	/** @} */

	/** Compute x to the power of y. */
	/** @{ */
	static constexpr FORCE_INLINE float32 pow(float32 x, float32 y) { return MATH_FN_FLOAT_32(pow)(x, y); }
	static constexpr FORCE_INLINE float64 pow(float64 x, float64 y) { return MATH_FN_FLOAT_64(pow)(x, y); }
	/** @} */

	/**
	 * @brief Aggressive inverse square root
	 * of x with low accuracy.
	 */
	static constexpr FORCE_INLINE float32 finvsqrt(float32 x)
	{
		// https://betterexplained.com/articles/understanding-quakes-fast-inverse-square-root/
		constexpr int32 magic = 0x5f3759df;
		float32 h = x * 0.5f;
		int32 n = *(int32*)&x;
		n = magic - (n >> 1);
		x = *(float32*)&n;
		x = x * (1.5f - h * x * x);
		return x;
	}

	/**
	 * @brief Aggressive square root of x
	 * with low accuracy.
	 */
	static constexpr FORCE_INLINE float32 fsqrt(float32 x)
	{
		return 1.f / finvsqrt(x);
	}

	/* Compute e to the power of x. */
	/** @{ */
	static constexpr FORCE_INLINE float32 exp(float32 x) { return MATH_FN_FLOAT_32(exp)(x); }
	static constexpr FORCE_INLINE float64 exp(float64 x) { return MATH_FN_FLOAT_64(exp)(x); }
	/** @} */

	/* Compute the natural logarithm of x. */
	/** @{ */
	static constexpr FORCE_INLINE float32 log(float32 x) { return MATH_FN_FLOAT_32(log)(x); }
	static constexpr FORCE_INLINE float64 log(float64 x) { return MATH_FN_FLOAT_64(log)(x); }
	/** @} */

	/* Compute the logarithm base 2 of x. */
	/** @{ */
	static constexpr FORCE_INLINE float32 log2(float32 x) { return MATH_FN_FLOAT_32(log2)(x); }
	static constexpr FORCE_INLINE float64 log2(float64 x) { return MATH_FN_FLOAT_64(log2)(x); }
	/** @} */

	/**
	 * @brief Compute x to the power of y, where
	 * y must be a non-negative integer.
	 *
	 * @tparam T the type of the base
	 * @param x base value
	 * @param y exponent value
	 * @return x^y
	 */
	template<typename T>
	static constexpr FORCE_INLINE T powi(T const& x, int64 y)
	{
		CHECK(!(y < 0));

		switch (y)
		{
		case 0: return T{1};
		case 1: return x;
		default:;
		}

		auto n = x;
		auto m = T{1};
		for (; y > 1; y = y >> 1)
		{
			if (y & 0x1) m *= n;
			n *= n;
		}

		return m * n;
	}

	/**
	 * @brief Returns the integer base 2 logarithm of the
	 * given number.
	 *
	 * This is equivalent to the position of the most
	 * significant non-zero bit.
	 *
	 * If the value is zero, the result is undefined.
	 *
	 * @param n the value to compute the log of
	 * @return floor[log2(n)]
	 */
	static constexpr FORCE_INLINE uint64 log2i(uint64 n)
	{
		uint64 p = 0;
		uint64 k = 32;

		for (; n != 0 && k != 0; k /= 2)
		{
			if (n >> k)
			{
				n = n >> k;
				p += k;
			}
		}

		return p;
	}

	/**
	 * @brief Return the number of non-zero bits modulo 2 in
	 * the given value.
	 *
	 * @param n the value to compute the parity of
	 * @return number of 1s modulo 2
	 */
	static constexpr FORCE_INLINE uint64 parity(uint64 n)
	{
		n = (n >> 32) ^ (n & 0xffffffff);
		n = (n >> 16) ^ (n & 0xffff);
		n = (n >> 8) ^ (n & 0xff);
		n = (n >> 4) ^ (n & 0xf);
		n = (n >> 2) ^ (n & 0b11);
		n = (n >> 1) ^ (n & 0b1);
		return n;
	}

	/**
	 * @brief Returns true if the given value is a power of
	 * 2.
	 *
	 * @param n the value to test
	 * @return true if value is power of 2
	 * @return false otherwise
	 */
	static constexpr FORCE_INLINE bool isPowerOf2(uint64 n)
	{
		return (n & (n - 1)) == 0;
	}

	/**
	 * @brief Returns a new value equal to the closest
	 * multiple of @c p and smaller than the given value.
	 *
	 * @c p must be a power of 2. If @c p is not a power of
	 * 2, the result is undefined.
	 *
	 * @param n the value to align
	 * @param p the required alignment
	 * @return value aligned to smaller value
	 */
	static constexpr FORCE_INLINE bool align2Down(uint64 n, uint64 p)
	{
		return n & ~(p - 1);
	}

	/**
	 * @brief Similar to align2Down(), but the returned
	 * value is larger than the given value.
	 *
	 * @param n the value to align
	 * @param p the required alignment
	 * @return value aligned to larger value
	 */
	static constexpr FORCE_INLINE bool align2Up(uint64 n, uint64 p)
	{
		return align2Down(n + p - 1, p);
	}

	/**
	 * @brief Given a value, returns the previous power of
	 * 2.
	 *
	 * For instance, 17 -> 16, 80 -> 64, 128 -> 128, etc.
	 *
	 * If the given value is a power of two, the result is
	 * the value itself.
	 *
	 * @param n the given value
	 * @return previous power of 2
	 */
	static constexpr FORCE_INLINE uint64 closestSmallerPowerOf2(uint64 n)
	{
		return 1ull << log2i(n);
	}

	/**
	 * @brief Similar to closestSmallerPowerOf2(), but
	 * returns the next power of two
	 *
	 * @param n the given value
	 * @return next power of 2
	 */
	static constexpr FORCE_INLINE uint64 closestLargerPowerOf2(uint64 n)
	{
		return 1ull << (log2i(n - 1) + 1);
	}
};
