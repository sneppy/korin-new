#pragma once

#include "core_types.h"
#include "types.h"

/**
 * @brief Cast any type to an r-value reference
 * of the same type.
 * 
 * @param x value to cast
 * @return value cast to r-value reference
 */
template<typename T>
constexpr FORCE_INLINE auto move(T && x)
{
	return reinterpret_cast<typename RemoveReference<T>::Type&&>(x);
}

/**
 * @{
 * @brief Forwards l-value references as l-value
 * references and r-value references as r-value
 * references.
 * 
 * @param x value to forward
 * @return l-value or r-value depending on input
 * 	value type
 */
template<typename T>
constexpr FORCE_INLINE auto forward(typename RemoveReference<T>::Type& x)
{
	return static_cast<T&&>(x);
}

template<typename T>
constexpr FORCE_INLINE auto forward(typename RemoveReference<T>::Type&& x)
{
	return static_cast<T&&>(x);
}
/** @} */

/**
 * @brief Swap two values of the same type.
 * The type must be movable.
 * 
 * @param x,y values to swap
 */
template<typename T>
FORCE_INLINE void swap(T & x, T & y)
{
	auto z = move(x);
	x = move(y);
	y = move(z);
}
