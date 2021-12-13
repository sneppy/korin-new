#pragma once

#include "containers_types.h"
#include "tree.h"

namespace Korin
{
	/**
	 * @brief This class implements an ordered set.
	 *
	 * The set is implemented with a RB tree.
	 *
	 * Common set operations are implemented:
	 * - set union `|`
	 * - set intersection `&`
	 * - set difference `-`
	 * - set disjoint union `^` (a.k.a. symmetric
	 *   difference)
	 *
	 * A set of Sets is partially ordered.
	 *
	 * Useful relationships:
	 * - `x ^ y <=> (x | y) - (x & y)`
	 * - `x - y <=> (x ^ y) & x`
	 *
	 * @tparam T type of the set items
	 * @tparam CompareT the compare class to use
	 */
	template<typename T, typename PolicyT>
	class Set : Tree<T, PolicyT>
	{
		using SuperT = Tree<T, PolicyT>;

	public:
		using TreeT = SuperT;
		using IteratorT = typename TreeT::IteratorT;
		using ConstIteratorT = typename TreeT::ConstIteratorT;
		using TreeT::begin;
		using TreeT::end;
		using TreeT::find;
		using TreeT::remove;

		/**
		 * @brief Returns the number of items in the set.
		 */
		FORCE_INLINE sizet getSize() const
		{
			return TreeT::getNumNodes();
		}

		/**
		 * @brief If the item identified by key exists,
		 * returns a ref to the item.
		 *
		 * If the item does not exist, the behavior is
		 * undefined.
		 *
		 * This is semantically equivalent to dereferencing
		 * the pointer returned by `find()`.
		 *
		 * @param key key that identifies the item
		 * @return ref to item
		 */
		FORCE_INLINE T& get(auto const& key)
		{
			return *TreeT::find(key);
		}

		FORCE_INLINE T const& get(auto const& key) const
		{
			return const_cast<Set&>(*this).get(key);
		}
		/** @} */

		/**
		 * @brief Returns true if an item identified by the
		 * given key exists in the set.
		 *
		 * @param key key that identifies the item
		 * @return true if item exists in the set
		 * @return false otherwise
		 */
		FORCE_INLINE bool contains(auto const& key) const
		{
			return TreeT::find(key) != TreeT::end();
		}

		/**
		 * @brief Returns true if this set and another set
		 * have the exact same items.
		 *
		 * @param other another set
		 * @return true if sets are equal
		 * @return false otherwise
		 */
		bool operator==(Set const& other) const
		{
			if (getSize() != other.getSize())
				// For sure they are not equal
				return false;

			for (auto it = begin(), endIt = end(), jt = other.begin(); it != endIt; ++it, ++jt)
			{
				if (PolicyT{}(*it, *jt) != 0)
					// Different items, sets are not equal
					return false;
			}

			// At this point all items are equal
			return true;
		}

		/**
		 * @brief Returns true if this set and another set
		 * have at least one item not in common.
		 *
		 * @param other another set
		 * @return true if sets are not equal
		 * @return false otherwise
		 */
		FORCE_INLINE bool operator!=(Set const& other) const
		{
			return !(*this == other);
		}

		/**
		 * @brief Returns true if this set is a subset of
		 * the other set (i.e. the other set contains at
		 * least all items of this set).
		 *
		 * A set is always a subset of itself.
		 *
		 * @param other another set
		 * @return true if set is a subset of the other
		 * @return false otherwise
		 */
		bool operator<=(Set const& other) const
		{
			if (getSize() > other.getSize())
				// Certainly not a subset
				return false;

			auto it = begin(), endIt = end();
			auto jt = other.begin(), endJt = other.end();
			while (it != endIt && jt != endJt)
			{
				int32 cmp = PolicyT{}(*it, *jt);
				if (cmp < 0)
				{
					// This item is not in the other set, so this is not a subset
					return false;
				}
				else if (cmp > 0)
				{
					// Sign of a possibly strict subset
					++jt;
				}
				else
				{
					// Same item, fine
					++it, ++jt;
				}
			}

			return it == endIt;
		}

		/**
		 * @brief Returns true if this set is a superset of
		 * the other set (i.e. contains at least all items
		 * of the other set).
		 *
		 * A set is always a superset of itself.
		 *
		 * @param other another set
		 * @return true if set is a superset of the other
		 * @return false otherwise
		 */
		FORCE_INLINE bool operator>=(Set const& other) const
		{
			return other <= *this;
		}

		/**
		 * @brief Returns true if set is a proper subset of
		 * the other set (i.e. set is subset and it's not
		 * equal to the other set).
		 *
		 * @param other another set
		 * @return true if set is a proper subset of other
		 * @return false otherwise
		 */
		FORCE_INLINE bool operator<(Set const& other) const
		{
			return getSize() < other.getSize() && *this <= other;
		}

		/**
		 * @brief Returns true if set is a proper superset
		 * of the other set (i.e. set is superset and it's
		 * not equal to the other set).
		 *
		 * @param other another set
		 * @return true if set is a proper superset of other
		 * @return false otherwise
		 */
		FORCE_INLINE bool operator>(Set const& other) const
		{
			return getSize() > other.getSize() && *this >= other;
		}

		/**
		 * @brief Returns true if this set and the other set
		 * are disjoint, i.e. they have no item in common.
		 *
		 * Note that `isDisjoint()` does not return the same
		 * results as `operator!=()`.
		 *
		 * @param other another set
		 * @return true if sets are disjoint
		 * @return false otherwise
		 */
		bool isDisjoint(Set const& other) const
		{
			auto it = begin(), jt = other.begin();
			for (auto endIt = end(), endJt = other.end(); it != endIt && jt != endJt;)
			{
				int32 cmp = PolicyT{}(*it, *jt);
				if (cmp < 0)
					++it;
				else if (cmp > 0)
					++jt;
				else
					// Sets have at least one common item
					return false;
			}

			// At this point we are sure there is no common item
			return true;
		}

		/**
		 * @brief Construct an item in the set.
		 *
		 * If the item already exists, the item is
		 * constructed and immediately destroyed.
		 *
		 * @param createArgs arguments passed to item
		 * constructor
		 * @return iterator pointing to created item
		 */
		FORCE_INLINE IteratorT emplace(auto&& ...createArgs)
		{
			return TreeT::findOrEmplace(FORWARD(createArgs)...);
		}

		/**
		 * @brief Inserts item in set if not already
		 * present.
		 *
		 * @param item the item to insert in the set
		 * @return iterator pointing to inserted item
		 * @{
		 */
		FORCE_INLINE IteratorT insert(T const& item)
		{
			return emplace(item);
		}

		FORCE_INLINE IteratorT insert(T&& item)
		{
			return emplace(move(item));
		}
		/** @} */

		/**
		 * @brief Insert all items in the given range.
		 *
		 * @tparam ItT the iterator type
		 * @param begin,end begin and end iterators
		 */
		template<typename ItT>
		FORCE_INLINE void update(ItT begin, ItT end)
		{
			for (; begin != end; ++begin)
			{
				// Insert all items from iterator
				insert(*begin);
			}
		}

		/**
		 * @brief Update this set by inserting all the
		 * items of another set.
		 *
		 * @param other another set
		 * @return ref to self
		 * @{
		 */
		FORCE_INLINE Set& operator|=(Set const& other)
		{
			for (auto const& item : other)
			{
				// Insert items from other set
				insert(item);
			}

			return *this;
		}

		FORCE_INLINE Set& operator|=(Set&& other)
		{
			for (auto&& item : other)
			{
				// Insert items from other set
				insert(move(item));
			}

			return *this;
		}
		/** @} */

		/**
		 * @brief Computes the union of this set with
		 * another set.
		 *
		 * @param other another set
		 * @return new set, equal to the union of the two
		 * sets.
		 * @{
		 */
		FORCE_INLINE Set operator|(Set const& other) const
		{
			return Set{*this} |= other;
		}

		FORCE_INLINE Set operator|(Set&& other) const
		{
			return Set{*this} |= move(other);
		}
		/** @} */

		/**
		 * @brief Udpate this set by removing all the items
		 * not in common with another set.
		 *
		 * @param other another set
		 * @return ref to self
		 */
		Set& operator&=(Set const& other)
		{
			auto it = begin(), jt = other.begin();
			while (it != end() && jt != other.end())
			{
				int32 cmp = PolicyT{}(*it, *jt);
				if (cmp < 0)
				{
					// Item is not in common, remove it
					it = remove(it);
				}
				else if (cmp > 0)
				{
					// Get next item to compare with
					jt++;
				}
				else
				{
					// Item in common, get next items
					++it, ++jt;
				}
			}

			while (it != end())
			{
				// Remove all remaining not-in-common items
				it = remove(it);
			}

			return *this;
		}

		/**
		 * @brief Computes the intersection between this set
		 * and another set.
		 *
		 * @param other another set
		 * @return new set, equal to the insersection
		 * of the two sets
		 * @{
		 */
		Set operator&(Set const& other) const
		{
			Set set{};
			for (auto it = begin(), jt = other.begin();
			     it != end() && jt != other.end();)
			{
				int32 cmp = PolicyT{}(*it, *jt);
				if (cmp < 0)
					++it;
				else if (cmp > 0)
					++jt;
				else
				{
					// Insert item in common
					set.insert(*it);
					++it, ++jt;
				}
			}

			return set;
		}

		Set operator&(Set&& other) const
		{
			// A bit of code duplication, but it's fine
			Set set{};
			for (auto it = begin(), jt = other.begin();
			     it != end() && jt != other.end();)
			{
				int32 cmp = PolicyT{}(*it, *jt);
				if (cmp < 0)
					++it;
				else if (cmp > 0)
					++jt;
				else
				{
					// Insert item in common
					set.insert(move(*it));
					++it, ++jt;
				}
			}

			return set;
		}
		/** @} */

		/**
		 * @brief Update this set by removing all the items
		 * in common with another set.
		 *
		 * @param other another set
		 * @return ref to self
		 */
		Set& operator-=(Set const& other)
		{
			for (auto it = begin(), jt = other.begin();
			     it != end() && jt != other.end();)
			{
				int32 cmp = PolicyT{}(*it, *jt);
				if (cmp < 0)
				{
					// Ok, don't remove and get next
					++it;
				}
				else if (cmp > 0)
				{
					// Get next item to compare with
					++jt;
				}
				else
				{
					// Item in common, remove it
					it = remove(it);
					jt++;
				}
			}

			return *this;
		}

		/**
		 * @brief Compute the difference between this
		 * set and another set.
		 *
		 * @param other another set
		 * @return new set, equal to the difference between
		 * the two sets
		 */
		Set operator-(Set const& other) const
		{
			Set set{};
			auto it = begin(), jt = other.begin();
			while (it != end() && jt != other.end())
			{
				int32 cmp = PolicyT{}(*it, *jt);
				if (cmp < 0)
				{
					// Insert item not in common
					set.insert(*it);
					++it;
				}
				else if (cmp > 0)
				{
					// Get the next item to compare
					++jt;
				}
				else
				{
					// Item in common, don't insert and get next items
					++it, ++jt;
				}
			}

			for (; it != end(); ++it)
			{
				// Insert all remaining items not in common
				set.insert(*it);
			}

			return set;
		}

		/**
		 * @brief Update this set by removing all the items
		 * in common with another set and adding those not
		 * in common.
		 *
		 * @param other another set
		 * @return ref to self
		 * @{
		 */
		Set& operator^=(Set const& other)
		{
			auto it = begin(), jt = other.begin();
			while (it != end() && jt != other.end())
			{
				int32 cmp = PolicyT{}(*it, *jt);
				if (cmp < 0)
				{
					// Item not in common, get next item to compare
					++it;
				}
				else if (cmp > 0)
				{
					// Item not in common, add to this set
					insert(*jt);
					++jt;
				}
				else
				{
					// Item in common, remove from this set
					it = remove(it);
					jt++;
				}
			}

			for (; jt != other.end(); ++jt)
			{
				// Insert remaining items of other set
				insert(*jt);
			}

			return *this;
		}


		Set& operator^=(Set&& other)
		{
			// Some code-duplication, cannot do otherwise with iterators
			auto it = begin(), jt = other.begin();
			while (it != end() && jt != other.end())
			{
				int32 cmp = PolicyT{}(*it, *jt);
				if (cmp < 0)
				{
					// Item not in common, get next item to compare
					++it;
				}
				else if (cmp > 0)
				{
					// Item not in common, add to this set
					insert(move(*jt));
					++jt;
				}
				else
				{
					// Item in common, remove from this set
					it = remove(it);
					jt++;
				}
			}

			for (; jt != other.end(); ++jt)
			{
				// Insert remaining items of other set
				insert(move(*jt));
			}

			return *this;
		}
		/** @} */

		/**
		 * @brief Compute the symmetric difference between
		 * this set and another set.
		 *
		 * In set operations, this is the equivalent of
		 * subtracting the intersection from the union.
		 *
		 * @param other another set
		 * @return new set, equal to the symmetric
		 * difference between the two sets
		 * @{
		 */
		FORCE_INLINE Set operator^(Set const& other) const
		{
			// This less efficient
			return Set{*this} ^= other;
		}

		FORCE_INLINE Set operator^(Set&&other) const
		{
			// This less efficient
			return Set{*this} ^= move(other);
		}
		/** @} */

	protected:
		using TreeT::root;
	};
} // namespace Korin
