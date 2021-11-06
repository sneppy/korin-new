#pragma once

#include "templates/utility.h"
#include "containers_types.h"

namespace Korin
{
	/**
	 * @brief A pair of two items.
	 *
	 * @tparam T type of the first item
	 * @tparam U type of the second item
	 */
	template<typename T, typename U>
	struct Pair
	{
		using KeyT = T;
		using ValT = U;

		/* First item of the pair. */
		T first;

		/* Second item of the pair. */
		U second;

		Pair() = default;

		/**
		 * @brief Construct a new pair by copying
		 * the given values.
		 *
		 * @param inFirst value of the first item
		 * @param inSecond value of the second item
		 */
		constexpr Pair(T const& inFirst, U const& inSecond)
			: first{inFirst}
			, second{inSecond}
		{
			//
		}

		/**
		 * @brief Construct a new pair by moving
		 * or copying the given values.
		 *
		 * @param inFirst value of the first item
		 * @param inSecond value of the second item
		 */
		constexpr Pair(auto&& inFirst, auto&& inSecond)
			: first{FORWARD(inFirst)}
			, second{FORWARD(inSecond)}
		{
			//
		}

		/**
		 * @brief Return a ref to the key item
		 * (i.e. the first item).
		 * @{
		 */
		constexpr KeyT& getKey()
		{
			return first;
		}

		constexpr KeyT const& getKey() const
		{
			return const_cast<Pair&>(*this).getKey();
		}

		/**
		 * @brief Return a ref to the value item
		 * (i.e. the second item).
		 * @{
		 */
		constexpr ValT& getVal()
		{
			return second;
		}

		constexpr ValT const& getVal() const
		{
			return const_cast<Pair&>(*this).getVal();
		}
		/** @} */

		/**
		 * @brief Compare two pairs, returns true
		 * if both pairs items are equal.
		 *
		 * @param other other pair to compare to
		 * @return true if both pairs of items are
		 * equal
		 * @return false otherwise
		 */
		constexpr bool operator==(Pair const& other) const
		{
			return first == other.first && second == other.second;
		}

		/**
		 * @brief Compare two pairs, returns true
		 * if at least a pair of items is not equal
		 *
		 * @param other other pair to compare to
		 * @return true if at least one pair is
		 * different
		 * @return false otherwise
		 */
		constexpr bool operator!=(Pair const& other) const
		{
			return !(*this == other);
		}
	};

	/**
	 * @brief Utility class to compare and sort
	 * kv-pairs. Also used as a policy to find a
	 * kv-pair in a sorted collection (e.g. a
	 * binary tree).
	 *
	 * @tparam PairT the type of the kv-pair
	 * @tparam PolicyT the policy run on the keys
	 */
	template<typename PairT, typename PolicyT>
	struct FindPair : private PolicyT
	{
		using KeyT = typename PairT::KeyT;

		/**
		 * @brief Invoke as a function to compare
		 * two key-like values (at least one of the
		 * values is expected to be a kv-pair of the
		 * given type).
		 *
		 * @param lhs,rhs values to compare
		 * @return 1, 0 or -1 depending on ordering
		 * @{
		 */
		constexpr int32 operator()(PairT const& lhs, PairT const& rhs) const
		{
			return PolicyT::operator()(lhs.getKey(), rhs.getKey());
		}

		constexpr int32 operator()(PairT const& lhs, KeyT const& rhs) const
		{
			return PolicyT::operator()(lhs.getKey(), rhs);
		}

		constexpr int32 operator()(KeyT const& lhs, PairT const& rhs) const
		{
			return PolicyT::operator()(lhs, rhs.getKey());
		}
		/** @} */
	};

	// TODO
	template<typename PairT, typename HashT>
	class HashPair;
} // namespace Korin

