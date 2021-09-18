#pragma once

#include "core_types.h"

/**
 * @brief Choose one or the other type
 * based on a truth value.
 * 
 * @tparam cond condition to evaluate
 * @tparam A,B types to choose from
 */
template<bool cond, typename A, typename B>
struct ChooseType
{
	using Type = A;
};

template<typename A, typename B> struct ChooseType<false, A, B> { using Type = B; };

/**
 * @brief Returns true if type is either
 * float32 or float64.
 * 
 * @tparam T the type to test
 */
template<typename T>
struct IsFloating
{
	enum { value = false };
};

template<> struct IsFloating<float32> { enum { value = true }; };
template<> struct IsFloating<float64> { enum { value = true }; };

/**
 * @brief Check if a type is a base for
 * another type.
 * 
 * @tparam BaseT the base type
 * @tparam DerivedT the derived type
 */
template<typename BaseT, typename DerivedT>
struct IsBaseOf
{
	enum { value = __is_base_of(BaseT, DerivedT) };
};

/**
 * @brief Check if type has trivial constructor.
 * 
 * @tparam T the type to test
 */
template<typename T>
struct IsTriviallyConstructible
{
	enum { value = __has_trivial_constructor(T) };
};

/**
 * @brief Check if a type has a trivial
 * copy assignment operator.
 * 
 * See @c __has_trivial_assign intrinsic.
 * 
 * @tparam T the type to test
 */
template<typename T>
struct IsTriviallyCopyable
{
	enum { value = __has_trivial_assign(T) };
};

/**
 * @brief Check if a type has a trivial
 * destructor.
 * 
 * See @c __has_trivial_destructor intrinsic.
 * 
 * @tparam T the type to test
 */
template<typename T>
struct IsTriviallyDestructible
{
	enum { value = __has_trivial_destructor(T) };
};

/**
 * @brief Strips the reference from a type.
 * 
 * @tparam T the type to strip the reference
 * 	from
 */
template<typename T>
struct RemoveReference
{
	using Type = T;
};

template<typename T> struct RemoveReference<T&> { using Type = T; };
template<typename T> struct RemoveReference<T&&> { using Type = T; };
