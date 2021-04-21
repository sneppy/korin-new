#pragma once

#include "list.h"

namespace Containers
{
	/**
	 * @brief Return the size of the container.
	 */
	template<typename T>
	FORCE_INLINE sizet getLength(List<T> const& list)
	{
		return list.getNumNodes();
	}
};
