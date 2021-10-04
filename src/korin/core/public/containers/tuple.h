#pragma once

#include "templates/utility.h"
#include "containers_types.h"

namespace Korin
{
	/**
	 * @brief Class that contains the recursive
	 * implementation for the tuple type.
	 *
	 * @tparam idx current item index (begins from 0)
	 * @tparam ItemsT type of the items
	 */
	template<sizet idx, typename... ItemsT>
	class TupleBase;

	/**
	 * @brief Recursive case of the tuple base
	 * implementation.
	 *
	 * @tparam idx current index
	 * @tparam HeadT type of the first item in
	 * the list
	 * @tparam ItemsT type of the other items
	 */
	template<sizet idx, typename HeadT, typename ...ItemsT>
	class TupleBase<idx, HeadT, ItemsT...> : public TupleBase<idx + 1, ItemsT...>
	{
		using BaseT = TupleBase<idx + 1, ItemsT...>;

		// TODO: Move and copy semantics?

	public:
		TupleBase() = default;

		/**
		 * @brief Construct subtuple by copying the
		 * given items.
		 * 
		 * @param head head of the list to copy
		 * @param items rest of the list
		 */
		constexpr TupleBase(HeadT const& head, ItemsT const& ...items)
			: BaseT{items...}
			, item{head}
		{
			//
		}

		/**
		 * @brief Construct subtuple.
		 * 
		 * @param head head of the list
		 * @param items rest of the list
		 */
		constexpr TupleBase(auto&& head, auto&& ...items)
			: BaseT{FORWARD(items)...}
			, item{FORWARD(head)}
		{
			//
		}

		/**
		 * @brief Get first item that matches the
		 * given type. Raises an error at compile
		 * time if such item does not exist.
		 * 
		 * @tparam T type to get
		 * @return ref to first item of the tuple
		 * with the given type
		 * @{ 
		 */
		template<typename T>
		constexpr auto& get()
		{
			if constexpr (SameType<T, HeadT>::value)
			{
				return item;
			}
			else
			{
				return BaseT::template get<T>();
			}
		}

		template<typename T>
		constexpr auto const& get() const
		{
			return const_cast<TupleBase&>(*this).get<T>();
		}
		/** @} */

		/**
		 * @brief Returns the i-th item of the
		 * tuple.
		 * 
		 * Raises a compile-time error if the
		 * index is out of bounds.
		 * 
		 * @tparam jdx item index
		 * @return ref to i-th item
		 * @{
		 */
		template<sizet jdx>
		constexpr auto& get()
		{
			if constexpr (jdx == idx)
			{
				return item;
			}
			else
			{
				return BaseT::template get<jdx>();
			}
		}

		template<sizet jdx>
		constexpr auto const& get() const
		{
			return const_cast<TupleBase&>(*this).get<jdx>();
		}
		/** @} */

	protected:
		/* First item of this subtuple. */
		HeadT item;
	};

	template<sizet idx, typename HeadT>
	class TupleBase<idx, HeadT>
	{
	public:
		TupleBase() = default;

		/**
		 * @brief Construct subtuple by copying
		 * the given item.
		 * 
		 * @param head item to copy
		 */
		constexpr TupleBase(HeadT const& head)
			: item{head}
		{
			//
		}

		/**
		 * @brief Cosntruct the subtuple by moving
		 * the given item.
		 * 
		 * @param head item to move
		 */
		constexpr TupleBase(auto&& head)
			: item{FORWARD(head)}
		{
			//
		}

		/**
		 * @copydoc TupleBase<sizet, typename, typename...>::get<typename>()
		 */
		template<typename T>
		constexpr auto& get()
		{
			static_assert(SameType<HeadT, T>::value, "Could not find item with the given type");
			return item;
		}

		/**
		 * @copydoc TupleBase<sizet, typename, typename...>::get<typename>()
		 */
		template<typename T>
		constexpr auto const& get() const
		{
			return const_cast<TupleBase&>(*this).get<T>();
		}

		/**
		 * @copydoc TupleBase<sizet, typename, typename...>::get<sizet>()
		 */
		template<sizet jdx>
		constexpr auto& get()
		{
			static_assert(jdx == idx, "Index out of bounds");
			return item;
		}

		/**
		 * @copydoc TupleBase<sizet, typename, typename...>::get<sizet>()
		 */
		template<sizet jdx>
		constexpr auto const& get() const
		{
			return const_cast<TupleBase&>(*this).get<jdx>();
		}

	protected:
		/* Last item of tuple. */
		HeadT item;
	};

	/**
	 * @brief A tuple is a compile-time, multi-type
	 * container.
	 * 
	 * See @c tuple and @c tie.
	 * 
	 * @tparam ItemsT types of the items
	 */
	template<typename ...ItemsT>
	class Tuple : public TupleBase<0, ItemsT...>
	{
		using BaseT = TupleBase<0, ItemsT...>;

	public:
		using BaseT::BaseT;

		/**
		 * @brief Returns the number of items
		 * in the tuple.
		 */
		constexpr sizet getNumItems() const
		{
			return numItems;
		}

		// TODO: Tuple concatenation

	protected:
		/* Compile-time computed number of items. */
		static constexpr sizet numItems = sizeof...(ItemsT);
	};

	/**
	 * @brief Create a new tuple, copying or
	 * moving the given items.
	 * 
	 * @tparam ItemsT the type of the items
	 * @param items items to insert into tuple
	 * @return new tuple of items
	 */
	template<typename ...ItemsT>
	constexpr auto tuple(ItemsT&& ...items)
	{
		using TupleT = Tuple<typename Decay<ItemsT>::Type...>;
		return TupleT{forward<ItemsT>(items)...};
	}

	/**
	 * @brief Create a new tuple with the
	 * refs of the given items.
	 * 
	 * May be used to forward all items
	 * inside tuple.
	 * 
	 * @tparam ItemsT the type of the items
	 * @param items items to take refs to
	 * @return new tuple of refs
	 */
	template<typename ...ItemsT>
	constexpr auto tie(ItemsT&& ...items)
	{
		return Tuple<ItemsT&&...>{FORWARD(items)...};
	}
} // namespace Korin

