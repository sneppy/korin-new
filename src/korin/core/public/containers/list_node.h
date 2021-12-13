#pragma once

namespace Korin
{
	/**
	 * @brief Add-on class to define a distributed
	 * forward list node type.
	 *
	 * e.g. `using NodeT = ForwardListNodeBase<BaseT>;`
	 *
	 * @tparam BaseT the base type
	 */
	template<typename BaseT>
	struct ForwardListNodeBase : public BaseT
	{
		using BaseT::BaseT;

		/* Ptr to the next node in the list. */
		ForwardListNodeBase* next = nullptr;
	};

	template<typename ListNodeT> class ForwardListIteratorBase;
	template<typename ListNodeT> class ForwardListConstIteratorBase;

	/**
	 * @brief Base iterator class to iterate
	 * over a forward list.
	 *
	 * @tparam ListNodeT the list node type
	 */
	template<typename ListNodeT>
	class ForwardListIteratorBase
	{
		friend ForwardListConstIteratorBase<ListNodeT>;

		using SelfT = ForwardListIteratorBase;

	public:
		/**
		 * @brief Construct a new instance of the
		 * iterator pointing to the given node
		 *
		 * @param node ptr to curr node or nullptr
		 */
		FORCE_INLINE ForwardListIteratorBase(ListNodeT* node = nullptr)
			: curr{node}
		{
			//
		}

		/* Returns true if this iterator and
		   another iterator point to the same
		   node. */
		FORCE_INLINE bool operator==(SelfT const& other) const
		{
			return curr == other.curr;
		}

		/* Returns true if this iterator and
		   another iterator point to different
		   nodes. */
		FORCE_INLINE bool operator!=(SelfT const& other) const
		{
			return !(*this == other);
		}

		/* Increment iterator and returns ref
		   to self. */
		FORCE_INLINE SelfT& operator++()
		{
			curr = curr->next;
			return *this;
		}

		/* Increment iterator and return a copy
		   of itself before the increment. */
		FORCE_INLINE SelfT operator++(int32)
		{
			SelfT other{*this};
			++(*this);
			return other;
		}

	protected:
		/* The node pointed by the iterator. */
		ListNodeT* curr;
	};

	/**
	 * @brief Base iterator class to iterate
	 * over a constant forward list.
	 *
	 * @tparam ListNodeT
	 */
	template<typename ListNodeT>
	class ForwardListConstIteratorBase
	{
		using SelfT = ForwardListConstIteratorBase;

	public:
		/**
		 * @copydoc ForwardListIterator::ForwardListIterator(ListNodeT*)
		 */
		FORCE_INLINE ForwardListConstIteratorBase(ListNodeT const* node = nullptr)
			: curr{node}
		{
			//
		}

		/**
		 * @brief Construct a new iterator that points
		 * to the same node of another non-const
		 * iterator.
		 *
		 * @param other a non-const list iterator
		 */
		FORCE_INLINE ForwardListConstIteratorBase(ForwardListIteratorBase<ListNodeT> const& other)
			: curr{other.curr}
		{
			//
		}

		/** @copydoc ForwardListIterator::operator==() */
		FORCE_INLINE bool operator==(SelfT const& other) const
		{
			return curr == other.curr;
		}

		/** @copydoc ForwardListIterator::operator!=() */
		FORCE_INLINE bool operator!=(SelfT const& other) const
		{
			return !(*this == other);
		}

		/** @copydoc ForwardListIterator::operator++() */
		FORCE_INLINE SelfT& operator++()
		{
			curr = curr->next;
			return *this;
		}

		/** @copydoc ForwardListIterator::operator++(int32) */
		FORCE_INLINE SelfT operator++(int32)
		{
			SelfT other{*this};
			++(*this);
			return other;
		}

	protected:
		/* The node pointed by the iterator. */
		ListNodeT const* curr;
	};
} // namespace Korin
