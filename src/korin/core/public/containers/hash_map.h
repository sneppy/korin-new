#pragma once

#include "containers_types.h"
#include "pair.h"
#include "hash_table.h"

namespace Korin
{
	/**
	 * @brief This class implements an associative
	 * container with nearly constant-time read and
	 * write operations, using an hash table.
	 *
	 * All keys within the map are unique.
	 *
	 * The hash map type comprises a key type and
	 * a value type.
	 *
	 * The key type must be comparable and hashable,
	 * either by defining the class method @c toHashKey,
	 * or by providing a custom hash policy as a third
	 * template parameter, or by specializing the
	 * @c ChooseHashPolicy struct. A default hash policy
	 * is provided for common types and strings.
	 *
	 * The elements of an hash map are not sorted, user
	 * code should never assume a particular ordering.
	 *
	 * @tparam _KeyT the type of the map keys
	 * @tparam _ValT the type of the values
	 * @tparam _HashPolicyT the policy used to hash the
	 * keys
	 */
	template<typename _KeyT, typename _ValT, typename _HashPolicyT>
	class HashMap : public HashTable<Pair<_KeyT, _ValT>, HashPair<Pair<_KeyT, _ValT>, _HashPolicyT>>
	{
	public:
		using KeyT = _KeyT;
		using ValT = _ValT;
		using PairT = Pair<KeyT, ValT>;
		using HashPolicyT = HashPair<PairT, _HashPolicyT>;
		using SuperT = HashTable<PairT, HashPolicyT>;
		using BucketT = HashBucket<PairT, HashPolicyT>;
		using IteratorT = typename SuperT::IteratorT;
		using ConstIteratorT = typename SuperT::ConstIteratorT;

		using SuperT::SuperT;
		using SuperT::getSize;
		using SuperT::getNumBuckets;
		using SuperT::begin;
		using SuperT::end;
		using SuperT::find;
		using SuperT::remove;
		using SuperT::clear;

		/**
		 * @brief This operator accepts a key and
		 * attempts to locate and return a ref to
		 * the value associated with that key.
		 *
		 * If the key is not found in the map, a
		 * new key-value pair is created with a
		 * default value, and a rw-ref to it is
		 * returned.
		 *
		 * @param key the key used to locate the
		 * value in the map
		 * @return the existing value corresponding
		 * to the key or a new default value
		 */
		ValT& operator[](auto const& key); // TODO
		/**
		 * @brief Construct a new pair in place and inserts
		 * it in the map if no duplicate key exists.
		 *
		 * If a duplicate key exists, the pair is not
		 * destroyed and not inserted instead.
		 *
		 * @param createArgs arguments used to create the
		 * pair
		 * @return an iterator pointing to the created pair,
		 * or to the existing pair if duplicate
		 */
		FORCE_INLINE IteratorT emplace(auto&& ...createArgs)
		{
			return SuperT::emplaceUnique(FORWARD(createArgs)...);
		}

		/**
		 * @brief Insert a new key-value pair in the map.
		 *
		 * If a pair with the same key already exists,
		 * the pair is not inserted and an iterator
		 * pointing to the existing value is returned
		 * instead.
		 *
		 * @param pair pair to insert
		 * @return iterator pointing to the inserted pair,
		 * or to the existing pair if duplicate
		 * @{
		 */
		FORCE_INLINE IteratorT insert(PairT const& pair)
		{
			return SuperT::insertUnique(pair);
		}

		FORCE_INLINE IteratorT insert(PairT&& pair)
		{
			return SuperT::insertUnique(move(pair));
		}
		/** @} */

		/**
		 * @brief Returns true if the map contains
		 * a pair with the given key
		 *
		 * @param key key to test
		 * @return true if pair with given key exists
		 * @return false otherwise
		 */
		FORCE_INLINE bool contains(auto const& key) const
		{
			return find(key) != end();
		}

		/**
		 * @brief Finds and removes the pair matching
		 * the given key. If such a pair does not exists
		 * in the map, the method simply returns.
		 *
		 * If specified, moves the value of the removed
		 * pair in the given output value.
		 *
		 * @param key the key used to locate the pair
		 * @param outVal returns the value of the removed
		 * pair, if any
		 * @return true if pair was found and removed
		 * @return false otherwise
		 * @{
		 */
		FORCE_INLINE bool removeAt(auto const& key, ValT& outVal)
		{
			auto it = find(key);
			if (it != end())
			{
				// Move out value
				outVal = move(it->second);
				remove(it);
				return true;
			}

			return false;
		}

		FORCE_INLINE bool removeAt(auto const& key)
		{
			auto it = find(key);
			if (it != end())
			{
				remove(it);
				return true;
			}

			return false;
		}
		/** @} */

	protected:
		using SuperT::buckets;
		using SuperT::numBuckets;
		using SuperT::numItems;
	};
} // namespace Korin
