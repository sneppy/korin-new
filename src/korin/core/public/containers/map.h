#pragma once

#include "containers_types.h"
#include "pair.h"
#include "tree.h"

namespace Korin
{
	template<typename PairT>
	using MapIterator = TreeIterator<PairT>;

	template<typename PairT>
	using MapConstIterator = TreeConstIterator<PairT>;

	/**
	 * @brief A map is a collection of key-value
	 * pairs with methods to find, insert and
	 * remove pairs.
	 *
	 * Map is a sorted collection with logarithmic
	 * find, insertion and removal algorithms.
	 *
	 * Map does not allow duplicate keys.
	 *
	 * @tparam _KeyT the type of the keys
	 * @tparam _ValT the type fo the values
	 * @tparam _PolicyT the policy used to sort and
	 * find the kv-pairs
	 */
	template<typename _KeyT, typename _ValT, typename _PolicyT>
	class Map
	{
	public:
		using KeyT = _KeyT;
		using ValT = _ValT;
		using PairT = Pair<KeyT, ValT>;
		using PolicyT = FindPair<PairT, _PolicyT>;
		using TreeT = Tree<PairT, PolicyT>;
		using IteratorT = MapIterator<PairT>;
		using ConstIteratorT = MapConstIterator<PairT>;

		Map() = default;

		/**
		 * @brief Returns the number of pairs in the
		 * map.
		 */
		FORCE_INLINE sizet getSize() const
		{
			return tree.getNumNodes();
		}

		/**
		 * @brief Returns a ref to an existing value
		 * associated with the given key, or creates
		 * and return a new value associated to the
		 * given key.
		 *
		 * @param key key used to search for value
		 * @return existing or new value associated
		 * with given key
		 */
		FORCE_INLINE ValT& operator[](auto&& key)
		{
			auto it = tree.findOrEmplace(FORWARD(key));
			return it->second;
		}

		/**
		 * @brief Returns an iterator pointing to
		 * the first kv-pair of the map.
		 * @{
		 */
		FORCE_INLINE IteratorT begin()
		{
			return tree.begin();
		}

		FORCE_INLINE ConstIteratorT begin() const
		{
			return const_cast<Map&>(*this).begin();
		}
		/** @} */

		/**
		 * @brief Returns an iterator pointing to
		 * the end of the map (beyond the last
		 * kv-pair).
		 * @{
		 */
		FORCE_INLINE IteratorT end()
		{
			return tree.end();
		}

		FORCE_INLINE ConstIteratorT end() const
		{
			return const_cast<Map&>(*this).end();
		}
		/** @} */

		/**
		 * @brief Returns an iterator pointing to
		 * the last kv-pair of the map.
		 */
		FORCE_INLINE IteratorT rbegin()
		{
			return tree.rbegin();
		}

		FORCE_INLINE ConstIteratorT rbegin() const
		{
			return const_cast<Map&>(*this).rbegin();
		}
		/** @} */

		/**
		 * @brief Returns an iterator pointing before
		 * the first kv-pair of the map.
		 * @{
		 */
		FORCE_INLINE IteratorT rend()
		{
			return tree.rend();
		}

		FORCE_INLINE ConstIteratorT rend() const
		{
			return const_cast<Map&>(*this).rend();
		}
		/** @} */

		/**
		 * @brief Returns an iterator pointing to
		 * the kv-pair with the given key, or
		 * pointing to the end of the map in no such
		 * kv-pair exists.
		 *
		 * @param key key used to find the kv-pair
		 * @return iterator pointing to kv-pair or
		 * to end of the map
		 * @{
		 */
		FORCE_INLINE IteratorT find(auto const& key)
		{
			return tree.find(key);
		}

		FORCE_INLINE ConstIteratorT find(auto const& key) const
		{
			return const_cast<Map&>(*this).find(key);
		}
		/** @} */

		/**
		 * @brief Returns true if a kv-pair with the
		 * given key exists.
		 *
		 * @param key key to test
		 * @return true if matching kv-pair exists
		 * @return false otherwise
		 */
		FORCE_INLINE bool contains(auto const& key) const
		{
			return tree.find(key) != tree.end();
		}

		/**
		 * @brief Construct a new pair and insert
		 * it into the map.
		 *
		 * If a pair with the same key already
		 * exists, replaces the pair value.
		 *
		 * @param createArgs arguments used to
		 * construct the pair
		 * @return iterator pointing to the
		 * contructed pair, or to the existing
		 * pair if duplicate
		 */
		FORCE_INLINE IteratorT emplace(auto&& ...createArgs)
		{
			return tree.emplaceUnique(FORWARD(createArgs)...);
		}

		/**
		 * @brief Insert a new pair in the map.
		 *
		 * @param pair the kv-pair to insert
		 * @return iterator pointing to the inserted
		 * kv-pair
		 */
		FORCE_INLINE IteratorT insert(PairT const& pair)
		{
			return emplace(pair);
		}

		FORCE_INLINE IteratorT insert(PairT&& pair)
		{
			return emplace(move(pair));
		}
		/** @} */

		/**
		 * @brief Attempts to find a kv-pair matching
		 * the given key, and constructs a new kv-pair
		 * if it cannot find it.
		 *
		 * @param key key of the pair
		 * @param valArgs arguments used to contruct
		 * the value
		 * @return iterator pointing to existing kv-pair
		 * or to newly constructed kv-pair
		 */
		FORCE_INLINE IteratorT findOrEmplace(KeyT const& key, auto const& ...valArgs)
		{
			return tree.findOrInsert(key, [&]() {

				return PairT{key, {valArgs...}};
			});
		}

		FORCE_INLINE IteratorT findOrEmplace(auto&& key, auto&& ...valArgs)
		{
			return tree.findOrInsert(key, [&]() {

				return PairT{FORWARD(key), {FORWARD(valArgs)...}};
			});
		}
		/** @} */

		/**
		 * @brief Attempts to find a kv-pair matching
		 * the given key, and inserts a new kv-pair
		 * with the given value if it cannot find it.
		 *
		 * @param key the key of the pair
		 * @param val the value of the pair
		 * @return iterator pointing to existing kv-pair
		 * or to created kv-pair
		 */
		FORCE_INLINE IteratorT findOrInsert(KeyT const& key, ValT const& val)
		{
			return findOrEmplace(key, val);
		}

		FORCE_INLINE IteratorT findOrInsert(auto&& key, auto&& val)
		{
			return findOrEmplace(FORWARD(key), FORWARD(val));
		}
		/** @} */

		/**
		 * @brief Remove kv-pair pointed by iterator
		 * from map.
		 *
		 * @param it iterator pointing to kv-pair to
		 * remove
		 * @return iterator pointing to next valid
		 * kv-pair
		 */
		FORCE_INLINE IteratorT remove(ConstIteratorT it)
		{
			return tree.remove(it);
		}

		/**
		 * @brief Remove kv-pair identified by the
		 * given key and returns the value of the
		 * removed pair.
		 *
		 * If the pair does not exist, nothing
		 * happens.
		 *
		 * @param key key of the pair to remove
		 * @param outVal if specified, the value of
		 * the removed kv-pair is returned
		 * @return true if pair exists
		 * @return false otherwise
		 * @{
		 */
		FORCE_INLINE bool removeAt(auto const& key, ValT& outVal)
		{
			if (auto it = tree.find(key); it != tree.end())
			{
				// Write out value
				outVal = move(it->second);

				// Remove the node
				tree.remove(it);
				return true;
			}

			return false;
		}

		FORCE_INLINE bool removeAt(auto const& key)
		{
			ValT tmp;
			return removeAt(key, tmp);
		}
		/** @} */

		/**
		 * @brief Remove all kv-pair of the map.
		 */
		FORCE_INLINE void clear()
		{
			// TODO: Implement Tree::clear
			//tree.clear()
		}

	protected:
		/* The binary tree instance. */
		TreeT tree;
	};
} // namespace Korin
