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
};
