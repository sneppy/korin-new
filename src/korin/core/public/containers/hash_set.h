#pragma once

#include "containers_types.h"
#include "hash_table.h"

namespace Korin
{
	/**
	 * @brief This class implements an unordered
	 * set type that uses an hash table.
	 *
	 * All items of the set are unique.
	 *
	 * The type of the items must be comparable
	 * and hashable, either by defining the method
	 * @c toHashKey in the class or by providing
	 * a custom hash policy in the template
	 * parameters or by specilazing the struct
	 * @c ChooseHashPolicy for the given type.
	 *
	 * The HashSet class defines operators for
	 * computing the union, intersection and
	 * difference of two sets.
	 *
	 * Partial ordering is defined between subsets
	 * and supersets.
	 *
	 * In the documentation, the term "hash key"
	 * refers to the computed hash key, whereas
	 * the term key is used is used to refer to
	 * the the properties that uniquely identify
	 * an item in the set.
	 *
	 * @tparam T the type of the items
	 * @tparam HashPolicyT the policy used to
	 * compute hash keys
	 */
	template<typename T, typename HashPolicyT>
	class HashSet : public HashTable<T, HashPolicyT>
	{
		using SuperT = HashTable<T, HashPolicyT>;

	public:
		using typename SuperT::IteratorT;
		using typename SuperT::ConstIteratorT;

		using SuperT::SuperT;
		using SuperT::getSize;
		using SuperT::begin;
		using SuperT::end;
		using SuperT::find;
		using SuperT::remove;
		using SuperT::clear;

		/**
		 * @brief Returns true if set contains an
		 * item that matches the given key.
		 *
		 * @param key the key to test
		 * @return true if item with given key exists
		 * @return false otherwise
		 */
		FORCE_INLINE bool contains(auto const& key) const
		{
			return find(key) != end();
		}

		/**
		 * @brief Return true if set is subset or equal
		 * to another set.
		 *
		 * @param other another set
		 * @return true if subset or equal
		 * @return false otherwise
		 */
		bool operator<=(HashSet const& other) const
		{
			for (auto const& item : *this)
			{
				if (!other.contains(item))
				{
					// Extra item found, not a subset
					return false;
				}
			}

			return true;
		}

		/**
		 * @brief Returns true if set is subset of
		 * another set.
		 *
		 * @param other another hash set
		 * @return true if subset
		 * @return false otherwise
		 */
		FORCE_INLINE bool operator<(HashSet const& other) const
		{
			return (*this <= other) && other.getSize() != getSize();
		}

		/**
		 * @brief Returns true if set is superset or
		 * equal to another set.
		 *
		 * @param other another hash set
		 * @return true if superset or equal
		 * @return false otherwise
		 */
		bool operator>=(HashSet const& other) const
		{
			return other <= *this;
		}

		/**
		 * @brief Returns true if this set is a
		 * superset of another set.
		 *
		 * @param other another hash set
		 * @return true if superset
		 * @return false otherwise
		 */
		FORCE_INLINE bool operator>(HashSet const& other) const
		{
			return other < *this;
		}

		/**
		 * @brief Returns true if this set and another
		 * set contain the same items.
		 *
		 * @param other another hash set
		 * @return true if sets are equal
		 * @return false otherwise
		 */
		FORCE_INLINE bool operator==(HashSet const& other) const
		{
			return (getSize() == other.getSize()) && *this <= other;
		}

		/**
		 * @brief Returns true if two sets are not
		 * equal.
		 *
		 * @param other another hash set
		 * @return true if sets are not equal
		 * @return false otherwise
		 */
		FORCE_INLINE bool operator!=(HashSet const& other) const
		{
			return !(*this == other);
		}

		/**
		 * @brief Returns true if two sets are disjoint.
		 *
		 * @param other another hash set
		 * @return true if sets have no item in
		 * common
		 * @return false otherwise
		 */
		bool isDisjoint(HashSet const& other) const
		{
			for (auto const& item : other)
			{
				if (contains(item))
				{
					// Common item found
					return false;
				}
			}

			return true;
		}

		/**
		 * @brief Construct a new item and insert
		 * it into the set.
		 *
		 * If an item with the same key already
		 * exists, the item is not inserted. The
		 * item is created and destroyed in this
		 * case.
		 *
		 * @param createArgs argument used to
		 * construct the item
		 * @return iterator pointing to created
		 * item, or to existing item if duplciate
		 */
		FORCE_INLINE IteratorT emplace(auto&& ...createArgs)
		{
			return SuperT::findOrEmplace(FORWARD(createArgs)...);
		}

		/**
		 * @brief Add an item into the set.
		 *
		 * If an item with the same key already
		 * exists, the item is not inserted.
		 *
		 * @param item the item to insert
		 * @return iterator pointing to inserted
		 * item, or to existing item if duplicate
		 * @{
		 */
		FORCE_INLINE IteratorT insert(T const& item)
		{
			return SuperT::findOrInsert(item);
		}

		FORCE_INLINE IteratorT insert(T&& item)
		{
			return SuperT::findOrInsert(move(item));
		}
		/** @} */

		/**
		 * @brief Removes the item identified by
		 * the key.
		 *
		 * If no such item exists, the method
		 * does nothing and returns false.
		 *
		 * If provided, the removed item is moved
		 * out before removal.
		 *
		 * @param key the key used to identify
		 * the item to remove
		 * @param outItem returns the removed item
		 * @return true if item was found and
		 * removed
		 * @return false otherwise
		 * @{
		 */
		FORCE_INLINE bool removeAt(auto const& key, T& outItem)
		{
			IteratorT it = find(key);
			if (it != end())
			{
				// Move out value
				outItem = move(*it);
				remove(it);
				return true;
			}

			return false;
		}

		FORCE_INLINE bool removeAt(auto const& key)
		{
			IteratorT it = find(key);
			if (it != end())
			{
				remove(it);
				return true;
			}

			return false;
		}
		/** @} */

		/**
		 * @brief Update this set by inserting all the
		 * items of another set.
		 *
		 * @param other another set
		 * @return ref to self
		 * @{
		 */
		FORCE_INLINE HashSet& operator|=(HashSet const& other)
		{
			for (auto const& it : other)
			{
				// Insert all items of other if not already present
				insert(it);
			}

			return *this;
		}

		FORCE_INLINE HashSet& operator|=(HashSet&& other)
		{
			for (auto& it : other)
			{
				// Insert all items of other if not already present
				insert(move(it));
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
		FORCE_INLINE HashSet operator|(HashSet const& other) const
		{
			return HashSet{*this} |= other;
		}

		FORCE_INLINE HashSet operator|(HashSet&& other) const
		{
			return HashSet{*this} |= move(other);
		}
		/** @} */

		/**
		 * @brief Udpate this set by removing all the items
		 * not in common with another set.
		 *
		 * @param other another set
		 * @return ref to self
		 */
		FORCE_INLINE HashSet& operator&=(HashSet const& other)
		{
			for (auto it = begin(); it != end();)
			{
				if (!other.contains(*it))
				{
					// Remove element if not in other
					it = remove(it);
				}
				else ++it;
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
		FORCE_INLINE HashSet operator&(HashSet const& other) const
		{
			HashSet set{};
			for (auto const& item : other)
			{
				if (contains(item))
				{
					// Insert items in common
					set.insert(item);
				}
			}

			return set;
		}

		FORCE_INLINE HashSet operator&(HashSet&& other) const
		{
			HashSet set{};
			for (auto& item : other)
			{
				if (contains(item))
				{
					// Insert items in common
					set.insert(move(item));
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
		FORCE_INLINE HashSet& operator-=(HashSet const& other)
		{
			for (auto&& item : other)
			{
				// Remove item in common
				removeAt(item);
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
		FORCE_INLINE HashSet operator-(HashSet const& other) const
		{
			HashSet set{};
			for (auto const& item : *this)
			{
				if (!other.contains(item))
				{
					set.insert(item);
				}
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
		HashSet& operator^=(HashSet const& other)
		{
			for (auto const& it : other)
			{
				if (!removeAt(it))
				{
					// If not in common, add item
					insert(it);
				}
			}

			return *this;
		}

		HashSet& operator^=(HashSet&& other)
		{
			for (auto& it : other)
			{
				if (!removeAt(it))
				{
					// If not in common, add item
					insert(move(it));
				}
			}
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
		FORCE_INLINE HashSet operator^(HashSet const& other) const
		{
			HashSet set{};
			for (auto const& item : *this)
			{
				if (!other.contains(item))
				{
					// Insert item from this set that are not in other set
					set.insert(item);
				}
			}
			for (auto const& item : other)
			{
				if (!contains(item))
				{
					// Insert item from the other set that are not in this set
					set.insert(item);
				}
			}

			return set;
		}

		FORCE_INLINE HashSet operator^(HashSet&& other) const
		{
			// A bit of a long repetition here, but it's fine
			HashSet set{};
			for (auto const& item : *this)
			{
				if (!other.contains(item))
				{
					// Insert item from this set that are not in other set
					set.insert(item);
				}
			}
			for (auto& item : other)
			{
				if (!contains(item))
				{
					// Insert item from the other set that are not in this set
					set.insert(move(item));
				}
			}

			return set;
		}
		/** @} */

	protected:
		using SuperT::buckets;
		using SuperT::numBuckets;
		using SuperT::numItems;
	};
} // namespace Korin
