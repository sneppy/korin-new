#pragma once

#include "hal/malloc.h"
#include "templates/utility.h"
#include "containers_types.h"
#include "tree_node.h"

namespace Korin
{
	template<typename T>
	struct BinaryNodePayload
	{
		/* The value contained in the node. */
		T value;

		constexpr FORCE_INLINE BinaryNodePayload(T const& inValue)
			: value{inValue}
		{
			//
		}

		constexpr FORCE_INLINE BinaryNodePayload(T&& inValue)
			: value{move(inValue)}
		{
			//
		}

		constexpr FORCE_INLINE T const& operator*() const
		{
			return value;
		}

		constexpr FORCE_INLINE T& operator*()
		{
			return value;
		}

		constexpr FORCE_INLINE T const* operator->() const
		{
			return &(**this);
		}

		constexpr FORCE_INLINE T* operator->()
		{
			return &(**this);
		}
	};

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
		friend Tree<T>;

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
		friend Tree<T>;

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

	template<typename T, typename PolicyT>
	class Tree
	{
	public:
		using NodeT = BinaryNode<T>;
		using IteratorT = TreeIterator<T>;
		using ConstIteratorT = TreeConstIterator<T>;

		FORCE_INLINE Tree()
			: root{nullptr}
			, numNodes{0}
		{
			//
		}

		FORCE_INLINE Tree(Tree const& other)
			: root{nullptr}
			, numNodes{other.numNodes}
		{
			if (other.root)
			{
				// Clone tree structure
				root = cloneSubtree(other.root);
			}
		}

		FORCE_INLINE Tree(Tree&& other)
			: root{other.root}
			, numNodes{other.numNodes}
		{
			other.root = nullptr;
			other.numNodes = 0;
		}

		FORCE_INLINE Tree& operator=(Tree const& other)
		{
			if (other.root)
			{
				// Copy over existing tree structure
				root = copySubtree(root, other.root);
			}

			numNodes = other.numNodes;
		}

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

		FORCE_INLINE ~Tree()
		{
			// Destroy tree
			destroy();
		}

		FORCE_INLINE sizet getNumNodes() const
		{
			return numNodes;
		}

		FORCE_INLINE IteratorT begin()
		{
			NodeT* min = TreeNode::getMin(root);
			return {min, this};
		}

		FORCE_INLINE ConstIteratorT begin() const
		{
			return const_cast<Tree*>(this)->begin();
		}

		FORCE_INLINE IteratorT end()
		{
			return {nullptr, this};
		}

		FORCE_INLINE ConstIteratorT end() const
		{
			return const_cast<Tree*>(this)->end();
		}

		FORCE_INLINE IteratorT begin(auto const& key);
		FORCE_INLINE ConstIteratorT begin(auto const& key) const
		{
			return const_cast<Tree*>(this)->begin();
		}

		FORCE_INLINE IteratorT end(auto const& key);
		FORCE_INLINE ConstIteratorT end(auto const& key) const
		{
			return const_cast<Tree*>(this)->end(key);
		}

		FORCE_INLINE IteratorT rbegin()
		{
			NodeT* max = TreeNode::getMax(root);
			return {max, this};
		}

		FORCE_INLINE ConstIteratorT rbegin() const
		{
			return const_cast<Tree*>(this)->rbegin();
		}

		FORCE_INLINE IteratorT rend()
		{
			return {nullptr, this};
		}

		FORCE_INLINE ConstIteratorT rend() const
		{
			return const_cast<Tree*>(this)->rend();
		}

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

		FORCE_INLINE IteratorT emplace(auto&& ...createArgs)
		{
			NodeT* newNode = createNode(FORWARD(createArgs)...);
			root = TreeNode::insert(root, newNode, [newNode](auto const* node) {

				return PolicyT{}(newNode->value, node->value);
			});

			numNodes++;

			return {newNode, this};
		}

		FORCE_INLINE IteratorT insert(T const& value)
		{
			return emplace(value);
		}

		FORCE_INLINE IteratorT insert(T&& value)
		{
			return emplace(move(value));
		}

		FORCE_INLINE IteratorT remove(ConstIteratorT it)
		{
			// TODO: Move forward and return iterator
			ASSERT(it->node != nullptr);

			NodeT* node = it.node;
			NodeT* next = node->next;
			root = TreeNode::remove(node);

			if (node != it.node)
			{
				// We removed the successor
				next = it.node;
			}

			destroyNode(node);
			numNodes--;

			return {next, this};
		}

	protected:
		NodeT const* getRoot() const
		{
			return root;
		}

		NodeT* getRoot()
		{
			return root;
		}

		NodeT* root;
		sizet numNodes;

	private:
		FORCE_INLINE NodeT* createNode(auto&& ...createArgs)
		{
			// TODO: Use allocator
			return new(gMalloc->malloc(sizeof(NodeT))) NodeT{FORWARD(createArgs)...};
		}

		FORCE_INLINE void destroyNode(NodeT* node)
		{
			// TODO: Use allocator
			ASSERT(node != nullptr)
			node->~NodeT();
			gMalloc->free(node);
		}

		FORCE_INLINE NodeT* cloneSubtree(NodeT* src)
		{
			ASSERT(src != nullptr)

			NodeT* dst = createNode(src->value);
			dst->color = src->color;

			if (src->left)
			{
				auto* left = cloneSubtree(src->left);
				TreeNode::Impl::insertLeft(dst, left);
			}

			if (src->right)
			{
				auto* right = cloneSubtree(src->right);
				TreeNode::Impl::insertRight(dst, right);
			}

			return dst;
		}

		FORCE_INLINE NodeT* copySubtree(NodeT* dst, NodeT* src)
		{
			ASSERT(src != nullptr)

			if (dst)
			{
				dst->value = src->value;
			}
			else
			{
				dst = createNode(src->value);
			}
			
			// Copy node color
			dst->color = src->color;

			if (src->left)
			{
				// Copy left subtree
				auto* left = copySubtree(dst->left, src->left);
				ASSERT(left == dst->left || dst->left == nullptr)

				if (!dst->left)
				{
					TreeNode::Impl::insertLeft(dst, left);
				}
			}

			if (src->right)
			{
				// Copy right subtree
				auto* right = copySubtree(dst->right, src->right);
				ASSERT(right == dst->right || dst->right == nullptr)

				if (!dst->right)
				{
					TreeNode::Impl::insertRight(dst, right);
				}
			}

			return dst;
		}

		FORCE_INLINE void destroySubtree(NodeT* root)
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
