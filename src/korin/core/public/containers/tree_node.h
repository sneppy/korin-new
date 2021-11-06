#pragma once

#include "containers_types.h"
#include "templates/enable_if.h"
#include "templates/ordering.h"
#include "templates/utility.h"

namespace Korin
{
	/**
	 * @brief The color of a red and black
	 * tree node.
	 */
	enum class BinaryNodeColor : ubyte
	{
		Red,
		Black
	};

	/**
	 * @brief This type implements an
	 * RB tree node.
	 *
	 * @tparam BaseT the base type, usually
	 * contains the node payload
	 */
	template<typename BaseT>
	struct BinaryNodeBase : public BaseT
	{
		using BaseT::BaseT;

		/* Ptr to the parent node. */
		BinaryNodeBase* parent = nullptr;

		/* Ptr to the left child. */
		BinaryNodeBase* left = nullptr;

		/* Ptr to the right child. */
		BinaryNodeBase* right = nullptr;

		/* Ptr to the next node in order. */
		BinaryNodeBase* next = nullptr;

		/* Ptr to the previous node in order. */
		BinaryNodeBase* prev = nullptr;

		/* The color of the node. */
		BinaryNodeColor color = BinaryNodeColor::Red;
	};

	namespace TreeNode
	{
		/**
		 * @brief Returns true if node is not null
		 * and is red.
		 *
		 * @tparam BaseT the node base type
		 * @param node node to test
		 * @return true if node is red
		 * @return false otherwise
		 */
		template<typename BaseT>
		FORCE_INLINE bool isRed(BinaryNodeBase<BaseT> const* node)
		{
			return node && node->color == BinaryNodeColor::Red;
		}

		/**
		 * @brief Returns true if node is null or
		 * is black.
		 *
		 * @tparam BaseT the node base type
		 * @param node node to test
		 * @return true if node is null or black
		 * @return false otherwise
		 */
		template<typename BaseT>
		FORCE_INLINE bool isBlack(BinaryNodeBase<BaseT> const* node)
		{
			return !isRed(node);
		}

		namespace Impl
		{
			/**
			 * @brief Append left child to node.
			 *
			 * @tparam BaseT the node base type
			 * @param parent parent node to append to
			 * @param child child to append
			 */
			template<typename BaseT>
			FORCE_INLINE void insertLeft(BinaryNodeBase<BaseT>* parent, BinaryNodeBase<BaseT>* child)
			{
				ASSERT(parent != nullptr)
				ASSERT(child != nullptr)
				CHECK(child->parent == nullptr)

				auto* const prev = parent->prev;

				parent->left = child;
				parent->prev = child;

				child->parent = parent;
				child->next = parent;
				child->prev = prev;

				if (child->prev)
				{
					child->prev->next = child;
				}
			}

			/**
			 * @brief Append right child to node.
			 *
			 * @tparam BaseT the node base type
			 * @param parent parent node to append to
			 * @param child child to append
			 */
			template<typename BaseT>
			FORCE_INLINE void insertRight(BinaryNodeBase<BaseT>* parent, BinaryNodeBase<BaseT>* child)
			{
				ASSERT(parent != nullptr)
				ASSERT(child != nullptr)
				CHECK(child->parent == nullptr)

				auto* const next = parent->next;

				parent->right = child;
				parent->next = child;

				child->parent = parent;
				child->prev = parent;
				child->next = next;

				if (child->next)
				{
					child->next->prev = child;
				}
			}

			/**
			 * @brief Perform a right rotation around
			 * the pivot.
			 *
			 * See https://en.wikipedia.org/wiki/Tree_rotation#:~:text=In%20discrete%20mathematics%2C%20tree%20rotation,tree%20and%20one%20node%20down.
			 *
			 * This does not affect the nodes color.
			 *
			 * @tparam BaseT the base type of the node
			 * @param pivot rotation pivot
			 */
			template<typename BaseT>
			FORCE_INLINE void rotateLeft(BinaryNodeBase<BaseT>* pivot)
			{
				ASSERT(pivot != nullptr)
				ASSERT(pivot->right != nullptr)

				auto* grand = pivot->parent;
				auto* node = pivot->right;
				auto* child = node->left;

				pivot->parent = node;
				pivot->right = child;
				node->parent = grand;
				node->left = pivot;

				if (grand)
				{
					if (grand->left == pivot)
						grand->left = node;
					else // if (grand->right == pivot)
						grand->right = node;
				}

				if (child)
				{
					child->parent = pivot;
				}
			}

			/**
			 * @brief Perform a left rotation around
			 * the pivot.
			 *
			 * @see rotateLeft
			 *
			 * @tparam BaseT the base type of the node
			 * @param pivot rotation pivot
			 */
			template<typename BaseT>
			FORCE_INLINE void rotateRight(BinaryNodeBase<BaseT>* pivot)
			{
				ASSERT(pivot != nullptr)
				ASSERT(pivot->left != nullptr)

				auto* grand = pivot->parent;
				auto* node = pivot->left;
				auto* child = node->right;

				pivot->parent = node;
				pivot->left = child;
				node->parent = grand;
				node->right = pivot;

				if (grand)
				{
					if (grand->right == pivot)
						grand->right = node;
					else // if (grand->teft == pivot)
						grand->left = node;
				}

				if (child)
				{
					child->parent = pivot;
				}
			}

			/**
			 * @brief Swap the payload between two nodes.
			 *
			 * @tparam BaseT the base type of the nodes
			 * @param node first node
			 * @param other second node
			 */
			template<typename BaseT>
			FORCE_INLINE void swapNodes(BinaryNodeBase<BaseT>* node, BinaryNodeBase<BaseT>* other)
			{
				// Swap everything
				swap(*node, *other);

				// Then swap back ptrs and color
				swap(node->parent, other->parent);
				swap(node->left, other->left);
				swap(node->right, other->right);
				swap(node->next, other->next);
				swap(node->prev, other->prev);
				swap(node->color, other->color);
			}

			/**
			 * @brief Replaces the node with one of its
			 * children.
			 *
			 * @tparam BaseT the type of the node
			 * @param node node to evict
			 * @return ptr to replacement node
			 * @return nullptr if node is leaf
			 */
			template<typename BaseT>
			FORCE_INLINE BinaryNodeBase<BaseT>* evictNode(BinaryNodeBase<BaseT>* node)
			{
				ASSERT(!(node->left && node->right))

				auto* repl = (decltype(node))nullptr;
				auto* parent = node->parent;

				if ((repl = node->left))
				{
					repl->parent = parent;
					repl->next = node->next;
					if (repl->next)
					{
						repl->next->prev = repl;
					}
				}
				else if ((repl = node->right))
				{
					repl->parent = parent;
					repl->prev = node->prev;
					if (repl->prev)
					{
						repl->prev->next = repl;
					}
				}
				else
				{
					if (node->next)
					{
						node->next->prev = node->prev;
					}

					if (node->prev)
					{
						node->prev->next = node->next;
					}
				}

				if (parent)
				{
					if (parent->left == node)
						parent->left = repl;
					else // if (parent->right == node)
						parent->right = repl;
				}

				return repl;
			}

			/**
			 * @brief Repair an RB tree after insertion.
			 *
			 * @tparam BaseT the base type of the nodes
			 * @param node the inserted node
			 */
			template<typename BaseT>
			void repair(BinaryNodeBase<BaseT>* node)
			{
				ASSERT(node != nullptr)

				if (!node->parent)
				{
					// Node is root
					node->color = BinaryNodeColor::Black;
				}
				else if (isRed(node->parent))
				{
					auto* parent = node->parent;
					auto* grand = parent->parent;
					auto* uncle = grand->left == parent
								? grand->right
								: grand->left;

					if (isRed(uncle))
					{
						// Parent and uncle are red, we can set
						// both black and repair grand
						uncle->color = parent->color = BinaryNodeColor::Black;
						grand->color = BinaryNodeColor::Red;
						repair(grand);
					}
					else // if (isBlack(uncle))
					{
						if (grand->left == parent)
						{
							if (parent->right == node)
							{
								Impl::rotateLeft(parent);
								swap(node, parent);
							}

							Impl::rotateRight(grand);
						}
						else // if (grand->right == parent)
						{
							if (parent->left == node)
							{
								Impl::rotateRight(parent);
								swap(node, parent);
							}

							Impl::rotateLeft(grand);
						}

						parent->color = BinaryNodeColor::Black;
						grand->color = BinaryNodeColor::Red;
					}
				}
			}

			/**
			 * @brief Repair RB tree structure after
			 * removing a node.
			 * 
			 * @tparam BaseT the base type of the nodes
			 * @param node the replacement node
			 * @param parent the parent of the node
			 */
			template<typename BaseT>
			void repairRemoved(BinaryNodeBase<BaseT>* node, BinaryNodeBase<BaseT>* parent)
			{
				do
				{
					if (!node && !parent)
						break; // Nothing to do
					else if (isRed(node) || !parent)
					{
						// https://en.wikipedia.org/wiki/Red%E2%80%93black_tree#Delete_case_2
						// https://en.wikipedia.org/wiki/Red%E2%80%93black_tree#Delete_case_4
						node->color = BinaryNodeColor::Black;
						break;
					}
					else if (parent->left == node)
					{
						auto* sibling = parent->right;

						if (isRed(sibling))
						{
							// https://en.wikipedia.org/wiki/Red%E2%80%93black_tree#Delete_case_3
							rotateLeft(parent);
							parent->color = BinaryNodeColor::Red;
							sibling->color = BinaryNodeColor::Black;

							// Update sibling ptr
							sibling = parent->right;
						}

						if (isBlack(sibling->right) && isBlack(sibling->left))
						{
							// https://en.wikipedia.org/wiki/Red%E2%80%93black_tree#Delete_case_1
							sibling->color = BinaryNodeColor::Red;
							node = parent;
						}
						else
						{
							if (isRed(sibling->left))
							{
								// https://en.wikipedia.org/wiki/Red%E2%80%93black_tree#Delete_case_5
								rotateRight(sibling);
								sibling->color = BinaryNodeColor::Red;
								sibling->parent->color = BinaryNodeColor::Black;

								// Update sibling
								sibling = sibling->parent;
							}

							// https://en.wikipedia.org/wiki/Red%E2%80%93black_tree#Delete_case_6
							rotateLeft(parent);
							sibling->color = parent->color;
							parent->color = BinaryNodeColor::Black;
							sibling->right->color = BinaryNodeColor::Black;

							break;
						}
					}
					else // if (parent->right == node)
					{
						auto* sibling = parent->left;

						if (isRed(sibling))
						{
							// https://en.wikipedia.org/wiki/Red%E2%80%93black_tree#Delete_case_3
							rotateRight(parent);
							parent->color = BinaryNodeColor::Red;
							sibling->color = BinaryNodeColor::Black;

							// Update sibling ptr
							sibling = parent->left;
						}

						if (isBlack(sibling->left) && isBlack(sibling->right))
						{
							// https://en.wikipedia.org/wiki/Red%E2%80%93black_tree#Delete_case_1
							sibling->color = BinaryNodeColor::Red;
							node = parent;
						}
						else
						{
							if (isRed(sibling->right))
							{
								// https://en.wikipedia.org/wiki/Red%E2%80%93black_tree#Delete_case_5
								rotateLeft(sibling);
								sibling->color = BinaryNodeColor::Red;
								sibling->parent->color = BinaryNodeColor::Black;

								// Update sibling
								sibling = sibling->parent;
							}

							// https://en.wikipedia.org/wiki/Red%E2%80%93black_tree#Delete_case_6
							rotateRight(parent);
							sibling->color = parent->color;
							parent->color = BinaryNodeColor::Black;
							sibling->left->color = BinaryNodeColor::Black;

							break;
						}
					}
				} while ((parent = node->parent));
			}
		} // namespace Impl

		/**
		 * @brief Returns the root of the tree that
		 * contains the given node.
		 *
		 * @tparam BaseT the base type of the nodes
		 * @param node a node of the tree
		 * @return root node of the tree
		 * @{
		 */
		template<typename BaseT>
		FORCE_INLINE BinaryNodeBase<BaseT> const* getRoot(BinaryNodeBase<BaseT> const* node)
		{
			ASSERT(node != nullptr)
			while (node->parent) node = node->parent;
			return node;
		}

		template<typename BaseT>
		FORCE_INLINE BinaryNodeBase<BaseT>* getRoot(BinaryNodeBase<BaseT>* node)
		{
			return const_cast<BinaryNodeBase<BaseT>*>(getRoot(const_cast<BinaryNodeBase<BaseT> const*>(node)));
		}
		/** @} */

		/**
		 * @brief Return the leftmost node in the
		 * subtree spawning from the given node.
		 *
		 * The leftmost node is also the minimum
		 * value node according to the policy
		 * used to populate the tree.
		 *
		 * @tparam BaseT the base type of the nodes
		 * @param root root of the subtree
		 * @return leftmost node of the subtree
		 * @{
		 */
		template<typename BaseT>
		FORCE_INLINE BinaryNodeBase<BaseT> const* getMin(BinaryNodeBase<BaseT> const* root)
		{
			ASSERT(root != nullptr)
			while (root->left) root = root->left;
			return root;
		}

		template<typename BaseT>
		FORCE_INLINE BinaryNodeBase<BaseT>* getMin(BinaryNodeBase<BaseT>* root)
		{
			return const_cast<BinaryNodeBase<BaseT>*>(getMin(const_cast<BinaryNodeBase<BaseT> const*>(root)));
		}
		/** @} */

		/**
		 * @brief Return the rightmost node in the
		 * subtree spawning from the given node.
		 *
		 * The rightmost node is also the maximum
		 * value node according to the policy
		 * used to populate the tree.
		 *
		 * @tparam BaseT the base type of the nodes
		 * @param root root of the subtree
		 * @return rightmost node of the subtree
		 * @{
		 */
		template<typename BaseT>
		FORCE_INLINE BinaryNodeBase<BaseT> const* getMax(BinaryNodeBase<BaseT> const* root)
		{
			ASSERT(root != nullptr)
			while (root->right) root = root->right;
			return root;
		}

		template<typename BaseT>
		FORCE_INLINE BinaryNodeBase<BaseT>* getMax(BinaryNodeBase<BaseT>* root)
		{
			return const_cast<BinaryNodeBase<BaseT>*>(getMax(const_cast<BinaryNodeBase<BaseT> const*>(root)));
		}
		/** @} */

		/**
		 * @brief Returns the first node that satisfy
		 * the given policy.
		 *
		 * If multiple nodes would satisfy the given
		 * policy, it returns the first one encountered.
		 *
		 * @tparam BaseT the base type of the nodes
		 * @tparam PolicyT the type of the policy
		 * @param root the root node of the tree
		 * @param policy the policy used to evaluate
		 * nodes
		 * @return ptr to first node found
		 * @return nullptr if not found
		 * @{
		 */
		template<typename BaseT, typename PolicyT>
		BinaryNodeBase<BaseT> const* find(BinaryNodeBase<BaseT> const* root, PolicyT&& policy)
		{
			while (root)
			{
				int32 cmp = policy(root);

				if (cmp < 0)
				{
					root = root->left;
				}
				else if (cmp > 0)
				{
					root = root->right;
				}
				else
				{
					// Found node
					return root;
				}
			}

			// No node found
			return nullptr;
		}

		template<typename BaseT, typename PolicyT>
		FORCE_INLINE BinaryNodeBase<BaseT>* find(BinaryNodeBase<BaseT>* root, PolicyT&& policy)
		{
			// TODO: Consider the other way around?
			// Like this, the node passed to the policy is const
			return const_cast<BinaryNodeBase<BaseT>*>(find(const_cast<BinaryNodeBase<BaseT> const*>(root), FORWARD(policy)));
		}
		/** @} */

		/**
		 * @brief Traverse the subtree spawning from
		 * the root node according to policy, and
		 * returns the last visited node.
		 *
		 * When the policy cannot make a decision it
		 * visits the left subtree.
		 *
		 * @tparam BaseT the base type of the nodes
		 * @tparam PolicyT the type of the policy
		 * @param root the root node of the subtree
		 * @param policy the policy used to evaluate
		 * nodes
		 * @return ptr to last visited node
		 * @return nullptr if subtree is empty
		 * @{
		 */
		template<typename BaseT, typename PolicyT>
		BinaryNodeBase<BaseT> const* bisectLeft(BinaryNodeBase<BaseT> const* root, PolicyT&& policy)
		{
			BinaryNodeBase<BaseT> const* parent = nullptr;
			while (root)
			{
				parent = root;
				if (policy(root) <= 0) // If equal go left
				{
					root = root->left;
				}
				else
				{
					root = root->right;
				}
			}

			return parent;
		}

		template<typename BaseT, typename PolicyT>
		FORCE_INLINE BinaryNodeBase<BaseT>* bisectLeft(BinaryNodeBase<BaseT>* root, PolicyT&& policy)
		{
			return const_cast<BinaryNodeBase<BaseT>*>(bisectLeft(const_cast<BinaryNodeBase<BaseT> const*>(root), FORWARD(policy)));
		}
		/** @} */

		/**
		 * @brief Traverse the subtree spawning from
		 * the root node according to policy, and
		 * returns the last visited node.
		 *
		 * When the policy cannot make a decision it
		 * visits the right subtree.
		 *
		 * @tparam BaseT the base type of the nodes
		 * @tparam PolicyT the type of the policy
		 * @param root the root node of the subtree
		 * @param policy the policy used to evaluate
		 * nodes
		 * @return ptr to last visited node
		 * @return nullptr if subtree is empty
		 * @{
		 */
		template<typename BaseT, typename PolicyT>
		BinaryNodeBase<BaseT> const* bisectRight(BinaryNodeBase<BaseT> const* root, PolicyT&& policy)
		{
			BinaryNodeBase<BaseT> const* parent = nullptr;
			while (root)
			{
				parent = root;
				if (policy(root) < 0) // If equal go right
				{
					root = root->left;
				}
				else
				{
					root = root->right;
				}
			}

			return parent;
		}

		template<typename BaseT, typename PolicyT>
		FORCE_INLINE BinaryNodeBase<BaseT>* bisectRight(BinaryNodeBase<BaseT>* root, PolicyT&& policy)
		{
			return const_cast<BinaryNodeBase<BaseT>*>(bisectRight(const_cast<BinaryNodeBase<BaseT> const*>(root), FORWARD(policy)));
		}
		/** @} */

		/**
		 * @brief Insert a node in the tree.
		 *
		 * @tparam BaseT the base type of the nodes
		 * @tparam PolicyT the type of the policy
		 * @param root the root of the tree
		 * @param node the node to insert
		 * @param policy the policy used to evaluate
		 * the nodes
		 * @return the new root of the tree
		 */
		template<typename BaseT, typename PolicyT>
		BinaryNodeBase<BaseT>* insert(BinaryNodeBase<BaseT>* root, BinaryNodeBase<BaseT>* node, PolicyT&& policy)
		{
			ASSERT(node != nullptr)

			if (auto* parent = bisectRight(root, FORWARD(policy)))
			{
				if (policy(parent) < 0)
				{
					Impl::insertLeft(parent, node);
				}
				else
				{
					Impl::insertRight(parent, node);
				}
			}

			// Repair the inserted node
			Impl::repair(node);

			// Return the new root
			return getRoot(node);
		}

		/**
		 * @brief Remove the node from the tree.
		 * 
		 * The node that is actually evicted from
		 * the tree is returned in the first
		 * parameter
		 * 
		 * @tparam BaseT the base type of the node
		 * @param node node to remove; returns ptr
		 * to node actually evicted
		 * @return ptr to new root
		 * @return nullptr if empty
		 */
		template<typename BaseT>
		BinaryNodeBase<BaseT>* remove(BinaryNodeBase<BaseT>*& node)
		{
			ASSERT(node != nullptr)

			auto* next = node->next;

			if (node->right && node->left)
			{
				// Swap node with his next
				Impl::swapNodes(node, next);
				swap(node, next);
			}

			// Replace node with left or right child
			// or simply evict from tree
			auto* parent = node->parent;
			auto* repl = Impl::evictNode(node);

			// Repair after eviction
			if (isBlack(node))
			{
				Impl::repairRemoved(repl, parent);
			}

			// Return new root
			if (parent)
			{
				return getRoot(parent);
			}
			else if (repl)
			{
				// If evicting root
				return getRoot(repl);
			}

			// Tree is empty
			return nullptr;
		}
	} // namespace TreeNode
} // namespace Korin
