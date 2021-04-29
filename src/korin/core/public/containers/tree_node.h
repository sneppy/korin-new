#pragma once

#include "containers_types.h"

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
	 * @brief A node of a red-black tree, with two
	 * child nodes (left and right) and a logical
	 * next and previous nodes.
	 * 
	 * @tparam T the type of the value
	 */
	template<typename T>
	struct BinaryNode
	{

		/// @brief The value of the node.
		T value;

		/// @brief Pointer to the parent node.
		BinaryNode* parent = nullptr;

		/// @brief Pointer to the left child.
		BinaryNode* left = nullptr;

		/// @brief Pointer to the right child.
		BinaryNode* right = nullptr;

		/// @brief Pointer to the next-in-order node.
		BinaryNode* next = nullptr;

		/// @brief Pointer to the previous-in-order node.
		BinaryNode* prev = nullptr;

		/// @brief The color of the node.
		BinaryNodeColor color = BinaryNodeColor::Color_RED;

		/**
		 * @brief Set the left child of this
		 * node.
		 * 
		 * @param other ptr to node to set
		 */
		constexpr FORCE_INLINE void setLeftChild(BinaryNode* other)
		{
			// TODO: Check that left is null, otherwise something's probably wrong
			if ((left = other))
			{
				left->parent = this;
				left->next = this;
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
		constexpr FORCE_INLINE void setRightChild(BinaryNode* other)
		{
			// TODO: Check that right is null, otherwise something's probably wrong
			if ((right = other))
			{
				right->parent = this;
				right->prev = this;
				if ((right->next = next))
				{
					right->next->prev = right;
				}
				next = right;
			}
		}
	};

	namespace Tree_Private
	{
		/**
		 * @brief Return the root node of the tree
		 * that contains the given node.
		 * 
		 * @param node a node in the tree
		 * @return ptr to root
		 */
		template<typename T>
		constexpr FORCE_INLINE BinaryNode<T>* getRoot(BinaryNode<T>* node)
		{
			// TODO: Check node is not null
			while (node->parent) node = node->parent;
			return node;
		}

		/**
		 * @brief Return true if the node is not
		 * null and is red.
		 * 
		 * @param node ptr to node to test
		 */
		template<typename T>
		constexpr FORCE_INLINE bool isRed(BinaryNode<T> const* node)
		{
			return node && node->color == BinaryNodeColor::Color_RED;
		}

		/**
		 * @brief Return true if the node is null
		 * or is black.
		 * 
		 * @param node ptr to node to test
		 */
		template<typename T>
		constexpr FORCE_INLINE bool isBlack(BinaryNode<T> const* node)
		{
			return !isRed(node);
		}

		/**
		 * @brief Rotate left around pivot node.
		 * 
		 * @param pivot ptr to pivot node
		 */
		template<typename T>
		constexpr void rotateLeft(BinaryNode<T>* pivot)
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
		template<typename T>
		constexpr void rotateRight(BinaryNode<T>* pivot)
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
		template<typename T>
		constexpr void repairInserted(BinaryNode<T>* node)
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
		template<typename T>
		void repairRemoved(BinaryNode<T>* node, BinaryNode<T>* parent)
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
	} // namespace Tree_Private
} // namespace Containers

