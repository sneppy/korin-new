namespace Korin
{
	/**
	 * @brief Return the length of the container.
	 * 
	 * @tparam T the type of the container
	 * @param param0 the container instance 
	 * @return number of items in the container
	 * @{
	 */
	template<typename T>
	constexpr FORCE_INLINE sizet len(Array<T> const& arr)
	{
		return arr.getNumItems();
	}

	template<typename T, typename CompareT>
	constexpr FORCE_INLINE sizet len(Set<T, CompareT> const& set)
	{
		return set.getNumItems();
	}

	template<typename CharT>
	constexpr FORCE_INLINE sizet len(StringBase<CharT> const& str)
	{
		return str.getLen();
	}
	/** @} */

	/**
	 * @brief Find the first occurance of a value
	 * between two iterators.
	 * 
	 * @tparam ItT a fwd iterator type
	 * @param begin iterator from where to start
	 * @param end iterator that points to the end
	 * @param value value to find
	 * @return iterator pointing to first occurance
	 * or to end if no occurance was found
	 */
	template<typename ItT>
	constexpr ItT find(ItT begin, ItT end, auto const& value)
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
	 * @brief Like find but returns the first value
	 * that satisfies the given policy.
	 * 
	 * @tparam ItT a fwd iterator type
	 * @tparam PolicyT the type of the policy
	 * @param begin iter that points to the start
	 * @param end iter that points to the end
	 * @param policy policy used to evaluate items
	 * @return iterator thet points to the first
	 * occurance or to the end if none found
	 */
	template<typename ItT, typename PolicyT>
	constexpr ItT findIf(ItT begin, ItT end, PolicyT&& policy)
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
}