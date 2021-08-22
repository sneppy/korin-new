namespace Containers
{
	template<typename T, typename CompareT>
	constexpr FORCE_INLINE sizet len(Set<T, CompareT> const& container)
	{
		return container.getNumItems();
	}

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