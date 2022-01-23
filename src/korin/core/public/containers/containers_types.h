#pragma once

#include "core_types.h"
#include "templates/ordering.h"
#include "templates/sequence.h"

namespace Korin
{
	template<typename>           struct ChoosePolicy;
	template<typename, typename> struct Pair;
	template<typename>           struct Optional;

	template<typename...>                  class Tuple;
	template<typename>                     class Stack;
	template<typename>                     class Queue;
	template<typename>                     class List;
	template<typename>                     class ArrayBase;
	template<typename>                     class Array;
	template<typename, typename>           class Tree;
	template<typename, typename>           class Set;
	template<typename, typename, typename> class Map;
	template<typename, typename, typename> class HashMap;
	template<typename, typename>           class HashSet;
	template<typename>                     class StringBase;

	/**
	 * @brief String type with 8-bit wide characters.
	 */
	using String = StringBase<ansichar>;
} // namespace Korin

#include "hash_types.h"

namespace Korin
{
	/**
	 * @brief Choose an appropriate default policy
	 * for the given type.
	 *
	 * @tparam T the type to choose policy for
	 */
	template<typename T>
	struct ChoosePolicy
	{
		// By default, use user-defined comparison operators
		using Type = GreaterThan;
	};

	template<typename T, typename PolicyT = typename ChoosePolicy<T>::Type>                      class Tree;
	template<typename T, typename PolicyT = typename ChoosePolicy<T>::Type>                      class Set;
	template<typename KeyT, typename ValT, typename PolicyT = typename ChoosePolicy<KeyT>::Type> class Map;

	/**
	 * @brief Helper to choose an appropriate hash
	 * policy for the given type.
	 *
	 * @tparam T the type for which to choose the
	 * policy
	 */
	template<typename T>
	struct ChooseHashPolicy
	{
		using Type = DefaultHashPolicy;
	};

	template<typename T, typename HashPolicyT = typename ChooseHashPolicy<T>::Type>                      class HashSet;
	template<typename KeyT, typename ValT, typename HashPolicyT = typename ChooseHashPolicy<KeyT>::Type> class HashMap;

	/**
	 * @brief Returns true if the type is a tuple.
	 *
	 * @tparam T the type to test
	 * @{
	 */
	template<typename T>
	struct IsTuple
	{
		enum { value = false };
	};

	template<typename ...ItemsT> struct IsTuple<Tuple<ItemsT...>> { enum { value = true }; };
	/** @} */

	/**
	 * @brief Returns the size of a given container.
	 *
	 * @param container a container instance.
	 */
	template<typename ContainerT>
	constexpr sizet len(ContainerT const& container);

	/**
	 * @brief Find the first item that matches
	 * the given value.
	 *
	 * The container items must be comparable
	 * with the value by means of the comparison
	 * operators.
	 *
	 * @tparam ItT the type of the iterator
	 * @param begin iter pointing to the beginning
	 * of the iterator
	 * @param end iter pointing to the end of the
	 * iterator
	 * @param value the value to match
	 * @return iterator pointing to the first item
	 * that matches the value, iterator pointing to
	 * the end of the container otherwise
	 */
	template<typename ItT>
	constexpr ItT find(ItT begin, ItT end, auto const& value);

	/**
	 * @brief Find the first item that matches
	 * the given policy.
	 *
	 * @tparam ItT the type of the iterator
	 * @tparam PolicyT the type of policy
	 * @param begin iter pointing to the first item of
	 * the container
	 * @param end iter pointing to the end of the
	 * container
	 * @param policy a callback that receives the item
	 * and returns true if it matches the criteria
	 * @return iter pointing to the first item that
	 * matches the criteria, iter pointing to the end
	 * of the container otherwise
	 */
	template<typename ItT, typename PolicyT>
	constexpr ItT findIf(ItT begin, ItT end, PolicyT&& policy);
} // namespace Korin

// TODO: Definition for iseqFor does not live
// under a namespace for the moment
/**
 * @brief Returns an index sequence to iterate
 * over the given tuple
 *
 * @tparam ItemsT the types of the items
 * @return index sequence to iterate over the
 * tuple
 */
template<typename ...ItemsT>
constexpr auto iseqFor(Korin::Tuple<ItemsT...> const&);

#include "containers_types.inl"
