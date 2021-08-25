#pragma once

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
