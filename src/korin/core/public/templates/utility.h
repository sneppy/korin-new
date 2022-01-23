#pragma once

#include "core_types.h"
#include "hal/platform_memory.h"
#include "types.h"
#include "enable_if.h"


/**
 * @brief Cast any type to an r-value reference
 * of the same type.
 *
 * @param x value to cast
 * @return value cast to r-value reference
 */
template<typename T>
constexpr FORCE_INLINE typename RemoveReference<T>::Type&& move(T&& x)
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
constexpr FORCE_INLINE T&& forward(typename RemoveReference<T>::Type& x)
{
	return static_cast<T&&>(x);
}

template<typename T>
constexpr FORCE_INLINE T&& forward(typename RemoveReference<T>::Type&& x)
{
	return static_cast<T&&>(x);
}
/** @} */

#define FORWARD(x) forward<decltype(x)>(x)

/**
 * @brief Swap two values of the same type.
 * The type must be movable.
 *
 * @param x,y values to swap
 */
template<typename T>
constexpr FORCE_INLINE void swap(T& x, T& y) // TODO: This constexpr may cause trouble
{
	auto z = move(x);
	x = move(y);
	y = move(z);
}

/**
 * @brief Initialize items with the given
 * value.
 *
 * @tparam T the type of the items
 * @param dst pointer to the items
 * @param src value to initialize to
 * @param n number of items
 * @{
 */
template<typename T>
FORCE_INLINE typename EnableIf<IsTriviallyConstructible<T>::value>::Type constructItems(T* dst, T const& src, sizet n)
{
	ASSERT(dst != nullptr)
	CHECK(n != 0)

	for (sizet i = 0; i < n; ++i)
	{
		dst[i] = src;
	}
}

template<typename T>
FORCE_INLINE typename EnableIf<!IsTriviallyConstructible<T>::value>::Type constructItems(T* dst, T const& src, sizet n)
{
	ASSERT(dst != nullptr)
	CHECK(n != 0)

	for (sizet i = 0; i < n; ++i)
	{
		// Construct items
		new (dst + i) T{src};
	}
}
/** @} */

/**
 * @brief Initialize a buffer of items by
 * copying another buffer.
 *
 * The two buffers must not overlap.
 *
 * @param dst ptr to the buffer to construct
 * @param src ptr to the buffer to copy
 * @param n the number of items to construct
 * @{
 */
template<typename T>
FORCE_INLINE typename EnableIf<IsTriviallyConstructible<T>::value>::Type copyConstructItems(T* RESTRICT dst, T const* RESTRICT src, sizet n)
{
	ASSERT(dst != nullptr)
	ASSERT(src != nullptr)
	CHECK(dst != src)
	CHECK(n != 0)

	PlatformMemory::memcpy(dst, src, n * sizeof(T));
}

template<typename T>
FORCE_INLINE typename EnableIf<!IsTriviallyConstructible<T>::value>::Type copyConstructItems(T* RESTRICT dst, T const* RESTRICT src, sizet n)
{
	ASSERT(dst != nullptr)
	ASSERT(src != nullptr)
	CHECK(dst != src)
	CHECK(n != 0)

	for (sizet i = 0; i < n; ++i)
	{
		// Copy construct item
		new (dst + i) T{src[i]};
	}
}
/** @} */

/**
 * @brief Initialize a buffer of items by
 * moving items from another buffer.
 *
 * @param dst ptr to the buffer to construct
 * @param src ptr to the buffer to move
 * @param n number of items to construct
 * @{
 */
template<typename T>
FORCE_INLINE typename EnableIf<IsTriviallyConstructible<T>::value>::Type moveConstructItems(T* dst, T* src, sizet n)
{
	ASSERT(dst != nullptr)
	ASSERT(src != nullptr)
	CHECK(dst != src)
	CHECK(n != 0)

	PlatformMemory::memmove(dst, src, n * sizeof(T));
}

template<typename T>
FORCE_INLINE typename EnableIf<!IsTriviallyConstructible<T>::value>::Type moveConstructItems(T* dst, T* src, sizet n)
{
	ASSERT(dst != nullptr)
	ASSERT(src != nullptr)
	CHECK(dst != src)
	CHECK(n != 0)

	for (sizet i = 0; i < n; ++i)
	{
		// Copy construct item
		new (dst + i) T{move(src[i])};
	}
}
/** @} */

/**
 * @brief Copy items from source to destination.
 *
 * if items are trivially copyable, uses
 * memcpy, otherwise uses class-defined
 * copy assignment operator.
 *
 * It assumes that destination and source
 * are not overlapping. If they are, use
 * @c moveItems instead.
 *
 * Note that if type is not trivially
 * constructible and items are not
 * initialized you should use
 * @c copyConstructItems instead.
 *
 * @param dst pointer to destination items
 * @param src pointer to source items
 * @param n number of items to copy
 * @{
 */
template<typename T>
FORCE_INLINE typename EnableIf<IsTriviallyCopyable<T>::value>::Type copyItems(T* RESTRICT dst, T const* RESTRICT src, sizet n)
{
	ASSERT(dst != nullptr)
	ASSERT(src != nullptr)
	CHECK(dst != src)
	CHECK(n != 0)

	PlatformMemory::memcpy(dst, src, n * sizeof(T));
}

template<typename T>
FORCE_INLINE typename EnableIf<!IsTriviallyCopyable<T>::value>::Type copyItems(T* RESTRICT dst, T const* RESTRICT src, sizet n)
{
	ASSERT(dst != nullptr)
	ASSERT(src != nullptr)
	CHECK(dst != src)
	CHECK(n != 0)

	for (sizet i = 0; i < n; ++i)
	{
		dst[i] = src[i];
	}
}
/** @} */

/**
 * @brief Move items from source to destination.
 *
 * If items are trivially copyable, uses
 * memmove, otherwise uses class-defined
 * move assignment operator.
 *
 * @param dst ptr to destination items
 * @param src ptr to source items
 * @param n number of items to move
 * @{
 */
template<typename T>
FORCE_INLINE typename EnableIf<IsTriviallyCopyable<T>::value>::Type moveItems(T* dst, T* src, sizet n)
{
	ASSERT(dst != nullptr)
	ASSERT(src != nullptr)
	CHECK(dst != src)
	CHECK(n != 0)

	PlatformMemory::memmove(dst, src, n * sizeof(T));
}

template<typename T>
FORCE_INLINE typename EnableIf<!IsTriviallyCopyable<T>::value>::Type moveItems(T* dst, T* src, sizet n)
{
	ASSERT(dst != nullptr)
	ASSERT(src != nullptr)
	CHECK(dst != src)
	CHECK(n != 0)

	if (dst > src)
	{
		// Move backward
		for (sizet i = 1; i <= n; ++i)
		{
			dst[n - i] = move(src[n - i]);
		}
	}
	else
	{
		// Move forward
		for (sizet i = 0; i < n; ++i)
		{
			dst[i] = move(src[i]);
		}
	}
}

// TODO: Add no overlap case

/**
 * @brief If type has non-trivial destructor,
 * destroy all items.
 *
 * @param items ptr to items to destroy
 * @param n number of items to destroy
 * @{
 */
template<typename T>
FORCE_INLINE typename EnableIf<IsTriviallyDestructible<T>::value>::Type destroyItems(T* items, sizet n)
{
	// Nothing to do
}

template<typename T>
FORCE_INLINE typename EnableIf<!IsTriviallyDestructible<T>::value>::Type destroyItems(T* items, sizet n)
{
	ASSERT(items != nullptr)
	CHECK(n != 0)

	for (sizet i = 0; i < n; ++i)
	{
		// Destroy all items
		items[i].~T();
	}
}
/** @} */


/**
 * @brief Return the least of two values.
 *
 * @param x the first value
 * @param y the second value
 * @return x < y ? x : y
 * @{
 */
constexpr FORCE_INLINE auto min(auto const& x, auto const& y)
{
	return x < y ? x : y;
}

template<typename T>
constexpr FORCE_INLINE T& min(T& x, T& y)
{
	return x < y ? x : y;
}
/** @} */


/**
 * @brief Returns the greatest of two values.
 *
 * @tparam T the type of the values
 * @param x the first value
 * @param y the second value
 * @return x > y ? x : y
 */
constexpr FORCE_INLINE auto max(auto const& x, auto const& y)
{
	return x > y ? x : y;
}

template<typename T>
constexpr FORCE_INLINE T& max(T& x, T& y)
{
	return x > y ? x : y;
}
/** @} */
