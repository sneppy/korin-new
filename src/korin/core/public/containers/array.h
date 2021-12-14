#pragma once

#include "containers_types.h"
#include "templates/utility.h"
#include "hal/platform_memory.h"

// TODO: Replace with platform math
template<typename T>
static constexpr FORCE_INLINE T max(T const& a, auto const& b)
{
	return a > b ? a : b;
}

#ifndef KORIN_ARRAY_MIN_SIZE
# define KORIN_ARRAY_MIN_SIZE 4
#endif

namespace Korin
{
	/**
	 * @brief A templated growing array
	 * that stores item in a contiguous
	 * memory buffer.
	 *
	 * @tparam T the type of the items
	 */
	template<typename T>
	class Array
	{
		template<typename> friend class StringBase;

		using Iterator = T*;
		using ConstIterator = T const*;

		/**
		 * @brief Called to destroy all the items
		 * and deallocate the buffer.
		 */
		void destroy()
		{
			if (data)
			{
				// Destroy items first
				destroyItems(data, count);

				// Dealloc buffer
				gMalloc->free(data);
				data = nullptr;
			}

			size = count = 0;
		}

		/**
		 * @brief Resize the buffer and move
		 * the items to the new buffer.
		 *
		 * @param newSize new size of the buffer
		 * in number of items
		 */
		void resize(sizet const newSize)
		{
			CHECKF(newSize != 0, "New buffer size is 0, did you meant to destroy the array?")
			CHECKF(newSize >= count, "Cannot fit array data (%llu items) in new buffer (%llu items)", count, newSize)

			// The alignment is known at compile time
			constexpr sizet alignment = max(alignof(T), MIN_ALIGNMENT);

			// Alloc new buffer
			T* newData = reinterpret_cast<T*>(gMalloc->malloc(newSize * sizeof(T), alignment));

			if (data)
			{
				// Move existing items to new buffer
				moveConstructItems(newData, data, count);

				// Dealloc old buffer
				gMalloc->free(data);
			}

			data = newData;
			size = newSize;
		}

		/**
		 * @brief Resize the buffer to fit the
		 * required number of items.
		 *
		 * @param requiredSize min size in number
		 * of items
		 * @return true if buffer was resized
		 * @return false otherwise
		 */
		FORCE_INLINE bool growToFit(sizet const requiredSize)
		{
			if (size < requiredSize)
			{
				// Find appropriate size
				sizet newSize = max(size, KORIN_ARRAY_MIN_SIZE);
				for (; newSize < requiredSize; newSize = newSize << 1);

				// Resize buffer
				resize(newSize);
				return true;
			}

			return false;
		}

		/**
		 * @brief Shrink down the buffer allocation
		 * as much as possible.
		 *
		 * The buffer must still fit the required
		 * number of items.
		 *
		 * @param requiredSize number of items to
		 * fit
		 * @return true if buffer was resized
		 * @return false otherwise
		 */
		bool shrinkToFit(sizet const requiredSize)
		{
			// Find required size
			sizet newSize = size;
			for (; newSize > requiredSize * 2; newSize = newSize >> 1);

			if (newSize != size)
			{
				// Resize the buffer
				resize(newSize);
				return true;
			}

			return false;
		}

		/**
		 * @brief Copy construct items in array
		 * with the given value.
		 *
		 * @param idx index of current item
		 * @param item current item
		 * @param items rest of items
		 */
		FORCE_INLINE void initItems(uint64 idx, auto&& item, auto&& ...items)
		{
			// Construct current item
			new (data + idx) T{FORWARD(item)};

			if constexpr (sizeof...(items) > 0)
			{
				// If there are items left, call again
				initItems(idx + 1, FORWARD(items)...);
			}
		}

		/**
		 * @brief Implementation for @c concat.
		 *
		 * Items of other will be copied into
		 * this array.
		 */
		void concatImpl(sizet numItems, Array const& other, auto&& ...others)
		{
			if constexpr (sizeof...(others) > 0)
			{
				concatImpl(numItems + other.count, FORWARD(others)...);
			}
			else
			{
				// Resize buffer if necessary
				growToFit(numItems + other.count);
				count = numItems + other.count;
			}

			// Copy items
			copyConstructItems(data + numItems, other.data, other.count);
		}

		/**
		 * @brief Implementation for @c concat.
		 *
		 * Items of other will be moved into
		 * this array, and other's count set to
		 * zero.
		 *
		 * When other will be destroyed, there
		 * will be no items to be destroyed, but
		 * buffer should still be freed.
		 *
		 * @param other
		 * @param others
		 */
		void concatImpl(sizet numItems, Array&& other, auto&& ...others)
		{
			if constexpr (sizeof...(others) > 0)
			{
				concatImpl(numItems + other.count, FORWARD(others)...);
			}
			else
			{
				// Resize buffer if necessary
				growToFit(numItems + other.count);
				count = numItems + other.count;
			}

			// Move items
			moveConstructItems(data + numItems, other.data, other.count);
			other.count = 0;
		}
		/** @} */

	public:
		/**
		 * @brief Construct an empty array.
		 */
		FORCE_INLINE Array()
			: data{nullptr}
			, size{0}
			, count{0}
		{
			//
		}

		/**
		 * @brief Construct an array with an
		 * initial size (but no items).
		 *
		 * @param reservedSize size to reserve
		 */
		FORCE_INLINE Array(sizet reservedSize)
			: Array{}
		{
			// Create buffer if necessary
			growToFit(reservedSize);
		}

		/**
		 * @brief Construct a new array and insert
		 * @c numItems copies of @c item in it.
		 *
		 * @param numItems number of copies to insert
		 * @param item item to copy
		 * @param slack extra size reserved
		 */
		FORCE_INLINE Array(sizet numItems, T const& item, sizet slack = 0)
			: Array{}
		{
			// Create buffer if necessary
			growToFit(numItems + slack);
			count = numItems;

			// Initialize items
			constructItems(data, item, count);
		}

		/**
		 * @brief Construct a new array to fit the
		 * given content, and copy the items.
		 *
		 * @param items pointer to items to copy
		 * @param numItems number of items to copy
		 * @param extraSlack extra space to reserve
		 */
		FORCE_INLINE Array(T const* items, sizet numItems, sizet extraSlack = 0)
			: Array{}
		{
			// Create buffer
			growToFit(numItems + extraSlack);
			count = numItems;

			// Copy items
			copyConstructItems(data, items, numItems);
		}

		/**
		 * @brief Construct a copy of another
		 * array.
		 *
		 * @param other another array
		 */
		Array(Array const& other)
			: data{nullptr}
			, size{0}
			, count{0}
		{
			if (other.data)
			{
				// Create buffer
				resize(other.size);

				// Copy items from other array
				copyConstructItems(data, other.data, other.count);
				count = other.count;
			}
		}

		/**
		 * @brief Construct a new array by copying another
		 * array, and reserve extra space for it.
		 *
		 * the extra items are not initialized, nor should
		 * they be managed before calling one of the methods
		 * to insert new items (unless it is safe to do so).
		 *
		 * @param other another array to copy
		 * @param slack extra space to reserve (in number
		 * of items)
		 */
		Array(Array const& other, uint64 slack)
			: data{nullptr}
			, size{0}
			, count{0}
		{
			if (other.data || slack > 0)
			{
				// Determine size
				size = other.size;
				if (!growToFit(other.count + slack))
				{
					// Then simply resize to other buffer size
					resize(other.size);
				}

				// Copy items from other array
				copyConstructItems(data, other.data, other.count);
				count = other.count;
			}
		}

		/**
		 * @brief Move another array.
		 *
		 * @param other another array
		 */
		Array(Array&& other)
			: data{other.data}
			, size{other.size}
			, count{other.count}
		{
			other.data = nullptr;
			other.size = other.count = 0;
		}

		/**
		 * @brief Copy another array.
		 *
		 * @param other another array
		 * @return ref to self
		 */
		Array& operator=(Array const& other)
		{
			if (data && !other.data)
			{
				// Destroy array
				destroy();
			}
			else
			{
				if (count > other.count)
				{
					// Destroy extra items
					destroyItems(data + other.count, count - other.count);

					count = other.count;
					shrinkToFit(count);
				}
				else
				{
					// Resize if necessary
					// TODO: Avoid moving items
					growToFit(other.count);
					count = other.count;
				}

				// Copy items
				copyItems(data, other.data, other.count);
			}

			return *this;
		}

		/**
		 * @brief Move another array.
		 *
		 * @param other another array
		 * @return ref to self
		 */
		Array& operator=(Array&& other)
		{
			// Destroy array first
			destroy();

			data = other.data;
			size = other.size;
			count = other.count;

			other.data = nullptr;
			other.size = other.count = 0;

			return *this;
		}

		/**
		 * @brief Destructor, destroys the
		 * array and all the items.
		 */
		FORCE_INLINE ~Array()
		{
			destroy();
		}

		/**
		 * @brief Returns the number of items
		 * in the array.
		 */
		FORCE_INLINE sizet getNumItems() const
		{
			return count;
		}

		/**
		 * @brief Returns the number of Bytes required
		 * to store array items.
		 */
		FORCE_INLINE sizet getNumBytes() const
		{
			return count * sizeof(T);
		}

		/**
		 * @brief Returns a pointer that points
		 * to the array buffer.
		 * @{
		 */
		FORCE_INLINE T const* operator*() const
		{
			return data;
		}

		FORCE_INLINE T* operator*()
		{
			return data;
		}
		/** @} */

		/**
		 * @brief Returns a reference to the
		 * i-th item.
		 *
		 * @param idx index of the item
		 * @return ref to the item
		 */
		FORCE_INLINE T const& operator[](uint64 idx) const
		{
			return data[idx];
		}

		FORCE_INLINE T& operator[](uint64 idx)
		{
			return data[idx];
		}
		/** @} */

		/**
		 * @brief Returns an iterator that points
		 * to the first item of the array.
		 * @{
		 */
		FORCE_INLINE Iterator begin()
		{
			return data;
		}

		FORCE_INLINE ConstIterator begin() const
		{
			return data;
		}
		/** @} */

		/**
		 * @brief Returns an iterator that points
		 * to the end of the array.
		 * @{
		 */
		FORCE_INLINE Iterator end()
		{
			return data + count;
		}

		FORCE_INLINE ConstIterator end() const
		{
			return data + count;
		}
		/** @} */

		/**
		 * @brief Returns an iterator that points
		 * to the last item of the array
		 * @{
		 */
		FORCE_INLINE Iterator rbegin()
		{
			return data + (count - 1);
		}

		FORCE_INLINE ConstIterator rbegin() const
		{
			return data + (count - 1);
		}
		/** @} */

		/**
		 * @brief Returns an iterator that points
		 * before the beginning of the array
		 * @{
		 */
		FORCE_INLINE Iterator rend()
		{
			return data - 1;
		}

		FORCE_INLINE ConstIterator rend() const
		{
			return data - 1;
		}
		/** @} */

		/**
		 * @brief Append one or more items to
		 * the array
		 *
		 * @param items items to append to the
		 * array
		 */
		void append(auto&& ...items)
		{
			constexpr sizet numItems = sizeof...(items);

			// Grow data buffer to accomodate for items
			growToFit(count + numItems);
			initItems(count, FORWARD(items)...);
			count += numItems;
		}

		/**
		 * @brief Insert one or more item in
		 * a precise position.
		 *
		 * @param idx index of the first item
		 * inserted
		 * @param items the items to insert
		 */
		void insert(uint64 idx, auto&& ...items)
		{
			constexpr sizet numItems = sizeof...(items);

			if (idx < count)
			{
				// Grow buffer and move items to accomodate for new items
				growToFit(count + numItems);
				moveItems(data + idx + numItems, data + idx, count - idx);
				count += numItems;
			}
			else
			{
				// Just grow the buffer, new items before inserted
				// items are left uninitialized
				growToFit(idx + numItems);
				count += numItems;
			}

			// Construct the items
			initItems(idx, FORWARD(items)...);
		}

		/**
		 * @brief Construct and append an item
		 * to the array.
		 *
		 * @param args arguments used to construct
		 * the item
		 */
		FORCE_INLINE T& emplaceLast(auto&& ...args)
		{
			append({FORWARD(args)...});
			return data[count - 1];
		}

		/**
		 * @brief Construct and insert an item
		 * in the array.
		 *
		 * @param idx index of inserted item
		 * @param args arguments used to construct
		 * the item
		 */
		FORCE_INLINE T& emplace(uint64 idx, auto&& ...args)
		{
			insert(idx, {FORWARD(args)...});
			return data[idx];
		}

		/**
		 * @brief Append one or more other arrays
		 * at the end of this array.
		 *
		 * If the array to append is passed as
		 * an r-value reference, all its items
		 * will be moved instead of copied.
		 *
		 * @param others arrays to append
		 * @{
		 */
		FORCE_INLINE Array& concat(auto&& ...others)
		{
			concatImpl(count, FORWARD(others)...);
			return *this;
		}

		/**
		 * @brief Remove the last item of the
		 * array.
		 */
		void pop()
		{
			count--;
			destroyItems(data + count, 1);
			shrinkToFit(count);
		}

		/**
		 * @brief Remove one or more items at
		 * the given position.
		 *
		 * @param idx index of first item to
		 * remove
		 * @param numItems number of items to
		 * remove
		 */
		void removeAt(uint64 idx, sizet numItems = 1)
		{
			CHECK(numItems > 0)
			CHECK(idx < count)

			if (idx == count - 1)
			{
				CHECKF(numItems == 1, "Trying to remove too many items (%llu out of %d removables)", numItems, 1)

				// Pop last item
				pop();
			}
			else
			{
				CHECKF(idx + numItems <= count, "Trying to remove too many items (%llu of %llu removables)", numItems, count - idx)

				// Start by destroying items to remove
				destroyItems(data + idx, numItems);

				if (idx + numItems < count)
				{
					// Move back items
					moveItems(data + idx, data + idx + numItems, count - (idx + numItems));
				}

				count -= numItems;
				shrinkToFit(count);
			}
		}

		/**
		 * @brief Remove all items in the given
		 * range.
		 *
		 * @param from iterator that points to the
		 * first item to remove
		 * @param to iterator that points after the
		 * last item to remove
		 */
		FORCE_INLINE void remove(ConstIterator from, ConstIterator to)
		{
			const sizet numItems = from - to;
			removeAt(from - data, numItems);
		}

		/**
		 * @brief Returns a slice of the array
		 * as a copy.
		 *
		 * @param beginIdx index of the beginning
		 * of the slice
		 * @param endIdx index of the end of the
		 * slice (excluded); if omitted assume
		 * end of array.
		 * @return sliced array copy
		 */
		Array slice(uint64 beginIdx, uint64 endIdx) const
		{
			CHECK(endIdx <= count)
			CHECK(beginIdx <= endIdx)
			CHECKF(beginIdx != endIdx, "Slice [%llu:%llu] is empty", beginIdx, endIdx)

			const sizet numItems = endIdx - beginIdx;

			// Resize array to fit the items
			Array sliced;
			sliced.growToFit(numItems);
			sliced.count = numItems;

			// Copy items
			copyConstructItems(sliced.data, data + beginIdx, numItems);

			return sliced;
		}

		FORCE_INLINE Array slice(uint64 beginIdx) const
		{
			// Slice from begin to end of array
			return slice(beginIdx, count);
		}
		/** @} */

	protected:
		/* Pointer to the data buffer. */
		T* data;

		/* Size of the buffer in num of items. */
		sizet size;

		/* Number of items in the array. */
		sizet count;
	};
} // namespace Korin
