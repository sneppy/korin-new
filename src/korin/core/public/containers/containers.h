#pragma once

namespace Containers
{
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
	ItT find(ItT begin, ItT end, auto const& value)
	{
		for (auto it = begin; it != end; ++it)
		{
			if (*it == value)
			{
				return it;
			}
		}

		return end;
	}

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
	ItT findIf(ItT begin, ItT end, PolicyT&& policy)
	{
		for (auto it = begin; it != end; ++it)
		{
			if (policy(*it))
			{
				return it;
			}
		}

		return end;
	}
} // namespace Containers

#include "list.h"
#include "tree.h"
#include "set.h"
