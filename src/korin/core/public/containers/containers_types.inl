namespace Korin
{
	template<typename ...ItemsT>
	constexpr sizet len(Tuple<ItemsT...> const& tup)
	{
		return tup.getLength();
	}

	template<typename T>
	constexpr sizet len(Array<T> const& arr)
	{
		return arr.getNumItems();
	}

	template<typename T, typename PolicyT>
	constexpr sizet len(Set<T, PolicyT> const& set)
	{
		return set.getSize();
	}

	template<typename KeyT, typename ValT, typename PolicyT>
	constexpr sizet len(Map<KeyT, ValT, PolicyT> const& map)
	{
		return map.getSize();
	}

	template<typename CharT>
	constexpr sizet len(StringBase<CharT> const& str)
	{
		return str.getLength();
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

template<typename ...ItemsT>
constexpr auto iseqFor(Korin::Tuple<ItemsT...> const&)
{
	return iseqFor<ItemsT...>();
}
