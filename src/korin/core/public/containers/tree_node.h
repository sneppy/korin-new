#pragma once

#include "containers_types.h"
#include "templates/enable_if.h"
#include "templates/ordering.h"
#include "templates/utility.h"

#define ENABLE_IF_NODE_T(T) typename EnableIf<IsBaseOf<BinaryNodeBase<T>, T>::value>::Type

namespace Korin
{
	/**
	 * @brief The color of a red-black tree node.
	 */
	enum class BinaryNodeColor
	{
		Color_RED,
		Color_BLACK
	};

	/**
	 * @brief Base class for RB tree nodes.
	 * 
	 * @tparam NodeT the node type, must be
	 * a subclass of this class
	 */
	template<typename NodeT>
	struct BinaryNodeBase
	{
		/// @brief Pointer to the parent node.
		NodeT* parent = nullptr;

		/// @brief Pointer to the left child.
		NodeT* left = nullptr;

		/// @brief Pointer to the right child.
		NodeT* right = nullptr;

		/// @brief Pointer to the next-in-order node.
		NodeT* next = nullptr;

		/// @brief Pointer to the previous-in-order node.
		NodeT* prev = nullptr;

		/// @brief The color of the node.
		BinaryNodeColor color = BinaryNodeColor::Color_RED;

		/**
		 * @brief Set the left child of this
		 * node.
		 * 
		 * @param other ptr to node to set
		 */
		constexpr FORCE_INLINE void setLeftChild(NodeT* other)
		{
			// TODO: Check that left is null, otherwise something's probably wrong
			if ((left = other))
			{
				left->parent = reinterpret_cast<NodeT*>(this);
				left->next = reinterpret_cast<NodeT*>(this);
				if ((left->prev = prev))
				{
					left->prev->next = left;
				}
				prev = left;
			}
		}

		/**
		 * @brief Set the right child of this
		 * node.
		 * 
		 * @param other ptr to node to set
		 */
		constexpr FORCE_INLINE void setRightChild(NodeT* other)
		{
			// TODO: Check that right is null, otherwise something's probably wrong
			if ((right = other))
			{
				right->parent = reinterpret_cast<NodeT*>(this);
				right->prev = reinterpret_cast<NodeT*>(this);
				if ((right->next = next))
				{
					right->next->prev = right;
				}
				next = right;
			}
		}
	};

	namespace TreeNode
	{
		/**
		 * @brief Return true if the node is not
		 * null and is red.
		 * 
		 * @param node ptr to node to test
		 */
		template<typename NodeT, typename = ENABLE_IF_NODE_T(NodeT)>
		constexpr FORCE_INLINE bool isRed(BinaryNodeBase<NodeT> const* node)
		{
			return node && node->color == BinaryNodeColor::Color_RED;
		}

		/**
		 * @brief Return true if the node is null
		 * or is black.
		 * 
		 * @param node ptr to node to test
		 */
		template<typename NodeT, typename = ENABLE_IF_NODE_T(NodeT)>
		constexpr FORCE_INLINE bool isBlack(BinaryNodeBase<NodeT> const* node)
		{
			return !isRed(node);
		}
	
		namespace Impl
		{
			/**
			 * @brief Set node as left child of
			 * another node.
			 * 
			 * @param parent ptr to parent node
			 * @param node ptr to node to set as
			 * left child
			 */
			template<typename NodeT, typename = ENABLE_IF_NODE_T(NodeT)>
			constexpr FORCE_INLINE void setLeftChild(NodeT* parent, NodeT* node)
			{
				ASSERT(parent)
				CHECK(!parent->left)

				if ((parent->left = node))
				{
					node->parent = parent;
					node->next = parent;
					if ((node->prev = parent->prev))
					{
						node->prev->next = node;
					}
					parent->prev = node;
				}
			}

			/**
			 * @brief Set node as left child of
			 * another node.
			 * 
			 * @param parent ptr to parent node
			 * @param node ptr to node to set as
			 * left child
			 */
			template<typename NodeT, typename = ENABLE_IF_NODE_T(NodeT)>
			constexpr FORCE_INLINE void setRightChild(NodeT* parent, NodeT* node)
			{
				ASSERT(parent)
				CHECK(!parent->right)

				if ((parent->right = node))
				{
					node->parent = parent;
					node->prev = parent;
					if ((node->next = parent->next))
					{
						node->next->prev = node;
					}
					parent->next = node;
				}
			}

			/**
			 * @brief 
			 * 
			 */
			template<typename NodeT, typename = ENABLE_IF_NODE_T(NodeT)>
			constexpr void swapNodes(NodeT* node, NodeT* other)
			{
				// Swap parents
				swap(node->parent, other->parent);
				if (node->parent)
				{
					if (node->parent->right == other)
						node->parent->right = node;
					else
						node->parent->left = node;
				}
				if (other->parent)
				{
					if (other->parent->right == node)
						other->parent->right = other;
					else
						other->parent->left = other;
				}

				// Swap left children
				swap(node->left, other->left);
				if (node->left)
					node->left->parent = node;
				if (other->left)
					other->left->parent = other;
				
				// Swap right children
				swap(node->right, other->right);
				if (node->right)
					node->right->parent = node;
				if (other->right)
					other->right->parent = other;
			}

			/**
			 * @brief 
			 * 
			 */
			template<typename NodeT, typename = ENABLE_IF_NODE_T(NodeT)>
			constexpr NodeT* replaceNode(NodeT* node)
			{
				ASSERT(node != nullptr)

				NodeT* repl = nullptr;

				if ((repl = node->left))
				{
					// Replace with left child
					if ((repl->next = node->next))
						repl->next->prev = repl;
				}
				else if ((repl = node->right))
				{
					// Replace with right child
					if ((repl->prev = node->prev))
						repl->prev->next = repl;
				}
				else
				{
					// Just evict from tree

					if (node->prev)
						node->prev->next = node->next;
					
					if (node->next)
						node->next->prev = node->prev;
				}

				if (auto* parent = node->parent)
				{
					if (parent->left == node)
						parent->left = repl;
					else
						parent->right = repl;
				}
				if (repl)
					repl->parent = node->parent;
				
				return repl;
			}

			/**
			 * @brief Rotate left around pivot node.
			 * 
			 * @param pivot ptr to pivot node
			 */
			template<typename NodeT, typename = ENABLE_IF_NODE_T(NodeT)>
			constexpr void rotateLeft(NodeT* pivot)
			{
				// TODO: Check pivot is not null

				auto* grand = pivot->parent;
				auto* node = pivot->right;
				auto* child = node->left;
				
				pivot->parent = node;
				node->parent = grand;
				if (grand)
				{
					grand->left == pivot
						? grand->left = node
						: grand->right = node;
				}

				node->left = pivot;
				pivot->right = child;
				if (child)
				{
					child->parent = pivot;
				}
			}
			/**
			 * @brief Rotate right around pivot node.
			 * 
			 * @param pivot ptr to pivot node
			 */
			template<typename NodeT, typename = ENABLE_IF_NODE_T(NodeT)>
			constexpr void rotateRight(NodeT* pivot)
			{
				// TODO: Check pivot is not null

				auto* grand = pivot->parent;
				auto* node = pivot->left;
				auto* child = node->right;
				
				pivot->parent = node;
				node->parent = grand;
				if (grand)
				{
					grand->right == pivot
						? grand->right = node
						: grand->left = node;
				}

				node->right = pivot;
				pivot->left = child;
				if (child)
				{
					child->parent = pivot;
				}
			}

			/**
			 * @brief Repair inserted node.
			 */
			template<typename NodeT, typename = ENABLE_IF_NODE_T(NodeT)>
			constexpr void repairInserted(NodeT* node)
			{
				// TODO: Check node is red

				if (!node)
					; // Nothing to do
				else if (!node->parent)
				{
					// Node is root, set black
					node->color = BinaryNodeColor::Color_BLACK;
				}
				else if (isBlack(node->parent))
					; // Node is already red
				else
				{
					auto* parent = node->parent;
					auto* grand = parent->parent;
					auto* uncle = grand
						? (grand->left == parent ? grand->right : grand->left)
						: nullptr;
					
					if (isRed(uncle))
					{
						uncle->color = parent->color = BinaryNodeColor::Color_BLACK;
						grand->color = BinaryNodeColor::Color_RED;

						// Recursive call on grandparent
						repairInserted(grand);
					}
					else // If uncle is black
					{
						if (grand->left == parent)
						{
							if (parent->right == node)
							{
								rotateLeft(parent);
								swap(node, parent);
							}
								
							rotateRight(grand);

							parent->color = BinaryNodeColor::Color_BLACK;
							grand->color = BinaryNodeColor::Color_RED;
						}
						else
						{
							if (parent->left == node)
							{
								rotateRight(parent);
								swap(node, parent);
							}
								
							rotateLeft(grand);

							parent->color = BinaryNodeColor::Color_BLACK;
							grand->color = BinaryNodeColor::Color_RED;
						}
					}
				}
			}

			/**
			 * @brief Repair red-black tree after a
			 * node removal.
			 * 
			 * @param node ptr to replaced node
			 * @param parent parent of removed node
			 */
			template<typename NodeT, typename = ENABLE_IF_NODE_T(NodeT)>
			constexpr void repairRemoved(NodeT* node, NodeT* parent)
			{
				// TODO: Add some comments

				if (!node && !parent)
					;
				else if (isRed(node) || !parent)
				{
					// Make node black
					node->color = BinaryNodeColor::Color_BLACK;
				}
				else if (parent->left == node)
				{
					auto* sibling = parent->right;

					if (isRed(sibling))
					{
						sibling->color = BinaryNodeColor::Color_BLACK;
						parent->color = BinaryNodeColor::Color_RED;

						rotateLeft(parent);
						sibling = parent->right;
					}

					if (isBlack(sibling) && isBlack(sibling->left) && isBlack(sibling->right))
					{
						sibling->color = BinaryNodeColor::Color_RED;
						repairRemoved(parent, parent->parent);
					}
					else
					{
						if (isRed(sibling->left))
						{
							sibling->color = BinaryNodeColor::Color_RED;
							sibling->left->color = BinaryNodeColor::Color_BLACK;

							rotateRight(sibling);
							sibling = sibling->parent;
						}

						sibling->color = parent->color;
						parent->color = BinaryNodeColor::Color_BLACK;
						sibling->right->color = BinaryNodeColor::Color_BLACK;

						rotateLeft(parent);
					}
				}
				else
				{
					auto* sibling = parent->left;

					if (isRed(sibling))
					{
						sibling->color = BinaryNodeColor::Color_BLACK;
						parent->color = BinaryNodeColor::Color_RED;

						rotateRight(parent);
						sibling = parent->left;
					}

					if (isBlack(sibling) && isBlack(sibling->right) && isBlack(sibling->left))
					{
						sibling->color = BinaryNodeColor::Color_RED;
						repairRemoved(parent, parent->parent);
					}
					else
					{
						if (isRed(sibling->right))
						{
							sibling->color = BinaryNodeColor::Color_RED;
							sibling->right->color = BinaryNodeColor::Color_BLACK;

							rotateLeft(sibling);
							sibling = sibling->parent;
						}

						sibling->color = parent->color;
						parent->color = BinaryNodeColor::Color_BLACK;
						sibling->left->color = BinaryNodeColor::Color_BLACK;

						rotateRight(parent);
					}
				}
			}
		} // namespace Impl
		
		/**
		 * @brief Return the root node of the tree
		 * that contains the given node.
		 * 
		 * @param node a node in the tree
		 * @return ptr to root
		 */
		template<typename NodeT, typename = ENABLE_IF_NODE_T(NodeT)>
		constexpr FORCE_INLINE NodeT* getRoot(NodeT* node)
		{
			ASSERT(node != nullptr)
			while (node->parent) node = node->parent;
			return node;
		}

		/**
		 * @brief Return the leftmost node of the tree.
		 * 
		 * @param root the root of the tree
		 * @return ptr to leftmost node
		 */
		template<typename NodeT, typename = ENABLE_IF_NODE_T(NodeT)>
		constexpr FORCE_INLINE NodeT* getMin(NodeT* root)
		{
			ASSERT(root != nullptr)
			while (root->left) root = root->left;
			return root;
		}

		/**
		 * @brief Return the rightmost node of the tree.
		 * 
		 * @param root the root of the tree
		 * @return ptr to rightmost node
		 */
		template<typename NodeT, typename = ENABLE_IF_NODE_T(NodeT)>
		constexpr FORCE_INLINE NodeT* getMax(NodeT* root)
		{
			ASSERT(root != nullptr)
			while (root->right) root = root->right;
			return root;
		}

		/**
		 * @brief Return the first node that matches
		 * the search policy.
		 * 
		 * @param root root of the tree
		 * @param policy a function that returns -1 if
		 * the value is before a node, 1 if the value
		 * is after a node and 0 if it's the same
		 * @param outParent if given and node is not
		 * found, it will point to the last valid node
		 * along the path
		 * @return ptr to node if found
		 * @return null ptr otherwise
		 * @{
		 */
		template<typename NodeT, typename PolicyT, typename = ENABLE_IF_NODE_T(NodeT)>
		constexpr NodeT* find(NodeT* root, PolicyT&& policy, NodeT*& outParent)
		{
			for (auto* it = root; it;)
			{
				outParent = it;

				const int32 cmp = policy(it);
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
					return it;
				}
			}

			return nullptr;
		}

		template<typename NodeT, typename PolicyT, typename = ENABLE_IF_NODE_T(NodeT)>
		constexpr FORCE_INLINE NodeT* find(NodeT* root, PolicyT&& policy)
		{
			NodeT* _ = nullptr;
			return find(root, FORWARD(policy), _);
		}
		/** @} */

		/**
		 * @brief Insert a node in the tree.
		 * 
		 * @param root tree root node
		 * @param node node to insert
		 * @param policy a function that compares two
		 * nodes to choose which branch to take
		 * @return new root node
		 */
		template<typename NodeT, typename PolicyT, typename = ENABLE_IF_NODE_T(NodeT)>
		constexpr NodeT* insert(NodeT* root, NodeT* node, PolicyT&& policy)
		{
			if (!root)
			{
				// Eazy
				node->color = BinaryNodeColor::Color_BLACK;
				return node;
			}

			// Traverse tree to find node position
			int32 cmp = 0;
			auto* it = root;
			auto* parent = root;
			while (it)
			{
				// Set parent
				parent = it;

				cmp = policy(node, it);
				if (cmp < 0)
				{
					it = it->left;
				}
				else
				{
					it = it->right;
				}
			}

			// Insert node
			if (cmp < 0)
			{
				Impl::setLeftChild(parent, node);
			}
			else
			{
				Impl::setRightChild(parent, node);
			}

			// Repair tree after insertion
			Impl::repairInserted(node);

			// After repair, the root may have changed
			return getRoot(node);
		}

		/**
		 * @brief Remove node from tree.
		 * 
		 * @param node the node of the tree to
		 * remove
		 * @return the new root of the tree
		 */
		template<typename NodeT, typename = ENABLE_IF_NODE_T(NodeT)>
		constexpr NodeT* remove(NodeT* node)
		{
			// Replace and remove the node from the tree
			auto* next = node->next;
			if (node->left && node->right)
			{
				Impl::swapNodes(node, next);
			}
			auto* repl = Impl::replaceNode(node);

			// Repair tree after remove
			if (isBlack(node))
			{
				Impl::repairRemoved(repl, node->parent);
			}

			if (repl)
			{
				return getRoot(repl);
			}
			else if (next)
			{
				return getRoot(next);
			}

			return nullptr;
		}
	} // namespace TreeNode
} // namespace Korin

#undef ENABLE_IF_NODE_T
