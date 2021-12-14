#pragma once

#include "core_types.h"

/**
 * @brief Helper class used to pass
 * a sequence of integers around at
 * compile-time.
 * 
 * Mainly used to unpack tuple items.
 * 
 * @tparam IntT type of the integers
 * @tparam seq sequence of integers
 */
template<typename IntT, IntT ...seq>
struct IntegerSequence
{
	/* Length of the sequence. */
	static constexpr sizet len = sizeof...(seq);
};

/**
 * @brief Partial specialization with
 * 64-bit integer type for indices.
 * 
 * @tparam idxs sequence of indices
 */
template<sizet ...idxs>
using IndexSequence = IntegerSequence<sizet, idxs...>;

namespace Sequence_Private
{
	/**
	 * @brief Implementation for @c srange.
	 */
	template<typename IntT, IntT to, IntT from, IntT ...seq>
	constexpr auto seq_Impl()
	{
		if constexpr (from == to)
		{
			return IntegerSequence<IntT, seq...>{};
		}
		else
		{
			return seq_Impl<IntT, to, from + 1, seq..., from>();
		}
	}
} // namespace Sequence_Private

/**
 * @brief Returns an integer sequence of
 * the given type, in the given range.
 * 
 * @tparam IntT type of the integers
 * @tparam to end of range (not included)
 * @tparam from starting number
 * @return new integer sequence
 */
template<typename IntT, IntT to, IntT from = 0>
constexpr auto seq()
{
	return Sequence_Private::seq_Impl<IntT, to, from>();
}

/**
 * @brief Like @c range but returns an
 * index sequence.
 * 
 * @tparam to end of range
 * @tparam from starting index
 * @return new index sequence
 */
template<sizet to, sizet from = 0>
constexpr auto iseq()
{
	return seq<sizet, to, from>();
}

/**
 * @brief Returns an index sequence to
 * iterate on the given parameter pack.
 * 
 * @tparam ArgsT parameter pack
 * @return new index sequence
 */
template<typename ...ArgsT>
constexpr auto iseqFor()
{
	return iseq<sizeof...(ArgsT)>();
}
