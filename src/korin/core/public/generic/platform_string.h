#pragma once

#include "core_types.h"

/**
 * @brief String abstraction layer.
 */
struct GenericPlatformString
{
#define ICMP(x, y) ((x) == (y) || ((x) - 'a' == (y) - 'A') || ((x) - 'A' == (y) - 'a'))

	/**
	 * @brief Returns the length of a
	 * null-terminated string.
	 *
	 * @tparam CharT type of the characters
	 * @param str pointer to C string
	 * @return length of the string
	 */
	template<typename CharT>
	static constexpr FORCE_INLINE sizet len(CharT* cstr)
	{
		ASSERT(cstr != nullptr)
		sizet n = 0; for (; cstr[n] != CharT{0}; ++n);
		return n;
	}

	/**
	 * @brief Returns the arithmetic difference
	 * between the first different characters
	 * in two null-terminated strings.
	 *
	 * @tparam CharT the type of the characters
	 * @param lhs,rhs string to compare
	 * @return difference of first non-equal
	 * characters
	 * @return zero if strings are equal
	 */
	template<typename CharT>
	static constexpr FORCE_INLINE CharT cmp(CharT* lhs, CharT* rhs)
	{
		ASSERT(lhs != nullptr)
		ASSERT(rhs != nullptr)
		for (; *lhs == *rhs && *lhs != CharT{0}; ++rhs, ++lhs);
		return *lhs - *rhs;
	}

	/**
	 * @brief Like @c cmp but stops after
	 * reading @c n characters.
	 *
	 * @tparam CharT the type of the characters
	 * @param lhs,rhs string to compare
	 * @param n max number of characters to read
	 * @return difference of first non-equal
	 * characters
	 * @return zero if strings are equal up to
	 * n-th character
	 */
	template<typename CharT>
	static constexpr FORCE_INLINE CharT cmpn(CharT* lhs, CharT* rhs, sizet n)
	{
		ASSERT(lhs != nullptr)
		ASSERT(rhs != nullptr)
		sizet i = 0; for (; lhs[i] == rhs[i] && i < n; ++i);
		return lhs[i] - rhs[i];
	}

	/**
	 * @brief Like @c cmp but ignore case.
	 *
	 * `icmp("KORIN", "korin") => 0`
	 *
	 * @tparam CharT the type of the characters
	 * @param lhs,rhs strings to compare
	 * @return difference of first non-equal
	 * characters
	 * @return zero if strings are equal,
	 * redargless of case
	 */
	template<typename CharT>
	static constexpr FORCE_INLINE CharT icmp(CharT* lhs, CharT* rhs)
	{
		ASSERT(lhs != nullptr)
		ASSERT(rhs != nullptr)
		for (; ICMP(*lhs, *rhs) && *lhs != CharT{0}; ++rhs, ++lhs);
		return *lhs - *rhs; // FIXME: Difference between different cases
	}

	/**
	 * @brief Like @c cmpn but ignore case.
	 *
	 *
	 * @tparam CharT the type of the characters
	 * @param lhs,rhs strings to compare
	 * @param n max number of characters to read
	 * @return difference of first non-equal
	 * characters
	 * @return zero if strings are equal up to
	 * n-th character, redargless of case
	 */
	template<typename CharT>
	static constexpr FORCE_INLINE CharT icmpn(CharT* lhs, CharT* rhs, sizet n)
	{
		ASSERT(lhs != nullptr)
		ASSERT(rhs != nullptr)
		sizet i = 0; for (; ICMP(lhs[i], rhs[i]) && i < n; ++i);
		return lhs[i] - rhs[i];
	}

#undef ICMP
};
