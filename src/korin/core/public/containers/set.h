#pragma once

#include "containers_types.h"
#include "tree.h"

namespace Korin
{
	/**
	 * @brief Set iterators.
	 * 
	 * @tparam T type of the set items
	 * @{
	 */
	template<typename T> using SetIterator = TreeIterator<T>;
	template<typename T> using SetConstIterator = TreeConstIterator<T>;
	/** @} */

	/**
	 * @brief This class implements an ordered
	 * set.
	 * 
	 * The set is implemented with a RB tree.
	 * 
	 * @tparam T type of the set items
	 * @tparam CompareT the compare class to use
	 */
	template<typename T, typename CompareT>
	class Set
	{
		using TreeT = Tree<T, CompareT>;
		using NodeT = typename TreeT::NodeT;

	public:
		using IteratorT = SetIterator<T>;
		using ConstIteratorT = SetConstIterator<T>;

		/**
		 * @brief Returns the number of items in
		 * the set.
		 */
		FORCE_INLINE uint64 getNumItems() const
		{
			return tree.getNumNodes();
		}

		/**
		 * @brief Return iterator pointing to the first
		 * item of the set.
		 * @{
		 */
		FORCE_INLINE ConstIteratorT begin() const
		{
			return tree.begin();
		}

		FORCE_INLINE IteratorT begin()
		{
			return tree.begin();
		}
		/** @} */

		/**
		 * @brief Return iterator pointing past the last
		 * item of the set.
		 * @{
		 */
		FORCE_INLINE ConstIteratorT end() const
		{
			return tree.end();
		}

		FORCE_INLINE IteratorT end()
		{
			return tree.end();
		}
		/** @} */

		/**
		 * @brief Return iterator pointing to the last
		 * item in the set.
		 * @{
		 */
		FORCE_INLINE ConstIteratorT rbegin() const
		{
			return tree.rbegin();
		}

		FORCE_INLINE IteratorT rbegin()
		{
			return tree.rbegin();
		}
		/** @} */

		/**
		 * @brief Return iterator pointing before the
		 * first item in the set.
		 * @{
		 */
		FORCE_INLINE ConstIteratorT rend() const
		{
			return tree.rend();
		}

		FORCE_INLINE IteratorT rend()
		{
			return tree.rend();
		}
		/** @} */

		/**
		 * @brief Insert an item in the set.
		 * 
		 * @param item item to insert
		 * @return iter pointing to inserted item
		 */
		FORCE_INLINE IteratorT insert(auto&& item)
		{
			// FIXME: Replace with insertUnique
			return tree.insert(FORWARD(item));
		}

		/**
		 * @brief Remove the item pointed by the
		 * iterator.
		 * 
		 * @param it iterator pointing to item to remove
		 * @return iter pointing to next valid item
		 */
		FORCE_INLINE IteratorT remove(ConstIteratorT it)
		{
			return tree.remove(it);
		}

		/**
		 * @brief Remove the item identified by key
		 * from the set.
		 * 
		 * In this instance, a key is any object that
		 * can be compared with an item of the set,
		 * according to the compare policy.
		 * 
		 * @param key key used to identify item to
		 * remove
		 * @return iter pointing to next valid item
		 */
		FORCE_INLINE IteratorT remove(auto const& key)
		{
			return tree.remove(tree.find(key));
		}

		/**
		 * @brief Return an iterator pointing to the
		 * item identified by the given key.
		 * 
		 * @param key key used to identify the item
		 * @return iter to item, iter to end otherwise
		 */
		FORCE_INLINE ConstIteratorT get(auto const& key) const
		{
			return tree.find(key);
		}

		FORCE_INLINE IteratorT get(auto const& key)
		{
			return tree.find(key);
		}
		/** @} */

		/**
		 * @brief Returns true if item identified by
		 * key is in set.
		 * 
		 * @param key value used to identify the item
		 * @return true if item is in set
		 * @return false otherwise
		 */
		FORCE_INLINE bool has(auto const& key) const
		{
			return tree.find(key) != tree.end();
		}

		/**
		 * @brief Compute the union of this set with
		 * another set.
		 * 
		 * @param other another set
		 * @return ref to self
		 */
		Set& operator|=(Set const& other)
		{
			//it = first item in this
			//for other item in other:
			//	find next item in this such that item >= other item
			//	if item != other item:
			//		add other item to this
			auto it = tree.begin();
			auto jt = other.tree.begin();

			for (; jt != other.tree.end(); jt++)
			{
				// Find next item in this set greater or equal
				// to the current item
				it = findIf(it, tree.end(), [jt](auto const& item) {

					return CompareT{}(item, *jt) >= 0;
				});

				if (it == tree.end())
				{
					// All remaining items are not in this set
					break;
				}

				if (CompareT{}(*it, *jt) != 0)
				{
					// Add new item to set
					it = tree.insert(*jt);
				}
			}

			// Add remaining items
			for (; jt != other.tree.end(); jt++)
			{
				tree.insert(*jt);
			}

			return *this;
		}

		/**
		 * @brief Compute the intersection of this
		 * set with another set.
		 * 
		 * @param other another set
		 * @return ref to self
		 */
		Set& operator&=(Set const& other)
		{
			auto it = tree.begin();
			auto jt = other.tree.begin();

			for (; jt != other.tree.end(); ++jt)
			{
				while (it != tree.end())
				{
					int32 cmp = CompareT{}(*it, *jt);
					
					if (cmp < 0)
					{
						it = tree.remove(it);
					}
					else
					{
						if (cmp == 0)
						{
							it++;
						}

						break;
					}
				}
			}

			// All remaining items should be removed
			for (; it != tree.end(); it = tree.remove(it));

			return *this;
		}

		/**
		 * @brief Compute the difference between
		 * this set and another set.
		 * 
		 * @param other another set
		 * @return ref to self
		 */
		Set& operator^=(Set const& other)
		{
			auto it = tree.begin();
			auto jt = other.tree.begin();

			for (; jt != other.tree.end(); ++jt)
			{
				// Find next item in set
				it = findIf(it, tree.end(), [jt](auto const& item) {

					return CompareT{}(item, *jt) >= 0;
				});
				
				if (it == tree.end())
				{
					break;
				}

				if (CompareT{}(*it, *jt) == 0)
				{
					it = tree.remove(it);
				}
			}

			return *this;
		}

		/**
		 * @brief Return a new set equal to the
		 * union of two sets.
		 * 
		 * @param other another set
		 * @return union of two sets
		 */
		FORCE_INLINE Set operator|=(Set const& other) const
		{
			return Set{*this} |= other;
		}

		/**
		 * @brief Return a new set equal to the
		 * intersection of two sets.
		 * 
		 * @param other another set
		 * @return intersection of two sets
		 */
		FORCE_INLINE Set operator&=(Set const& other) const
		{
			return Set{*this} &= other;
		}

		/**
		 * @brief Return a new set equal to the
		 * difference of two sets.
		 * 
		 * @param other another set
		 * @return difference of two sets
		 */
		FORCE_INLINE Set operator^=(Set const& other) const
		{
			return Set{*this} ^= other;
		}

	protected:
		/* The RB tree instance. */
		TreeT tree;
	};
} // namespace Korin
