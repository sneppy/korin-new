#pragma once

#include "containers_types.h"
#include "templates/enable_if.h"
#include "templates/ordering.h"
#include "templates/utility.h"

#define ENABLE_IF_NODE_T(T) typename EnableIf<IsBaseOf<BinaryNodeBase<T>, T>::value>::Type

namespace Containers
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
			// TODO: Check node is not null
			while (node->parent) node = node->parent;
			return node;
		}

		/**
		 * @brief Insert a node in the tree.
		 * 
		 * @param root tree root node
		 * @param node node to insert
		 * @param cmpfn if provided, a lambda that compares
		 * the two nodes
		 * @return new root node
		 * @{
		 */
		template<typename NodeT, typename CompareT, typename = ENABLE_IF_NODE_T(NodeT)>
		constexpr NodeT* insert(NodeT* root, NodeT* node, CompareT&& cmpfn)
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

				cmp = cmpfn(node, it);
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

		template<typename NodeT, typename CompareT, typename = ENABLE_IF_NODE_T(NodeT)>
		constexpr NodeT* insert(NodeT* root, NodeT* node)
		{
			return insert(root, node, CompareT{});
		}
		/** @} */
	} // namespace TreeNode
} // namespace Containers

