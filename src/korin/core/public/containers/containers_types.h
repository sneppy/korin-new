#pragma once

#include "core_types.h"
#include "templates/ordering.h"

namespace Korin
{
	template<typename> struct Pair;
	template<typename> struct Tuple;
	template<typename> struct Optional;

	template<typename>							class Stack;
	template<typename>							class Queue;
	template<typename>							class List;
	template<typename>							class Array;
	template<typename, typename = GreaterThan>	class Tree;
	template<typename, typename, typename>		class Map;
	template<typename, typename = GreaterThan>	class Set;
	template<typename>							class HashMap;
	template<typename>							class HashSet;

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

#include "containers_types.inl"
