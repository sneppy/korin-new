#pragma once

#include "containers_types.h"
#include "tree_node.h"
#include "templates/utility.h"

namespace Containers
{
	template<typename T> struct TreeIterator;
	template<typename T> struct TreeConstIterator;

	/**
	 * @brief An iterator used to iterate over
	 * the nodes of a tree.
	 * 
	 * @tparam T type of the values of the nodes
	 */
	template<typename T>
	struct TreeIterator
	{
		friend TreeConstIterator<T>;

		using SelfT = TreeIterator;
		using NodeT = BinaryNode<T>;
		using RefT = T&;
		using PtrT = T*;

		/**
		 * @brief Construct an iterator that starts
		 * from the given node.
		 * 
		 * @param inNode ptr to starting node
		 */
		FORCE_INLINE explicit TreeIterator(NodeT* inNode, [[maybe_unused]] void* inTree)
			: node{inNode}
#if !BUILD_RELEASE
			, tree{inTree}
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
		 * Nodes should belong to the same tree.
		 * 
		 * @param other another iterator
		 */
		FORCE_INLINE bool operator==(SelfT const& other) const
		{
			// TODO: Check same tree
			return node == other.node;
		}

		/**
		 * @brief Compare two iterators, return
		 * true if they point to different nodes.
		 * 
		 * @param other another iterator
		 */
		FORCE_INLINE bool operator!=(SelfT const& other) const
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
		TreeIterator() = delete;

		/// @brief The node pointed by this iterator.
		NodeT* node;

#if !BUILD_RELEASE
		/// @brief Address of the actual tree instance.
		void* tree;
#endif
	};

	/**
	 * @brief Like TreeIterator<T> but prevents
	 * writing the values.
	 * 
	 * @tparam T the type of the node values
	 */
	template<typename T>
	struct TreeConstIterator
	{
		template<typename, typename> friend class Tree;

		using SelfT = TreeConstIterator;
		using NodeT = BinaryNode<T>;
		using IteratorT = TreeIterator<T>;
		using RefT = T const&;
		using PtrT = T const*;

		/**
		 * @brief Construct an iterator that starts
		 * from the given node.
		 * 
		 * @param inNode ptr to starting node
		 */
		FORCE_INLINE explicit TreeConstIterator(NodeT* inNode, [[maybe_unused]] void* inTree)
			: node{inNode}
#if !BUILD_RELEASE
			, tree{inTree}
#endif
		{
			//
		}

		/**
		 * @brief Copy a non-const iterator.
		 * 
		 * @param other a non-const iterator
		 */
		FORCE_INLINE TreeConstIterator(IteratorT const& other)
			: node{other.node}
#if !BUILD_RELEASE
			, tree{other.tree}
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
		 * Nodes should belong to the same tree.
		 * 
		 * @param other another iterator
		 */
		FORCE_INLINE bool operator==(SelfT const& other) const
		{
			// TODO: Check same tree
			return node == other.node;
		}

		/**
		 * @brief Compare two iterators, return
		 * true if they point to different nodes.
		 * 
		 * @param other another iterator
		 */
		FORCE_INLINE bool operator!=(SelfT const& other) const
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
		TreeConstIterator() = delete;

		/// @brief The node pointed by this iterator.
		NodeT* node;

#if !BUILD_RELEASE
		/// @brief Address of the actual tree instance.
		void* tree;
#endif
	};

	/**
	 * @brief A binary tree container implemented
	 * with a red-black tree, with logarithmic
	 * time insertion and removal.
	 * 
	 * @tparam T type of the node values
	 * @tparam CompareT the comparison operator
	 * used to compare node values
	 */
	template<typename T, typename CompareT>
	class Tree
	{
		template<typename, typename, typename> friend class Map;
		template<typename, typename>		   friend class Set;

		using NodeT = BinaryNode<T>;

	public:
		using IteratorT = TreeIterator<T>;
		using ConstIteratorT = TreeConstIterator<T>;

		/**
		 * @brief Construct an empty tree.
		 */
		FORCE_INLINE Tree()
			: root{nullptr}
			, numNodes{0ull}
		{
			//
		}

		/**
		 * @brief Return the number of nodes in
		 * the tree.
		 */
		FORCE_INLINE sizet getNumNodes() const
		{
			// TODO: Check against actual number of nodes
			return numNodes;
		}

		/**
		 * @brief Return a pointer to the root
		 * node. The node cannot be modified.
		 */
		FORCE_INLINE NodeT const* getRootNode() const
		{
			return root;
		}

		/**
		 * @brief Return a pointer to the leftmost
		 * node of the tree (which holds the
		 * minimum value).
		 */
		FORCE_INLINE NodeT const* getMinNode() const
		{
			if (!root) return nullptr;
			
			NodeT* it = root;
			while (it->left) it = it->left;
			return it;
		}

		/**
		 * @brief Return a pointer to the rightmost
		 * node of the tree (which holds the
		 * maximum value).
		 */
		FORCE_INLINE NodeT const* getMaxNode() const
		{
			if (!root) return nullptr;
			
			NodeT* it = root;
			while (it->right) it = it->right;
			return it;
		}

		/**
		 * @brief Return the min value of the tree.
		 * If the tree is empty, the behavior is
		 * undefined.
		 */
		FORCE_INLINE T const& getMin() const
		{
			return getMinNode()->value;
		}

		/**
		 * @brief Return the max value of the tree.
		 * If the tree is empty, the behavior is
		 * undefined.
		 */
		FORCE_INLINE T const& getMax() const
		{
			return getMaxNode()->value;
		}

		/**
		 * @{
		 * @brief Return an iterator pointing to
		 * the leftmost (min) node of the tree.
		 */
		FORCE_INLINE ConstIteratorT begin() const
		{
			NodeT const* min = getMinNode();
			return ConstIteratorT{min, this};
		}

		FORCE_INLINE IteratorT begin()
		{
			NodeT* min = const_cast<NodeT*>(getMinNode());
			return IteratorT{min, this};
		}
		/** @} */

		/**
		 * @{
		 * @brief Return an iterator pointing past
		 * the rightmost (max) node of the tree.
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
		 * to the rightmost (max) node of the tree.
		 */
		FORCE_INLINE ConstIteratorT rbegin() const
		{
			NodeT const* max = getMaxNode();
			return ConstIteratorT{max, this};
		}

		FORCE_INLINE IteratorT rbegin()
		{
			NodeT* max = const_cast<NodeT*>(getMaxNode());
			return IteratorT{max, this};
		}
		/** @} */

		/**
		 * @{
		 * @brief Return a reverse iterator pointing
		 * past the leftmost (min) node of the tree.
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
		 * @brief Search for a specific node using
		 * a key.
		 * 
		 * @param key any key that can be compared
		 * with node values
		 * @return ptr to node, or null
		 */
		NodeT const* findNode(auto const& key) const
		{
			NodeT* it = root;
			while (it)
			{
				int32 cmp = CompareT{}(key, it->value);
				if (cmp < 0)
				{
					it = it->left;
				}
				else if (cmp > 0)
				{
					it = it->right;
				}
				else
				{
					// First node found
					return it;
				}
			}
			
			return nullptr;
		}

		/**
		 * @{
		 * @brief Return an iterator pointing to
		 * the first node found that matches the
		 * search key.
		 * 
		 * @param key any key comparable to the
		 * node values
		 * @return an iterator pointing to the
		 * found node, or an iterator equal to
		 * the result of end()
		 */
		FORCE_INLINE ConstIteratorT find(auto const& key) const
		{
			NodeT const* node = findNode(key);
			return ConstIteratorT{node, this};
		}

		FORCE_INLINE IteratorT find(auto const& key)
		{
			NodeT* node = const_cast<NodeT*>(findNode(key));
			return IteratorT{node, this};
		}
		/** @} */

		/**
		 * @brief Insert a new value in the tree.
		 * 
		 * @param createArgs arguments passed to
		 * the constructor of the node
		 * @return an iterator pointing to the
		 * created node
		 */
		IteratorT emplace(auto&& ...createArgs)
		{
			NodeT* node = createNode(FORWARD(createArgs)...);

			if (!root)
			{
				root = node;
				root->color = BinaryNodeColor::Color_BLACK;
			}
			else
			{
				// TODO: Can be moved in a separate function
				NodeT* it = root;
				for (;;)
				{
					int32 cmp = CompareT{}(node->value, it->value);
					if (cmp < 0)
					{
						if (!it->left)
						{
							it->setLeftChild(node);
							break;
						}
						
						it = it->left;
					}
					else
					{
						if (!it->right)
						{
							it->setRightChild(node);
							break;
						}

						it = it->right;
					}
				}
			}
			repairInserted(node);
			numNodes++;

			return IteratorT{node, this};
		}

		/**
		 * @{
		 * @brief Insert a value in the tree.
		 * @see emplace(auto&&...)
		 */
		IteratorT insert(T const& value)
		{
			return emplace(value);
		}

		IteratorT insert(T&& value)
		{
			return emplace(move(value));
		}
		/** @} */

		/**
		 * @brief Remove one node from the tree.
		 * 
		 * @param node ptr to the node to remove
		 * @return the next valid node or null if
		 * tree is empty
		 */
		NodeT* removeNode(NodeT* node)
		{
			// TODO: Check node is in tree

			NodeT* repl = nullptr; // The node used to replace the node to remove
			NodeT* valid = node->parent; // A node which is always valid or null if the tree is empty
			NodeT* next = node->next; // The next valid node or null if the node to remove was the rightmost

			if (node->left && node->right)
			{
				// Swap value with successor, and set
				// the successor as the node to remove
				next = valid = node;
				node = node->next;
				swap(node->value, valid->value);
			}

			if (node->left)
			{
				// Replace with left child
				valid = repl = node->left;
				if ((repl->next = node->next))
				{
					repl->next->prev = repl;
				}
			}
			else if (node->right)
			{
				// Replace with right child
				valid = repl = node->right;
				if ((repl->prev = node->prev))
				{
					repl->prev->next = repl;
				}
			}
			else
			{
				if (node->prev)
				{
					node->prev->next = node->next;
				}
				if (node->next)
				{
					node->next->prev = node->prev;
				}
			}

			if (node->parent)
			{
				if (node->parent->left == node)
				{
					node->parent->left = repl;
				}
				else
				{
					node->parent->right = repl;
				}
				
				if (repl)
				{
					repl->parent = node->parent;
				}
			}
			else if (repl)
			{
				repl->parent = nullptr;
			}
			
			// Repair tree
			repairRemoved(node, repl, node->parent, valid);
			numNodes--;

			// Finally destroy removed node
			destroyNode(node);

			return next;
		}

		/**
		 * @brief Remove the node pointed by the
		 * iterator from the tree, and return an
		 * iterator pointing to the next valid
		 * node.
		 * 
		 * @param it an iterator pointing to a
		 * valid node of the tree
		 * @return an iterator pointing to the next
		 * valid node
		 */
		FORCE_INLINE IteratorT remove(ConstIteratorT const& it)
		{
			NodeT* next = removeNode(const_cast<NodeT*>(it.node));
			return IteratorT{next, this};
		}

	protected:
		/**
		 * @brief Create a new node for the tree.
		 * 
		 * @param createArgs arguments passed to
		 * the value constructor
		 * @return ptr to the created node
		 */
		FORCE_INLINE NodeT* createNode(auto&& ...createArgs)
		{
			// TODO: Replace with allocator
			return new (::malloc(sizeof(NodeT))) NodeT{{FORWARD(createArgs)...}};
		}

		/**
		 * @brief Destroy a node created for this
		 * tree.
		 * 
		 * @param node ptr to node to destroy
		 */
		FORCE_INLINE void destroyNode(NodeT* node)
		{
			node->~NodeT();
			::free(node);
		}

		/**
		 * @brief Destroy the subtree spawning from
		 * the given node.
		 * 
		 * @param node root node of the subtree
		 */
		void destroySubtree(NodeT* node)
		{
			// TODO: Check node is not null

			// Destroy sub-subtrees
			if (node->left) destroySubtree(node->left);
			if (node->right) destroySubtree(node->right);

			destroyNode(node);

			// TODO: Not tail-recursive, we could do
			// better, but it's very simple and elegant
		}

		/**
		 * @brief Destroy the tree and all the nodes
		 * in it.
		 */
		FORCE_INLINE void destroy()
		{
			if (root)
			{
				destroySubtree(root);
				root = nullptr;
			}

			numNodes = 0ull;
		}

		/**
		 * @brief Return true if the tree contains
		 * the given node.
		 * 
		 * @param node ptr to node to test
		 * @return true if node is inside tree
		 */
		FORCE_INLINE bool containsNode(NodeT* node)
		{
			// We can bisect on the value, and then check the pointers
			NodeT* it = root;
			while (it)
			{
				if (node == it) return true;

				int32 cmp = CompareT{}(node->value, it->value);
				if (cmp < 0)
				{
					it = it->left;
				}
				else
				{
					it = it->right;
				}
			}
			
			return false;
		}

		/**
		 * @brief Repair tree after node insertion.
		 * 
		 * @param node ptr to inserted node
		 */
		FORCE_INLINE void repairInserted(NodeT* node)
		{
			Tree_Private::repairInserted(node);
			root = Tree_Private::getRoot(node);
		}

		/**
		 * @brief Repair tree after node removal.
		 * 
		 * @param node ptr to remove node
		 * @param repl ptr to replaced node
		 * @param parent ptr to parent of repl, in
		 * case repl is null
		 * @param valid ptr to a valid node in
		 * the tree, null only if tree is empty
		 */
		FORCE_INLINE void repairRemoved(NodeT* node, NodeT* repl, NodeT* parent, NodeT* valid)
		{
			if (Tree_Private::isBlack(node))
			{
				Tree_Private::repairRemoved(repl, parent);
			}

			root = valid ? Tree_Private::getRoot(valid) : nullptr;
		}

		/// @brief Root node of the tree.
		NodeT* root;

		/// @brief Number of nodes in the tree.
		sizet numNodes;
	};
} // namespace Containers

