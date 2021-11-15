#pragma once

#include "containers_types.h"
#include "list_node.h"

namespace Korin
{
	template<typename, typename> class HashTable;
	template<typename, typename> class HashTableIterator;
	template<typename, typename> class HashTableConstIterator;

	namespace HashTable_Impl
	{
		/* The type of the hash table nodes. */
		template<typename T, typename HashPolicyT>
		using HashTableNode = ForwardListNodeBase<HashBucket<T, HashPolicyT>>;
	} // namespace HashTable_Impl

	/**
	 * @brief Implementation of the hash table
	 * iterator, used to iterate the elements
	 * of an hash table.
	 *
	 * @tparam T the type of the table items
	 * @tparam HashPolicyT the hash policy used
	 * to hash the keys of the table
	 */
	template<typename T, typename HashPolicyT>
	class HashTableIterator
		: public ForwardListIteratorBase<HashTable_Impl::HashTableNode<T, HashPolicyT>>
	{
		friend HashTable<T, HashPolicyT>;

		using SuperT = ForwardListIteratorBase<HashTable_Impl::HashTableNode<T, HashPolicyT>>;
		using SuperT::curr;
		using SuperT::SuperT;

	public:
		using RefT = T&;
		using PtrT = T*;

		FORCE_INLINE RefT operator*() const
		{
			return curr->value;
		}

		FORCE_INLINE PtrT operator->() const
		{
			return &(**this);
		}
	};

	/**
	 * @brief Constant iterator for hash table.
	 * @see HashTableIterator
	 *
	 * @tparam T the type of the table items
	 * @tparam HashPolicyT the hash policy used
	 * to hash the keys of the table
	 */
	template<typename T, typename HashPolicyT>
	class HashTableConstIterator
		: public ForwardListConstIteratorBase<HashTable_Impl::HashTableNode<T, HashPolicyT>>
	{
		friend HashTable<T, HashPolicyT>;

		using SuperT = ForwardListConstIteratorBase<HashTable_Impl::HashTableNode<T, HashPolicyT>>;
		using SuperT::curr;
		using SuperT::SuperT;

	public:
		using RefT = T const&;
		using PtrT = T const*;

		FORCE_INLINE RefT operator*() const
		{
			return curr->value;
		}

		FORCE_INLINE PtrT operator->() const
		{
			return &(**this);
		}
	};

	/**
	 * @brief An associative container with
	 * constant read and write operations.
	 *
	 * Used to implement the likes of hash
	 * maps and unordered sets.
	 *
	 * The item must be hashable and comparable.
	 * @see HashMap for more details
	 *
	 * @tparam T the type of the items
	 * @tparam HashPolicyT the policy used to
	 * hash the keys
	 */
	template<typename T, typename HashPolicyT>
	class HashTable
	{
	protected:
		using BucketT = HashTable_Impl::HashTableNode<T, HashPolicyT>;
		using IteratorT = HashTableIterator<T, HashPolicyT>;
		using ConstIteratorT = HashTableConstIterator<T, HashPolicyT>;

		/* List of buckets. */
		BucketT** buckets;

		/* Current number of buckets, always a power of two. */
		sizet numBuckets;

		/* Number of items in the table. */
		sizet numItems;

		/* Statically allocated node, used as a placeholder. */
		/* See pushToBucket for more details. */
		union { ubyte _[sizeof(BucketT)]; BucketT placeholder; };
		// Trick to prevent construction

		/**
		 * @brief Construct an empty hash table.
		 */
		FORCE_INLINE HashTable()
			: buckets{}
			, numBuckets{}
			, numItems{}
			, _{}
		{
			// Allocate buckets
			reallocBuckets(HASH_BUCKET_INITIAL_COUNT);
		}

		/**
		 * @brief Construct a table by copying all
		 * the items of another table.
		 *
		 * @param other another hast table
		 */
		FORCE_INLINE HashTable(HashTable const& other)
			: buckets{}
			, numBuckets{}
			, numItems{}
			, _{}
		{
			// Allocate buckets and copy all items
			reallocBuckets(other.numBuckets);
			copyNodes(other.placeholder.next);
			numItems = other.numItems;
		}

		/**
		 * @brief Construct a new hash table, by
		 * taking all the items from another table.
		 *
		 * The other table is destroyed in the
		 * process.
		 *
		 * @param other another hash table
		 */
		FORCE_INLINE HashTable(HashTable&& other)
			: buckets{other.buckets}
			, numBuckets{other.numBuckets}
			, numItems{other.numItems}
			, _{}
		{
			placeholder.next = other.placeholder.next;
			if (placeholder.next)
			{
				// Update bucket pointing to placeholder
				sizet const bucketIdx = getBucketIdx(placeholder.next);
				buckets[bucketIdx] = &placeholder;
			}

			other.buckets = nullptr;
			other.numBuckets = 0;
			other.numItems = 0;
			other.placeholder.next = nullptr;
		}

		/**
		 * @brief Reset table and copy all the items
		 * from antoher hash table.
		 *
		 * @param other another hash table
		 */
		FORCE_INLINE HashTable& operator=(HashTable const& other)
		{
			// TODO: Simple, maybe not efficient
			// Remove all items
			clear();

			// Allocate required buckets and copy items
			reallocBuckets(other.numBuckets);
			copyNodes(other.placeholder.next);
			numItems = other.numItems;

			return *this;
		}

		/**
		 * @brief Reset table and take all the items
		 * of another hash table.
		 *
		 * The other hash table is destroyed in doing
		 * so.
		 *
		 * @param other another hash table
		 */
		FORCE_INLINE HashTable& operator=(HashTable&& other)
		{
			destroy();
			buckets = other.buckets;
			numBuckets = other.numBuckets;
			numItems = other.numItems;
			placeholder.next = other.placeholder.next;
			if (placeholder.next)
			{
				// Update bucket pointing to placeholder
				sizet const bucketIdx = getBucketIdx(placeholder.next);
				buckets[bucketIdx] = &placeholder;
			}

			other.buckets = nullptr;
			other.numBuckets = 0;
			other.numItems = 0;
			other.placeholder.next = nullptr;

			return *this;
		}

		/**
		 * @brief Destroy this hash table.
		 */
		FORCE_INLINE ~HashTable()
		{
			destroy();
		}

		/**
		 * @brief Returns the number of items in
		 * the table.
		 */
		FORCE_INLINE sizet getSize() const
		{
			return numItems;
		}

		/**
		 * @brief Returns the number of buckets
		 * used, just for debug purpose.
		 */
		FORCE_INLINE sizet getNumBuckets() const
		{
			return numBuckets;
		}

		/**
		 * @brief Returns an iterator that points
		 * to the first item of the table.
		 * @{
		 */
		FORCE_INLINE IteratorT begin()
		{
			return {placeholder.next};
		}

		FORCE_INLINE ConstIteratorT begin() const
		{
			return const_cast<HashTable&>(*this).begin();
		}
		/** @} */

		/**
		 * @brief Returns an iterator that points
		 * beyond the last item of the table.
		 * @{
		 */
		FORCE_INLINE IteratorT end()
		{
			return {};
		}

		FORCE_INLINE ConstIteratorT end() const
		{
			return const_cast<HashTable&>(*this).end();
		}
		/** @} */

		/**
		 * @brief Search the table for an item
		 * matching the given key.
		 *
		 * @param key key used to locate item
		 * @return iterator pointing to the first
		 * item that matches the given key
		 * @{
		 */
		FORCE_INLINE IteratorT find(auto const& key)
		{
			// Search in buckets
			BucketT* node = locateNode(key);
			return {node};
		}

		/**
		 * @brief
		 *
		 * @param key
		 * @return FORCE_INLINE
		 */
		FORCE_INLINE ConstIteratorT find(auto const& key) const
		{
			return const_cast<HashTable&>(*this).find(key);
		}
		/** @} */

		/**
		 * @brief Construct and insert a new item
		 * in the table.
		 *
		 * @param createArgs arguments used to
		 * construct the item
		 * @return iterator pointing to created
		 * item
		 */
		IteratorT emplace(auto&& ...createArgs)
		{
			// FIXME: this should be changed allow duplicates
			return findOrEmplace(FORWARD(createArgs)...);
		}

		/**
		 * @brief Insert a new item in the table.
		 *
		 * The item can be duplicate.
		 *
		 * @param item item to insert
		 * @return iterator pointing to inserted
		 * item
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
		 * @brief Construct and insert a new item,
		 * if no duplicate exists.
		 *
		 * Two items are duplicate if they have the
		 * same hash key and compare equal.
		 *
		 * The item is first constructed, and
		 * destroyed if a duplicate exists.
		 *
		 * @param createArgs arugments used to create
		 * the item
		 * @return iterator pointing to inserted item,
		 * or to existing item if duplicate
		 */
		IteratorT findOrEmplace(auto&& ...createArgs)
		{
			// We need to create the bucket to comput the key.
			// If the key already exists the bucket will be destroyed
			BucketT* node = createNode(FORWARD(createArgs)...);
			node->hkey = HashBucket_Impl::computeHash(node->value, HashPolicyT{});

			// Search in buckets
			if (BucketT* found = locateNode(node->value, node->hkey))
			{
				// Destroy bucket first
				destroyNode(node);
				return {found};
			}

			// Reserve space and compute bucket idx
			reserve(1);

			// Push to front of bucket
			pushToBucket(node, getBucketIdx(node));
			numItems++;

			return {node};
		}

		/**
		 * @brief Inserts a new item if no duplicate
		 * exists.
		 * @see findOrEmplace
		 *
		 * @param item item to insert
		 * @return iterator pointing to inserted item,
		 * or to existing item if duplicate
		 * @{
		 */
		FORCE_INLINE IteratorT findOrInsert(T const& item)
		{
			return findOrInsert_Impl(item);
		}

		FORCE_INLINE IteratorT findOrInsert(T&& item)
		{
			return findOrInsert_Impl(move(item));
		}
		/** @} */

		/**
		 * @brief Remove the item pointed by the
		 * given iterator from the table.
		 *
		 * If the iterator doesn't belong to this
		 * table, the behavior of the function is
		 * undefined.
		 *
		 * @param it iterator pointing to item to
		 * remove
		 * @return next valid iterator
		 */
		IteratorT remove(ConstIteratorT it)
		{
			BucketT* node = const_cast<BucketT*>(it.curr);
			sizet const bucketIdx = getBucketIdx(node);

			// Get ptr to prev node
			BucketT* prev = buckets[bucketIdx];
			for (; prev->next != node; prev = prev->next);
			KORIN_ASSERTF(prev != nullptr, "Trying to remove an item that belongs to someone else")

			sizet nextBucketIdx = bucketIdx;
			if (node->next)
			{
				// If the node we are removing is a
				// placeholder for the next bucket
				// we need to udpate the placeholder
				nextBucketIdx = getBucketIdx(node->next);
				buckets[nextBucketIdx] = prev;
			}

			if (prev == buckets[bucketIdx] && (!node->next || nextBucketIdx != bucketIdx))
			{
				// Bucket is empty now, it should work
				// anyway but this should be more
				// efficient
				buckets[bucketIdx] = nullptr;
			}

			// Unlink from chain and dealloc
			prev->next = node->next;
			destroyNode(node);
			numItems--;

			return {prev->next};
		}

		/**
		 * @brief Removes all items from the table.
		 */
		void clear()
		{
			if (numItems == 0)
				// Skip if no items
				return;

			BucketT*& it = placeholder.next;
			while (it)
			{
				// Destroy all nodes
				BucketT* next = it->next;
				destroyNode(it);
				it = next;
			}

			for (sizet bucketIdx = 0; bucketIdx < numBuckets; ++bucketIdx)
			{
				// Reset buckets
				buckets[bucketIdx] = nullptr;
			}

			// Reset num items
			numItems = 0;
		}

		// -----------------------------------------------------
		/**
		 * @brief Construct a new node.
		 *
		 * @param createArgs arguments used to
		 * construct node item
		 * @return ptr to created node
		 */
		FORCE_INLINE BucketT* createNode(auto&& ...createArgs)
		{
			void* mem = gMalloc->malloc(sizeof(BucketT), alignof(BucketT));
			return new (mem) BucketT{{FORWARD(createArgs)...}};
		}

		/**
		 * @brief Destroy an existing node.
		 *
		 * If the node does not belong to this
		 * table, the behavior is undefined.
		 *
		 * @param node ptr to node to destroy
		 */
		FORCE_INLINE void destroyNode(BucketT* node)
		{
			node->~BucketT();
			gMalloc->free(node);
		}

		/**
		 * @brief Push a node to the front of
		 * the bucket identified by the given
		 * bucket idx.
		 *
		 * if the index is out of bound,
		 * behavior is undefined.
		 *
		 * @param node ptr to the node to push
		 * @param bucketIdx index of the bucket
		 * to push to
		 */
		FORCE_INLINE void pushToBucket(BucketT* node, sizet bucketIdx)
		{
			KORIN_ASSERT(bucketIdx < numBuckets);
			if (buckets[bucketIdx])
			{
				node->next = buckets[bucketIdx]->next;
				buckets[bucketIdx]->next = node;
			}
			else
			{
				// Bucket is empty
				// This trick is only used to update the chain
				// without having a ptr to the prev node
				node->next = placeholder.next;
				placeholder.next = node;

				if (node->next)
				{
					buckets[getBucketIdx(node->next)] = node;
				}

				buckets[bucketIdx] = &placeholder;
			}
		}

		/**
		 * @brief Copy nodes of another hash table.
		 *
		 * @param nodes other tabel nodes
		 */
		void copyNodes(BucketT* nodes)
		{
			for (BucketT* it = nodes; it; it = it->next)
			{
				// Create new node
				BucketT* node = createNode(it->value);
				node->hkey = it->hkey;

				// Push to bucket
				pushToBucket(node, getBucketIdx(node));
			}
		}

		/**
		 * @brief Called by the destructor.
		 *
		 * Removes all items and deallocs buckets.
		 */
		FORCE_INLINE void destroy()
		{
			if (buckets)
			{
				// Remove and destroy all items
				clear();

				// Deallocate all buckets
				gMalloc->free(buckets);
				numBuckets = 0;
			}
		}

		/**
		 * @brief Reserve space for extra items.
		 *
		 * If the new number items exceeds the
		 * maximum load factor, the table is
		 * reallocated with double the number of
		 * buckets.
		 *
		 * If realloc is necessary, all items
		 * must be rehashed. This does not
		 * invalidated existing iterators.
		 *
		 * @param extraItems
		 * @return true if realloc was necessary
		 * @return false otherwise
		 */
		FORCE_INLINE bool reserve(sizet extraItems)
		{
			sizet const newNumItems = numItems + extraItems;
			if (newNumItems / static_cast<float>(numBuckets) >= HASH_BUCKET_LOAD_FACTOR)
			{
				// Double the number of buckets
				reallocBuckets(numBuckets << 1);
				return true;
			}

			return false;
		}

		/**
		 * @brief Allocate the desired number of
		 * buckets.
		 *
		 * If buckets already exist, all items are
		 * redistributed in the various buckets.
		 *
		 * If the deisired number of buckets is not
		 * a power of two, it is rounded up to the
		 * smallest larger power of two (e.g. 17 ->
		 * 32).
		 *
		 * @param desiredNumBuckets new number of
		 * buckets
		 */
		void reallocBuckets(sizet desiredNumBuckets)
		{
			BucketT** oldBuckets = buckets;
			sizet const oldNumBuckets = numBuckets;

			// TODO: Check number of buckets is po2 and round up if necessary
			numBuckets = desiredNumBuckets;
			if (numBuckets == oldNumBuckets)
				// Skip, no need to realloc
				return;

			// Allocate buckets and initialize them
			void* mem = gMalloc->malloc(numBuckets * sizeof(*buckets));
			buckets = new (mem) BucketT*[numBuckets]{};

			if (oldBuckets)
			{
				// Rehash all items
				BucketT* head = placeholder.next;
				placeholder.next = nullptr;
				while (head)
				{
					// Reinsert all items
					BucketT* next = head->next;
					pushToBucket(head, getBucketIdx(head));
					head = next;
				}

				// Destroy old buckets
				gMalloc->free(oldBuckets);
			}
		}

		/**
		 * @brief Returns the bucket index for the
		 * given hash key
		 *
		 * @param hkey hash key
		 * @return index of the bucket
		 */
		FORCE_INLINE sizet getBucketIdx(HashKey hkey) const
		{
			return hkey & (numBuckets - 1);
		}

		/**
		 * @brief Returns the bucket index for the
		 * given node
		 *
		 * @param node ptr to node
		 * @return index of the bucket
		 */
		FORCE_INLINE sizet getBucketIdx(BucketT* node)
		{
			return getBucketIdx(node->hkey);
		}

		/**
		 * @brief Returns a ptr to the first node
		 * of the bucket identified by the bucket
		 * index.
		 *
		 * @param bucketIdx index of the bucket
		 * @return ptr to first node of bucket
		 */
		FORCE_INLINE BucketT* getBucketFirstNode(sizet bucketIdx)
		{
			BucketT* prev = buckets[bucketIdx];
			return prev ? prev->next : prev;
		}

		/**
		 * @brief Attempts to find a node with the
		 * given key.
		 *
		 * @param key key used to find node
		 * @param hkey hash key corresponding to the
		 * given key
		 * @return ptr to first node that matches key
		 * @return nullptr if no such node existsù
		 * @{
		 */
		BucketT* locateNode([[maybe_unused]] auto const& key, HashKey hkey)
		{
			sizet const bucketIdx = getBucketIdx(hkey);
			for (BucketT* it = getBucketFirstNode(bucketIdx);
			     it && getBucketIdx(it->hkey) == bucketIdx;
				 it = it->next)
			{
				if (it->hkey == hkey)
				{
					return it;
				}
			}

			return nullptr;
		}

		FORCE_INLINE BucketT* locateNode(auto const& key)
		{
			return locateNode(key, HashBucket_Impl::computeHash(key, HashPolicyT{}));
		}
		/** @} */

		/**
		 * @brief Implementation for @c findOrInsert()
		 * method.
		 *
		 * @param item item to insert
		 * @return iterator pointing to inserted item,
		 * or first existing item if duplicate
		 */
		IteratorT findOrInsert_Impl(auto&& item)
		{
			// Compute hkey and try to locate item
			HashKey const hkey = HashBucket_Impl::computeHash(item, HashPolicyT{});
			if (BucketT* found = locateNode(item, hkey))
			{
				return {found};
			}

			// Reserve space for new item
			reserve(1);

			// Create a new node
			BucketT* node = createNode(FORWARD(item));
			node->hkey = hkey;

			// Push node to bucket
			pushToBucket(node, getBucketIdx(node));
			numItems++;

			return {node};
		}
	};
} // namespace Korin
