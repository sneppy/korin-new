#pragma once

#include "core_types.h"

/**
 * @brief Check if two types are the same
 * type.
 *
 * @param T,U types to test
 */
template<typename T, typename U>
struct SameType
{
	enum { value = false };
};

template<typename T> struct SameType<T, T> { enum { value = true}; };

/**
 * @brief Check if type is an integral type.
 *
 * @tparam T type to test
 */
template<typename T>
struct IsIntegral
{
	enum { value = false };
};

template<> struct IsIntegral<uint8>  { enum { value = true }; };
template<> struct IsIntegral<uint16> { enum { value = true }; };
template<> struct IsIntegral<uint32> { enum { value = true }; };
template<> struct IsIntegral<uint64> { enum { value = true }; };
template<> struct IsIntegral<int8>   { enum { value = true }; };
template<> struct IsIntegral<int16>  { enum { value = true }; };
template<> struct IsIntegral<int32>  { enum { value = true }; };
template<> struct IsIntegral<int64>  { enum { value = true }; };
template<> struct IsIntegral<char>   { enum { value = true }; };

/**
 * @brief Return true if type is a cv POD type.
 *
 * @tparam T the type to test
 */
template<typename T>
struct IsPOD
{
	enum { value = __is_pod(T) };
};

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

/**
 * @brief Remove the const and volatile
 * qualifier from a type.
 *
 * @tparam T type to modify
 */
template<typename T>
struct RemoveCV
{
	using Type = T;
};

template<typename T> struct RemoveCV<T const>          { using Type = T; };
template<typename T> struct RemoveCV<T volatile>       { using Type = T; };
template<typename T> struct RemoveCV<T const volatile> { using Type = T; };

/**
 * @brief Strip type from references and
 * qualifiers.
 *
 * @tparam T type to decay
 */
template<typename T>
class Decay
{
	using RemoveRefT = typename RemoveReference<T>::Type;

public:
	using Type = typename RemoveCV<RemoveRefT>::Type;
	// TODO: See std::__decay for arrays and functions
};
