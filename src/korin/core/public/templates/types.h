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
