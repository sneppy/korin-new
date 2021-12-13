#pragma once

#include "hal/malloc.h"
#include "templates/utility.h"
#include "containers_types.h"
#include "tree_node.h"

namespace Korin
{
	/**
	 * @brief Payload of a binary tree node.
	 *
	 * @tparam T the type of the node's value
	 */
	template<typename T>
	struct BinaryNodePayload
	{
		/* The value contained in the node. */
		T value;

		/**
		 * @brief Copy another value.
		 *
		 * @param inValue value to copy
		 */
		constexpr FORCE_INLINE BinaryNodePayload(T const& inValue)
			: value{inValue}
		{
			//
		}

		/**
		 * @brief Move another value.
		 *
		 * @param inValue value to move
		 */
		constexpr FORCE_INLINE BinaryNodePayload(T&& inValue)
			: value{move(inValue)}
		{
			//
		}

		/**
		 * @brief Returns a ref to the contained value.
		 * @{
		 */
		constexpr FORCE_INLINE T const& operator*() const
		{
			return value;
		}

		constexpr FORCE_INLINE T& operator*()
		{
			return value;
		}
		/** @} */

		/**
		 * @brief Returns a ptr to the contained value
		 * in order to access its members.
		 * @{
		 */
		constexpr FORCE_INLINE T const* operator->() const
		{
			return &(**this);
		}

		constexpr FORCE_INLINE T* operator->()
		{
			return &(**this);
		}
		/** @} */
	};

	/**
	 * @brief The node of a red and black tree.
	 *
	 * @tparam T the type of the node's value
	 */
	template<typename T>
	using BinaryNode = BinaryNodeBase<BinaryNodePayload<T>>;

	/**
	 * @brief Iterator class used to iterate
	 * over a read-only binary tree.
	 *
	 * @tparam T the type of the node value
	 */
	template<typename T>
	struct TreeConstIterator
	{
		template<typename, typename> friend class Tree;

		using NodeT = BinaryNode<T>;
		using RefT = T const&;
		using PtrT = T const*;

		/**
		 * @brief Cosntruct a new iterator pointing to
		 * the given node.
		 *
		 * @param inNode the node pointed by the iterator
		 * @param inTreeId the address of the tree (only
		 * needed for debug checks)
		 */
		FORCE_INLINE TreeConstIterator(NodeT* inNode = nullptr, [[maybe_unused]] void* inTreeId = nullptr)
			: node{inNode}
#if !KORIN_RELEASE
			, treeId{inTreeId}
#endif
		{
			//
		}

		/**
		 * @brief Dereference pointer, returns the
		 * value of the pointed node.
		 */
		FORCE_INLINE RefT operator*() const
		{
			return node->value;
		}

		/**
		 * @brief Returns a ptr to the value of the
		 * pointed node, for member access.
		 */
		FORCE_INLINE PtrT operator->() const
		{
			return &(**this);
		}

		/**
		 * @brief Compare two iterators, returns true
		 * if they point to the same node.
		 *
		 * Both iterators should belong to the same
		 * tree.
		 *
		 * @param other another tree iterator
		 * @return true if the point to the same node
		 * @return false otherwise
		 */
		FORCE_INLINE bool operator==(TreeConstIterator const& other) const
		{
			CHECKF(treeId == other.treeId, "Comparing iterators from different trees")
			return node == other.node;
		}

		/**
		 * @brief Compare two iterators, returns true
		 * if they point to the same node.
		 *
		 * Both iterators should belong to the same
		 * tree.
		 *
		 * @param other another tree iterator
		 * @return true if the point to different nodes
		 * @return false otherwise
		 */
		FORCE_INLINE bool operator!=(TreeConstIterator const& other) const
		{
			return !(*this == other);
		}

		/**
		 * @brief Move forward iterator by one step,
		 * and returns ref to self.
		 */
		FORCE_INLINE TreeConstIterator& operator++()
		{
			node = node->next;
			return *this;
		}

		/**
		 * @brief Move iterator forward by one step,
		 * and return iterator before increment.
		 */
		FORCE_INLINE TreeConstIterator operator++(int32)
		{
			TreeConstIterator copy{*this};
			operator++();
			return copy;
		}

		/**
		 * @brief Move iterator backward by one step,
		 * and returns ref to self.
		 */
		FORCE_INLINE TreeConstIterator& operator--()
		{
			node = node->prev;
			return *this;
		}

		/**
		 * @brief Move iterator backward by one step,
		 * and returns ref to self.
		 */
		FORCE_INLINE TreeConstIterator operator--(int32)
		{
			TreeConstIterator copy{*this};
			operator--();
			return copy;
		}

	private:
		/* Ptr to the current node. */
		NodeT* node;

#if !KORIN_RELEASE
		/* Address of the tree this iterator belongs to. */
		void* treeId;
#endif
	};

	template<typename T>
	struct TreeIterator
	{
		template<typename, typename> friend class Tree;

		using NodeT = BinaryNode<T>;
		using RefT = T&;
		using PtrT = T*;

		/**
		 * @brief Cosntruct a new iterator pointing to
		 * the given node.
		 *
		 * @param inNode the node pointed by the iterator
		 * @param inTreeId the address of the tree (only
		 * needed for debug checks)
		 */
		FORCE_INLINE TreeIterator(NodeT* inNode = nullptr, [[maybe_unused]] void* inTreeId = nullptr)
			: node{inNode}
#if !KORIN_RELEASE
			, treeId{inTreeId}
#endif
		{
			//
		}

		/**
		 * @brief Dereference pointer, returns the
		 * value of the pointed node.
		 */
		FORCE_INLINE RefT operator*() const
		{
			return node->value;
		}

		/**
		 * @brief Returns a ptr to the value of the
		 * pointed node, for member access.
		 */
		FORCE_INLINE PtrT operator->() const
		{
			return &(**this);
		}

		/**
		 * @brief Compare two iterators, returns true
		 * if they point to the same node.
		 *
		 * Both iterators should belong to the same
		 * tree.
		 *
		 * @param other another tree iterator
		 * @return true if the point to the same node
		 * @return false otherwise
		 */
		FORCE_INLINE bool operator==(TreeIterator const& other) const
		{
			CHECKF(treeId == other.treeId, "Comparing iterators from different trees")
			return node == other.node;
		}

		/**
		 * @brief Compare two iterators, returns true
		 * if they point to the same node.
		 *
		 * Both iterators should belong to the same
		 * tree.
		 *
		 * @param other another tree iterator
		 * @return true if the point to different nodes
		 * @return false otherwise
		 */
		FORCE_INLINE bool operator!=(TreeIterator const& other) const
		{
			return !(*this == other);
		}

		/**
		 * @brief Move forward iterator by one step,
		 * and returns ref to self.
		 */
		FORCE_INLINE TreeIterator& operator++()
		{
			node = node->next;
			return *this;
		}

		/**
		 * @brief Move iterator forward by one step,
		 * and return iterator before increment.
		 */
		FORCE_INLINE TreeIterator operator++(int32)
		{
			TreeIterator copy{*this};
			operator++();
			return copy;
		}

		/**
		 * @brief Move iterator backward by one step,
		 * and returns ref to self.
		 */
		FORCE_INLINE TreeIterator& operator--()
		{
			node = node->prev;
			return *this;
		}

		/**
		 * @brief Move iterator backward by one step,
		 * and returns ref to self.
		 */
		FORCE_INLINE TreeIterator operator--(int32)
		{
			TreeIterator copy{*this};
			operator--();
			return copy;
		}

		FORCE_INLINE operator TreeConstIterator<T>() const
		{
#if !KORIN_RELEASE
			return {node, treeId};
#else
			return {node};
#endif
		}

	private:
		/* Ptr to the current node. */
		NodeT* node;

#if !KORIN_RELEASE
		/* Address of the tree this iterator belongs to. */
		void* treeId;
#endif
	};

	/**
	 * @brief Implements a fully managed binary
	 * tree with red and black balancing algo.
	 *
	 * A user-provided policy determines the
	 * order of the node in the tree. The policy
	 * accepts two values and return a positive
	 * value if the lhs value is greater, a
	 * negative value if the lhs is less or zero
	 * otherwise.
	 *
	 * @see GreaterThan.
	 *
	 * @tparam T the type of the nodes' value
	 * @tparam PolicyT the policy used to populate
	 * and search the tree
	 */
	template<typename T, typename PolicyT>
	class Tree
	{
	public:
		using NodeT = BinaryNode<T>;
		using IteratorT = TreeIterator<T>;
		using ConstIteratorT = TreeConstIterator<T>;

		/**
		 * @brief Construct a new empty tree.
		 */
		FORCE_INLINE Tree()
			: root{nullptr}
			, numNodes{0}
		{
			//
		}

		/**
		 * @brief Construct a new tree by copying
		 * another tree.
		 *
		 * @param other tree to copy
		 */
		FORCE_INLINE Tree(Tree const& other)
			: root{nullptr}
			, numNodes{other.numNodes}
		{
			if (other.root)
			{
				// Clone tree structure
				root = createNode(other.root->value);
				cloneSubtree(root, other.root);
			}
		}

		/**
		 * @brief Construct a new tree by moving
		 * another tree.
		 *
		 * @param other tree to move
		 */
		FORCE_INLINE Tree(Tree&& other)
			: root{other.root}
			, numNodes{other.numNodes}
		{
			other.root = nullptr;
			other.numNodes = 0;
		}

		/**
		 * @brief Copy another tree.
		 *
		 * @param other tree to copy
		 * @return ref to self
		 */
		FORCE_INLINE Tree& operator=(Tree const& other)
		{
			if (other.root)
			{
				if (!root)
				{
					// Create root node
					root = createNode(other.root->value);
				}

				// Copy over existing tree structure
				copySubtree(root, other.root);
			}

			numNodes = other.numNodes;

			return *this;
		}

		/**
		 * @brief Move another tree.
		 *
		 * @param other tree to move
		 * @return ref to self
		 */
		FORCE_INLINE Tree& operator=(Tree&& other)
		{
			// Destroy this tree
			destroy();

			root = other.root;
			numNodes = other.numNodes;

			other.root = nullptr;
			other.numNodes = 0;

			return *this;
		}

		/**
		 * @brief Destroy this tree and all the nodes
		 * it contains.
		 */
		FORCE_INLINE ~Tree()
		{
			// Destroy tree
			destroy();
		}

		/**
		 * @brief Returns the number of nodes in
		 * this tree.
		 */
		FORCE_INLINE sizet getNumNodes() const
		{
			return numNodes;
		}

		/**
		 * @brief Returns an iterator pointing to the
		 * min/leftmost node of the tree.
		 * @{
		 */
		FORCE_INLINE IteratorT begin()
		{
			NodeT* min = root ? TreeNode::getMin(root) : root;
			return {min, this};
		}

		FORCE_INLINE ConstIteratorT begin() const
		{
			return const_cast<Tree*>(this)->begin();
		}
		/** @} */

		/**
		 * @brief Returns an iterator that points past
		 * the max/rightmost node of the tree.
		 * @{
		 */
		FORCE_INLINE IteratorT end()
		{
			return {nullptr, this};
		}

		FORCE_INLINE ConstIteratorT end() const
		{
			return const_cast<Tree*>(this)->end();
		}
		/** @} */

		/**
		 * @brief Returns an iterator pointing to
		 * the first item with the given key.
		 *
		 * If no item is found, returns the end
		 * iterator.
		 *
		 * @param key key used to find item
		 * @return iterator pointing to first item
		 * or end iterator
		 * @{
		 */
		FORCE_INLINE IteratorT begin(auto const& key)
		{
			NodeT* lb = TreeNode::lowerBound(root, [key](const auto* node) {

				return PolicyT{}(key, node->value);
			});
			return {lb, this};
		}

		FORCE_INLINE ConstIteratorT begin(auto const& key) const
		{
			return const_cast<Tree*>(this)->begin();
		}
		/** @} */

		/**
		 * @brief Returns an iterator pointing
		 * beyond the last item with the given
		 * key.
		 *
		 * If not such item exists, returns the
		 * end iterator.
		 *
		 * @param key the key used to search
		 * @return iterator pointing beyond the
		 * last item, or end iterator
		 * @{
		 */
		FORCE_INLINE IteratorT end(auto const& key)
		{
			NodeT* ub = TreeNode::upperBound(root, [key](const auto* node) {

				return PolicyT{}(key, node->value);
			});
			return {ub ? ub->next : ub, this};
		}

		FORCE_INLINE ConstIteratorT end(auto const& key) const
		{
			return const_cast<Tree*>(this)->end(key);
		}
		/** @} */

		/**
		 * @brief Returns an iterator that points to
		 * the first node that matches the search key.
		 *
		 * @param key
		 * @return iter that points to first node found
		 * @return iter that points to end of tree if
		 * no node was found
		 */
		FORCE_INLINE IteratorT find(auto const& key)
		{
			NodeT* node = TreeNode::find(root, [&key](auto const* node) {

				return PolicyT{}(key, node->value);
			});

			return {node, this};
		}

		FORCE_INLINE ConstIteratorT find(auto const& key) const
		{
			return const_cast<Tree*>(this)->find(key);
		}
		/** @} */

		/**
		 * @brief Costruct and insert a new node in the
		 * tree.
		 *
		 * @param createArgs arguments passed to construct
		 * the node value
		 * @return iter that points to inserted node
		 */
		FORCE_INLINE IteratorT emplace(auto&& ...createArgs)
		{
			NodeT* newNode = createNode(FORWARD(createArgs)...);
			root = TreeNode::insert(root, newNode, [newNode](auto const* node) {

				return PolicyT{}(newNode->value, node->value);
			});

			numNodes++;

			return {newNode, this};
		}

		/**
		 * @brief Insert a new node with the given value
		 * in the tree.
		 *
		 * @param value value to insert
		 * @return iter that points to inserted node
		 */
		FORCE_INLINE IteratorT insert(T const& value)
		{
			return emplace(value);
		}

		FORCE_INLINE IteratorT insert(T&& value)
		{
			return emplace(move(value));
		}
		/** @} */

		/**
		 * @brief Construct a new node with the given
		 * arguments.
		 *
		 * If a node with the same key exists, that
		 * node is replaced with the new one.
		 *
		 * @param createArgs arguments used to construct
		 * the node
		 * @return iterator pointing to constructed node
		 */
		IteratorT emplaceUnique(auto&& ...createArgs)
		{
			// Create the new node
			NodeT* node = createNode(FORWARD(createArgs)...);
			NodeT* found = node;
			root = TreeNode::insertUnique(root, found, [node](auto const* other) {

				return PolicyT{}(node->value, other->value);
			});

			if (found != node)
			{
				// Existing node replaced, destroy new node
				destroyNode(node);
			}
			else
				// Update number of nodes after insertion
				numNodes++;

			return {found, this};
		}

		/**
		 * @brief Insert a new node with the given value.
		 *
		 * If a node with the same key exists, the
		 * existing node is replaced with the new one.
		 *
		 * @param value the value to insert
		 * @return iterator pointing to inserted node
		 * @{
		 */
		FORCE_INLINE IteratorT insertUnique(T const& value)
		{
			return emplaceUnique(value);
		}

		FORCE_INLINE IteratorT insertUnique(T&& value)
		{
			return emplaceUnique(move(value));
		}
		/** @} */

		/**
		 * @brief Construct a new item and inserts it into
		 * the tree.
		 *
		 * If an item with the same key already exists, the
		 * item is not inserted and an iterator pointing to
		 * the existing item is returned instead. Note that
		 * the item is constructed in any case, so it will
		 * be constructed and destroyed immediately if a
		 * duplicate exists.
		 *
		 * @param createArgs the arguments used to contruct
		 * the item
		 * @return iterator pointing to new node or to
		 * existing node
		 */
		IteratorT findOrEmplace(auto&& ...createArgs)
		{
			// Create new node, will de destroyed if key already exists
			NodeT* node = createNode(FORWARD(createArgs)...);
			NodeT* found = node;
			root = TreeNode::findOrInsert(root, found, [node](auto const* other) {

				return PolicyT{}(node->value, other->value);
			});

			if (found != node)
			{
				// Node already exists, destroy new node
				destroyNode(node);
			}
			else
				// Update number of nodes
				numNodes++;

			return {found, this};
		}

		/**
		 * @brief Remove the node pointed by the
		 * given iterator from the tree.
		 *
		 * @param it iter that points to node to
		 * remove
		 * @return iter that points to next valid
		 * node
		 */
		FORCE_INLINE IteratorT remove(ConstIteratorT it)
		{
			// TODO: Move forward and return iterator
			ASSERT(it->node != nullptr);

			NodeT* node = it.node;
			NodeT* next = node->next;
			root = TreeNode::remove(node, next);

			destroyNode(node);
			numNodes--;

			return {next, this};
		}

	protected:
		/* The root node of the tree. */
		NodeT* root;

		/* The number of nodes in the tree. */
		sizet numNodes;

	private:
		/**
		 * @brief Create a new node.
		 *
		 * @param createArgs arguments used to create
		 * the node
		 * @return ptr to created node
		 */
		FORCE_INLINE NodeT* createNode(auto&& ...createArgs)
		{
			// TODO: Use allocator
			void* mem = gMalloc->malloc(sizeof(NodeT));
			return new(mem) NodeT{{FORWARD(createArgs)...}};
		}

		/**
		 * @brief Destroy a node of the tree.
		 *
		 * @param node ptr to node to destroy
		 */
		FORCE_INLINE void destroyNode(NodeT* node)
		{
			// TODO: Use allocator
			ASSERT(node != nullptr)
			node->~NodeT();
			gMalloc->free(node);
		}

		/**
		 * @brief Recursively clone a subtree.
		 *
		 * @param dst root of new subtree
		 * @param src root of the source subtree
		 */
		void cloneSubtree(NodeT* dst, NodeT* src)
		{
			ASSERT(src != nullptr)

			if (src->left)
			{
				// Create node
				auto* left = createNode(src->left->value);
				left->color = src->left->color;
				TreeNode::Impl::insertLeft(dst, left);

				// Clone left subtree
				cloneSubtree(left, src->left);
			}

			if (src->right)
			{
				// Create node
				auto* right = createNode(src->right->value);
				right->color = src->right->color;
				TreeNode::Impl::insertRight(dst, right);

				// Clone right subtree
				cloneSubtree(right, src->right);
			}
		}

		/**
		 * @brief Recursively copy a subtree over the
		 * existing subtree.
		 *
		 * @param dst root of existing subtree
		 * @param src root of subtree to copy
		 */
		void copySubtree(NodeT* dst, NodeT* src)
		{
			ASSERT(src != nullptr)

			if (src->left)
			{
				auto* left = dst->left;

				if (left)
				{
					// Copy value
					left->value = src->left->value;
					left->color = src->left->color;
				}
				else
				{
					// Create node and insert it
					left = createNode(src->left->value);
					left->color = src->left->color;
					TreeNode::Impl::insertLeft(dst, left);
				}

				copySubtree(dst->left, src->left);
			}
			// FIXME: if dst->left remove it

			if (src->right)
			{
				auto* right = dst->right;

				if (right)
				{
					// Copy value
					right->value = src->right->value;
					right->color = src->right->color;
				}
				else
				{
					// Create node and insert it
					right = createNode(src->right->value);
					right->color = src->right->color;
					TreeNode::Impl::insertRight(dst, right);
				}

				copySubtree(dst->right, src->right);
			}
			// FIXME: if dst->right remove it
		}

		/**
		 * @brief Recursively destroy all the nodes in
		 * the subtree.
		 *
		 * @param root root node of the subtree
		 */
		void destroySubtree(NodeT* root)
		{
			// Recursion is fine, tree height is log2(n)
			ASSERT(root != nullptr)

			if (root->left)
			{
				destroySubtree(root->left);
			}

			if (root->right)
			{
				destroySubtree(root->right);
			}

			destroyNode(root);
		}

		/**
		 * @brief Destroy this tree.
		 */
		FORCE_INLINE void destroy()
		{
			if (root)
			{
				destroySubtree(root);
				root = nullptr;
			}

			numNodes = 0;
		}
	};
} // namespace Korin
