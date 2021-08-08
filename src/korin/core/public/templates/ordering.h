#pragma once

#include "hal/platform.h"

/**
 * @brief Imposes ascending order.
 * 
 * See @c GreaterThan::operator()(auto const&, auto const&).
 */
struct GreaterThan
{
	/**
	 * @brief Returns a value indicating the
	 * ordering of two values.
	 * 
	 * @return negative if x < y
	 * @return positive if x > y
	 * @return zero if x == y
	 */
	constexpr FORCE_INLINE int32 operator()(auto const& x, auto const& y) const
	{
		return static_cast<int32>(x > y) - static_cast<int32>(x < y);
	}
};

/**
 * @brief Imposes descending order.
 * 
 * See @c LessThan::operator()(auto const&, auto const&).
 */
struct LessThan
{
	/**
	 * @brief Returns a value indicating the
	 * ordering of two values.
	 * 
	 * @return negative if x < y
	 * @return positive if x > y
	 * @return zero if x == y
	 */
	constexpr FORCE_INLINE int32 operator()(auto const& x, auto const& y) const
	{
		return static_cast<int32>(x < y) - static_cast<int32>(x > y);
	}
};
