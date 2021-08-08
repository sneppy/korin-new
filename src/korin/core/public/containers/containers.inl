namespace Containers
{
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
}