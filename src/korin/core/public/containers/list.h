#pragma once

#include "containers_types.h"
#include "templates/utility.h"

namespace Korin
{
	template<typename> struct ListIterator;
	template<typename> struct ListConstIterator;

	/**
	 * @brief A node of a doubly-linked list.
	 * 
	 * @tparam type of the node value
	 */
	template<typename T>
	struct ListNode
	{
		/// @brief The value of the node.
		T value;

		/// @brief Ptr to the next node.
		ListNode* next = nullptr;

		/// @brief Ptr to the previous node.
		ListNode* prev = nullptr;
	};

	/**
	 * @brief An iterator used to iterate over a
	 * list.
	 * 
	 * @tparam type of the list node
	 */
	template<typename T>
	struct ListIterator
	{
		friend ListConstIterator<T>;
		friend List<T>;

		using SelfT = ListIterator;
		using NodeT = ListNode<T>;
		using RefT = T&;
		using PtrT = T*;

		/**
		 * @brief Construct an iterator that starts
		 * from the given node.
		 * 
		 * @param inNode starting node
		 */
		FORCE_INLINE explicit ListIterator(NodeT* inNode, [[maybe_unused]] void* inList)
			: node{inNode}
#if !BUILD_RELEASE
			, list{inList}
#endif
		{
			//
		}

		/**
		 * @brief Return a reference to the current
		 * node value.
		 */
		FORCE_INLINE RefT operator*() const
		{
			return node->value;
		}

		/**
		 * @brief Return a pointer to the current
		 * node value.
		 */
		FORCE_INLINE PtrT operator->() const
		{
			return &(*this);
		}

		/**
		 * @brief Compare two iterators, return
		 * true if they point to the same node.
		 * Nodes should belong to the same list.
		 * 
		 * @param other another iterator
		 */
		FORCE_INLINE bool operator==(const SelfT& other) const
		{
			// TODO: Check same list
			return node == other.node;
		}

		/**
		 * @brief Compare two iterators, return
		 * true if they point to different nodes.
		 * 
		 * @param other another iterator
		 */
		FORCE_INLINE bool operator!=(const SelfT& other) const
		{
			return !(*this == other);
		}

		/**
		 * @brief Step forward iterator and return
		 * ref to self.
		 */
		FORCE_INLINE SelfT& operator++()
		{
			node = node->next;
			return *this;
		}

		/**
		 * @brief Step forward iterator and return
		 * a new iterator pointing to the previous
		 * node.
		 */
		FORCE_INLINE SelfT operator++(int32)
		{
			SelfT other{*this};
			++(*this);
			return other;
		}

		/**
		 * @brief Step backward iterator and return
		 * ref to self.
		 */
		FORCE_INLINE SelfT& operator--()
		{
			node = node->prev;
			return *this;
		}

		/**
		 * @brief Step backward iterator and return
		 * a new iterator pointing to the previous
		 * node.
		 */
		FORCE_INLINE SelfT operator--(int32)
		{
			SelfT other{*this};
			--(*this);
			return other;
		}

	protected:
		ListIterator() = delete;

		/// @brief Pointer to current node.
		NodeT* node;

#if !BUILD_RELEASE
		/// @brief The actual list
		void* list;
#endif
	};

	/**
	 * @brief Like ListIterator<T> prevent any
	 * write to the list values.
	 * 
	 * @tparam type of the list items
	 */
	template<typename T>
	struct ListConstIterator
	{
		friend List<T>;

		using SelfT = ListConstIterator;
		using NodeT = ListNode<T>;
		using IteratorT = ListIterator<T>;
		using RefT = T const&;
		using PtrT = T const*;

		/**
		 * @brief Construct an iterator that starts
		 * from the given node.
		 * 
		 * @param inNode starting node
		 */
		FORCE_INLINE explicit ListConstIterator(NodeT* inNode, [[maybe_unused]] void* inList)
			: node{inNode}
#if !BUILD_RELEASE
			, list{inList}
#endif
		{
			//
		}

		/**
		 * @brief Cast an iterator to a const iterator.
		 * 
		 * @param other a non-const iterator
		 */
		FORCE_INLINE ListConstIterator(IteratorT const& other)
			: node{other.node}
#if !BUILD_RELEASE
			, list{other.list}
#endif
		{
			//
		}

		/**
		 * @brief Return a reference to the current
		 * node value.
		 */
		FORCE_INLINE RefT operator*() const
		{
			return node->value;
		}

		/**
		 * @brief Return a pointer to the current
		 * node value.
		 */
		FORCE_INLINE PtrT operator->() const
		{
			return &(*this);
		}

		/**
		 * @brief Compare two iterators, return
		 * true if they point to the same node.
		 * Nodes should belong to the same list.
		 * 
		 * @param other another iterator
		 */
		FORCE_INLINE bool operator==(const SelfT& other) const
		{
			// TODO: Check same list
			return node == other.node;
		}

		/**
		 * @brief Compare two iterators, return
		 * true if they point to different nodes.
		 * 
		 * @param other another iterator
		 */
		FORCE_INLINE bool operator!=(const SelfT& other) const
		{
			return !(*this == other);
		}

		/**
		 * @brief Step forward iterator and return
		 * ref to self.
		 */
		FORCE_INLINE SelfT& operator++()
		{
			node = node->next;
			return *this;
		}

		/**
		 * @brief Step forward iterator and return
		 * a new iterator pointing to the previous
		 * node.
		 */
		FORCE_INLINE SelfT operator++(int32)
		{
			SelfT other{*this};
			++(*this);
			return other;
		}

		/**
		 * @brief Step backward iterator and return
		 * ref to self.
		 */
		FORCE_INLINE SelfT& operator--()
		{
			node = node->prev;
			return *this;
		}

		/**
		 * @brief Step backward iterator and return
		 * a new iterator pointing to the previous
		 * node.
		 */
		FORCE_INLINE SelfT operator--(int32)
		{
			SelfT other{*this};
			--(*this);
			return other;
		}

	protected:
		ListConstIterator() = delete;

		/// @brief Pointer to current node.
		NodeT const* node;

#if !BUILD_RELEASE
		/// @brief The actual list
		void* list;
#endif
	};

	/**
	 * @brief A doubly-linked list, with constant
	 * time insertion and removal of items.
	 * 
	 * @tparam T the type of the items
	 */
	template<typename T>
	class List
	{
		using NodeT = ListNode<T>;

	public:
		using IteratorT = ListIterator<T>;
		using ConstIteratorT = ListConstIterator<T>;

		/**
		 * @brief Construct an empty list.
		 */
		FORCE_INLINE explicit List()
			: head{nullptr}
			, tail{nullptr}
			, numNodes{0ull}
		{
			//
		}

		/**
		 * @brief Copy-construct this list.
		 * 
		 * @param other another list
		 */
		FORCE_INLINE List(List const& other)
			: List{}
		{
			// Copy the list
			copyList(other.head);
			numNodes = other.numNodes;
		}

		/**
		 * @brief Move-construct this list.
		 * 
		 * @param other another list
		 */
		FORCE_INLINE List(List&& other)
			: head{other.head}
			, tail{other.tail}
			, numNodes{other.numNodes}
		{
			other.head = other.tail = nullptr;
			other.numNodes = 0ull;
		}

		/**
		 * @brief Copy-assign another list to this
		 * list.
		 * 
		 * @param other another list
		 * @return ref to self
		 */
		FORCE_INLINE List& operator=(List const& other)
		{
			copyList(other.head);
			numNodes = other.numNodes;

			return *this;
		}

		/**
		 * @brief Move-assign anothre list to this
		 * list.
		 * 
		 * @param other another list
		 * @return ref to self
		 */
		FORCE_INLINE List& operator=(List&& other)
		{
			// Swap with the other list.
			// Then the other list will be destroyed
			swap(head, other.head);
			swap(tail, other.tail);
			swap(numNodes, other.numNodes);

			return *this;
		}

		/**
		 * @brief Destroy list, removing all the nodes.
		 */
		FORCE_INLINE ~List()
		{
			destroy();
		}

		/**
		 * @brief Return the number of nodes.
		 */
		FORCE_INLINE sizet getNumNodes() const
		{
			return numNodes;
		}

		/**
		 * @{
		 * @brief Return a pointer to the head node.
		 */
		FORCE_INLINE NodeT const* getHead() const
		{
			return head;
		}

		FORCE_INLINE NodeT* getHead()
		{
			return head;
		}
		/** @} */

		/**
		 * @{
		 * @brief Return a pointer to the tail node.
		 * 
		 */
		FORCE_INLINE NodeT const* getTail() const
		{
			return tail;
		}

		FORCE_INLINE NodeT* getTail()
		{
			return tail;
		}
		/** @} */

		/**
		 * @brief Return the first value of the list.
		 */
		FORCE_INLINE T const& getFirst() const
		{
			return head->value;
		}

		/**
		 * @brief Return the last value of the list.
		 */
		FORCE_INLINE T const& getLast() const
		{
			return tail->value;
		}

		/**
		 * @{
		 * @brief Return an iterator pointing to
		 * the first item of the list.
		 */
		FORCE_INLINE ConstIteratorT begin() const
		{
			return ConstIteratorT{head, this};
		}

		FORCE_INLINE IteratorT begin()
		{
			return IteratorT{head, this};
		}
		/** @} */

		/**
		 * @{
		 * @brief Return an iterator pointing past
		 * the last item of the list.
		 */
		FORCE_INLINE ConstIteratorT end() const
		{
			return ConstIteratorT{nullptr, this};
		}

		FORCE_INLINE IteratorT end()
		{
			return IteratorT{nullptr, this};
		}
		/** @} */

		/**
		 * @{
		 * @brief Return a reverse iterator pointing
		 * to the last item of the list.
		 */
		FORCE_INLINE ConstIteratorT rbegin() const
		{
			return ConstIteratorT{tail, this};
		}

		FORCE_INLINE IteratorT rbegin()
		{
			return IteratorT{tail, this};
		}
		/** @} */

		/**
		 * @{
		 * @brief Return a reverse iterator pointing
		 * past the first item of the list.
		 */
		FORCE_INLINE ConstIteratorT rend() const
		{
			return ConstIteratorT{nullptr, this};
		}

		FORCE_INLINE IteratorT rend()
		{
			return IteratorT{nullptr, this};
		}
		/** @} */

		/**
		 * @brief Append a new value to the end
		 * of the list.
		 * 
		 * @param createArgs arguments used to
		 * construct the value
		 */
		void emplaceBack(auto&& ...createArgs)
		{
			if (!tail)
			{
				head = tail = createNode(FORWARD(createArgs)...);
			}
			else
			{
				tail->next = createNode(FORWARD(createArgs)...);
				tail->next->prev = tail;
				tail = tail->next;
			}
			
			numNodes++;
		}

		/**
		 * @{
		 * @brief Append a value to the end
		 * of the list.
		 * 
		 * @param value value to append
		 */
		FORCE_INLINE void pushBack(T const& value)
		{
			emplaceBack(value);
		}

		FORCE_INLINE void pushBack(T&& value)
		{
			emplaceBack(move(value));
		}
		/** @} */

		/**
		 * @brief Append a value to the
		 * beginning of the list.
		 * 
		 * @param createArgs arguments passed to
		 * the value constructor
		 */
		void emplaceFront(auto&& ...createArgs)
		{
			// TODO: Add check after create node
			
			if (!head)
			{
				// Create first node
				head = tail = createNode(FORWARD(createArgs)...);
			}
			else
			{
				head->prev = createNode(FORWARD(createArgs)...);
				head->prev->next = head;
				head = head->prev;
			}

			numNodes++;
		}

		/**
		 * @{
		 * @brief Append a value to the
		 * beginning of the list.
		 * 
		 * @param value value to append
		 */
		FORCE_INLINE void pushFront(T const& value)
		{
			emplaceFront(value);
		}

		FORCE_INLINE void pushFront(T&& value)
		{
			emplaceFront(move(value));
		}
		/** @} */

		/**
		 * @brief Insert a new value after the
		 * given node.
		 * 
		 * @param node node after which to insert
		 * @param createArgs arguments used to
		 * construct the new value
		 */
		void emplaceAfter(NodeT* node, auto&& ...createArgs)
		{
			// TODO: Check this list has the node
			NodeT* next = createNode(FORWARD(createArgs)...);
			if (node->next)
			{
				// Rather confusing naming
				next->next = node->next;
				next->next->prev = next;
			}
			else
			{
				tail = next;
			}
			node->next = next;
			next->prev = node;

			numNodes++;
		}

		/**
		 * @brief Insert a new value after the node
		 * pointed by the iterator.
		 * 
		 * @param it iterator pointing to the node
		 * after which to insert the value
		 * @param createArgs arguments used to
		 * construct the new value
		 */
		FORCE_INLINE void emplaceAfter(ConstIteratorT const& it, auto&& ...createArgs)
		{
			emplaceAfter(const_cast<NodeT*>(it.node), FORWARD(createArgs)...);
		}

		/**
		 * @{
		 * @brief Insert a value after the given node.
		 * @see emplaceAfter(NodeT*, auto&&...)
		 */
		FORCE_INLINE void insertAfter(NodeT* node, T const& value)
		{
			emplaceAfter(node, value);
		}

		FORCE_INLINE void insertAfter(NodeT* node, T&& value)
		{
			emplaceAfter(node, move(value));
		}
		/** @} */

		/**
		 * @{
		 * @brief Insert a value after the node pointed
		 * by the given iterator.
		 * @see emplaceAfter(ConstIteratorT, auto&&...)
		 */
		FORCE_INLINE void insertAfter(ConstIteratorT const& it, T const& value)
		{
			emplaceAfter(it, value);
		}

		FORCE_INLINE void insertAfter(ConstIteratorT const& it, T&& value)
		{
			emplaceAfter(it, move(value));
		}
		/** @} */

		/**
		 * @brief Insert a new value before the
		 * given node.
		 * 
		 * @param node node before which to insert
		 * @param createArgs arguments used to
		 * construct the new value
		 */
		void emplaceBefore(NodeT* node, auto&& ...createArgs)
		{
			// TODO: Check this list has the node
			NodeT* prev = createNode(FORWARD(createArgs)...);
			if (node->prev)
			{
				// Rather confusing naming
				prev->prev = node->prev;
				prev->prev->prev = prev;
			}
			else
			{
				head = prev;
			}
			node->prev = prev;
			prev->next = node;

			numNodes++;
		}

		/**
		 * @brief Insert a new value before the node
		 * pointed by the iterator.
		 * 
		 * @param it iterator pointing to the node
		 * before which to insert the value
		 * @param createArgs arguments used to
		 * construct the new value
		 */
		FORCE_INLINE void emplaceBefore(ConstIteratorT const& it, auto&& ...createArgs)
		{
			emplaceBefore(const_cast<NodeT*>(it.node), FORWARD(createArgs)...);
		}

		/**
		 * @{
		 * @brief Insert a value before the given node.
		 * @see emplaceAfter(NodeT*, auto&&...)
		 */
		FORCE_INLINE void insertBefore(NodeT* node, T const& value)
		{
			emplaceBefore(node, value);
		}

		FORCE_INLINE void insertBefore(NodeT* node, T&& value)
		{
			emplaceBefore(node, move(value));
		}
		/** @} */

		/**
		 * @{
		 * @brief Insert a value before the node pointed
		 * by the given iterator.
		 * @see emplaceAfter(ConstIteratorT, auto&&...)
		 */
		FORCE_INLINE void insertBefore(ConstIteratorT const& it, T const& value)
		{
			emplaceBefore(it, value);
		}

		FORCE_INLINE void insertBefore(ConstIteratorT const& it, T&& value)
		{
			emplaceBefore(it, move(value));
		}
		/** @} */

		/**
		 * @brief Remove and return the last value
		 * of the list. Undefined behavior if the
		 * list is empty.
		 */
		T popBack()
		{
			// TODO: Check tail exists
			NodeT* prev = tail->prev;
			T value{move(tail->value)};
			destroyNode(tail);
			numNodes--;

			if (prev)
			{
				tail = prev;
				tail->next = nullptr;
			}
			else
			{
				tail = head = nullptr;
			}

			return value;
		}

		/**
		 * @brief Move the last value and then remove
		 * it from the list, if the list is not empty.
		 * 
		 * @param outValue value to move
		 * @return true if list was not empty
		 */
		FORCE_INLINE bool popBack(T& outValue)
		{
			if (!tail) return false;

			outValue = popBack();
			return true;
		}

		/**
		 * @brief Remove and return the first value
		 * of the list. Undefined behavior if the
		 * list is empty.
		 */
		T popFront()
		{
			// TODO: Check head exists
			NodeT* next = head->next;
			T value{move(head->value)};
			destroyNode(head);
			numNodes--;

			if (next)
			{
				head = next;
				head->prev = nullptr;
			}
			else
			{
				head = tail = nullptr;
			}

			return value;
		}

		/**
		 * @brief Move the first value and then remove
		 * it from the list, if the list is not empty.
		 * 
		 * @param outValue value to move
		 * @return true if list was not empty
		 */
		FORCE_INLINE bool popFront(T& outValue)
		{
			if (!head) return false;

			outValue = popFront();
			return true;
		}

		/**
		 * @brief Remove one ore more values from
		 * the list, starting from the given node.
		 * 
		 * @param node starting node
		 * @param n number of nodes to remove
		 */
		void removeAt(NodeT* node, sizet n = 1ull)
		{
			// TODO: Check that num does not exceed list length

			if (n == numNodes)
			{
				// Empty the list
				destroy();
				return;
			}

			NodeT* prev = node->prev;

			for (sizet i = 0; i < n; ++i)
			{
				NodeT* next = node->next;
				destroyNode(node);
				node = next;
			}

			if (node && prev)
			{
				prev->next = node;
				node->prev = prev;
			}
			else if (prev)
			{
				prev->next = nullptr;
				tail = prev;
			}
			else if (node)
			{
				node->prev = nullptr;
				head = node;
			}
			else; // Case handled above (n == numNodes)

			numNodes -= n;
		}

		/**
		 * @brief Remove one or more values from
		 * the list, starting with the value pointed
		 * by the given iterator.
		 * 
		 * @param it iterator pointing to the starting
		 * node
		 * @param n number of nodes to remove
		 */
		FORCE_INLINE void removeAt(ConstIteratorT const& it, sizet n = 1ull)
		{
			removeAt(const_cast<NodeT*>(it.node), n);
		}

		/**
		 * @brief Remove all nodes from the list.
		 */
		FORCE_INLINE void reset()
		{
			destroy();
		}

	protected:
		/**
		 * @brief Creates a new node with the
		 * given value.
		 * 
		 * @param createArgs arguments used to
		 * create the value
		 * @return ptr to the created node
		 */
		FORCE_INLINE NodeT* createNode(auto&& ...createArgs)
		{
			// TODO: Replace with allocator
			return new (::malloc(sizeof(NodeT))) NodeT{{FORWARD(createArgs)...}};
		}

		/**
		 * @brief Destroy a node of this list.
		 * 
		 * @parma node ptr to node to destroy
		 */
		FORCE_INLINE void destroyNode(NodeT* node)
		{
			// TODO: Check node is not nullptr
			node->~NodeT();
			::free(node);
		}

		/**
		 * @brief Copy the list structure (nodes and
		 * values) from another list, starting from
		 * the given node.
		 * 
		 * @param node (head) node of the other list
		 */
		void copyList(NodeT const* node)
		{
			if (!head && !node)
			{
				return;
			}
			else if (!head)
			{
				// Copy all nodes
				head = tail = createNode(node->value);
				for (NodeT* it = node->next; it; it = it->next)
				{
					tail->next = createNode(it->value);
					tail->next->prev = tail;
					tail = tail->next;
				}
			}
			else if (!node)
			{
				// Destroy the list
				destroy();
			}
			else
			{
				// Copy only values until possible
				NodeT* it = head;
				for (; it && node; it = it->next, node = node->next)
				{
					it->value = node->value;
				}

				if (it)
				{
					// Destroy remaining nodes
					tail = it->prev;
					tail->next = nullptr;
					while (it)
					{
						NodeT* next = it->next;
						destroyNode(it);
						it = next;
					}
				}
				else
				{
					// Create new nodes
					for (; node; node = node->next)
					{
						tail->next = createNode(it->value);
						tail->next->prev = tail;
						tail = tail->next;
					}
				}
			}
		}

		/**
		 * @brief Remove all nodes from the list
		 * and reset to initial state.
		 */
		void destroy()
		{
			while (head)
			{
				NodeT* node = head;
				head = head->next;
				destroyNode(node);
			}
			
			head = tail = nullptr;
			numNodes = 0ull;
		}

		/**
		 * @brief Return true if the node to test
		 * is part of this list.
		 */
		FORCE_INLINE bool containsNode(NodeT const* node) const
		{
			for (NodeT const* it = head; it; it = it->next)
			{
				if (it == node) return true;
			}

			return false;
		}

		/// @brief Head node.
		NodeT* head;

		/// @brief Tail node.
		NodeT* tail;

		/// @brief Number of nodes in the list.
		sizet numNodes;
	};
} // namespace Korin
