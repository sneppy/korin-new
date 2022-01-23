#pragma once

#include "containers_types.h"
#include "misc/logging.h"
#include "templates/utility.h"
#include "hal/platform_memory.h"
#include "hal/platform_math.h"


#ifndef KORIN_ARRAY_MIN_SIZE
# define KORIN_ARRAY_MIN_SIZE 4ull
#endif

namespace Korin
{
	/**
	 * @brief Base implementation of a generic dynamic
	 * array.
	 *
	 * @tparam T the type of the items
	 */
	template<typename T>
	class ArrayBase
	{
		friend StringBase<T>;

	public:
		using IteratorT = T*;
		using ConstIteratorT = T const*;

		/**
		 * @brief Construct an empty array.
		 *
		 * This constructor does not allocate any space for
		 * the array.
		 */
		FORCE_INLINE ArrayBase()
			: buffer{nullptr}
			, size{0}
			, numItems{0}
			, allocator{gMalloc}
		{
			//
		}

		/**
		 * @brief Construct a new array and initialize one
		 * or more elements with an initial value.
		 *
		 * @param inNumItems the number of elements to
		 * initialize
		 * @param item the item used to initialize all items
		 * @param slack extra space reserved (in number of
		 * items)
		 */
		FORCE_INLINE ArrayBase(uint64 inNumItems, auto const& item, sizet slack = 0)
			: ArrayBase{}
		{
			KORIN_CHECK(inNumItems > 0)

			// Grow buffer and construct items
			growToFit(inNumItems + slack);
			constructItems(buffer, item, inNumItems);
			numItems = inNumItems;
		}

		/**
		 * @brief Construct a new array by copying the items
		 * from a source buffer.
		 *
		 * @param items ptr to items to copy
		 * @param inNumItems number of items to copy
		 * @param slack extra space to reserve (in number of
		 * items)
		 */
		FORCE_INLINE ArrayBase(T const* items, uint64 inNumItems, sizet slack = 0)
			: ArrayBase{}
		{
			KORIN_ASSERT(items != nullptr)
			KORIN_CHECK(inNumItems > 0)

			// Grow buffer and copy items
			growToFit(inNumItems + slack);
			copyConstructItems(buffer, items, inNumItems);
			numItems = inNumItems;
		}

		/**
		 * @brief Construct a new array by copying another
		 * array.
		 *
		 * @param other another array
		 */
		FORCE_INLINE ArrayBase(ArrayBase const& other)
			: ArrayBase{other, 0}
		{
			//
		}

		/**
		 * @brief Construct a new array by moving another
		 * array.
		 *
		 * @param other another array
		 */
		ArrayBase(ArrayBase&& other)
			: buffer{other.buffer}
			, size{other.size}
			, numItems{other.numItems}
			, allocator{other.allocator}
		{
			// Reset other
			other.buffer = nullptr;
			other.size = 0;
			other.numItems = 0;
			other.allocator = 0;
		}

		/**
		 * @brief Copy another array.
		 *
		 * @param other another array to copy
		 * @return ref to self
		 */
		ArrayBase& operator=(ArrayBase const& other)
		{
			// Grow buffer to fit other's items
			growToFit(other.numItems);

			if (other.numItems >= numItems)
			{
				// Copy items over existing items and construct the others
				copyItems(buffer, other.buffer, numItems);
				copyConstructItems(buffer + numItems, other.buffer + numItems, other.numItems - numItems);
			}
			else
			{
				// Copy items over existing items and destroy the others
				copyItems(buffer, other.buffer, other.numItems);
				copyConstructItems(buffer + other.numItems, other.buffer + other.numItems, numItems - other.numItems);
			}
			numItems = other.numItems;

			return *this;
		}

		/**
		 * @brief Move another array.
		 *
		 * @param other another array
		 * @return ref to self
		 */
		ArrayBase& operator=(ArrayBase&& other)
		{
			// Destroy this array
			destroy();

			buffer = other.buffer;
			size = other.size;
			numItems = other.numItems;
			allocator = other.allocator;

			other.buffer = nullptr;
			other.size = 0;
			other.numItems = 0;
			other.allocator = nullptr;

			return *this;
		}

		/**
		 * @brief Destroy this array.
		 */
		FORCE_INLINE ~ArrayBase()
		{
			// Destroy array
			destroy();
		}

		/**
		 * @brief Returns the number of items in the array.
		 */
		FORCE_INLINE sizet getLength() const
		{
			return numItems;
		}

		/**
		 * @brief Returns the size of the allocated buffer
		 * (in bytes).
		 *
		 * The size of the buffer may be larger than the
		 * number of items.
		 */
		FORCE_INLINE sizet getNumBytes() const
		{
			return size * sizeof(*buffer);
		}

		/**
		 * @brief Return a ptr to the allocated buffer.
		 * @{
		 */
		FORCE_INLINE T* operator*()
		{
			return buffer;
		}

		FORCE_INLINE T const* operator*() const
		{
			return *const_cast<ArrayBase&>(*this);
		}
		/** @} */

		/**
		 * @brief Return a referefence to the i-th item of
		 * this array.
		 *
		 * If the index is out of range, the behavior is
		 * undefined.
		 *
		 * @param idx index of the item
		 * @return ref to the i-th item
		 * @{
		 */
		FORCE_INLINE T& operator[](uint64 idx)
		{
			KORIN_CHECK(idx < numItems);
			return buffer[idx];
		}

		FORCE_INLINE T const& operator[](uint64 idx) const
		{
			return const_cast<ArrayBase&>(*this)[idx];
		}
		/** @} */

		/**
		 * @brief Compares the content of two arrays and
		 * returns true if all items are equal.
		 *
		 * @param other another array
		 * @return true if all items are equal
		 * @return false otherwise
		 */
		bool operator==(ArrayBase const& other) const
		{
			if (numItems != other.numItems)
				// Length must be the same
				return false;

			for (ssizet idx = 0; idx < numItems; ++idx)
			{
				if (buffer[idx] != other.buffer[idx])
				{
					// Items differ
					return false;
				}
			}

			// No difference
			return true;
		}

		/**
		 * @brief Returns true if the items of this array
		 * and the items of another array are not all equal.
		 *
		 * @param other another array
		 * @return true if not all items are equal
		 * @return false otherwise
		 */
		FORCE_INLINE bool operator!=(ArrayBase const& other) const
		{
			return !(*this == other);
		}

		/**
		 * @brief Construct a new item at the end of the
		 * array.
		 *
		 * This call may invalidate any reference to the
		 * array's items.
		 *
		 * @param createArgs arguments used to construct a
		 *                   new item
		 * @return iterator pointing to the new item
		 */
		IteratorT emplaceBack(auto&& ...createArgs)
		{
			// Grow buffer if necessary
			growToFit(numItems + 1);

			// Construct item and append to array
			auto* item = initItem(buffer + numItems, FORWARD(createArgs)...);
			numItems++;

			return {item};
		}

		/**
		 * @brief Construct a new item at the given position
		 * of the array.
		 *
		 * If the position is greater than the array's
		 * length, the item is still constructed at the
		 * given position, and the new number of items is
		 * adjusted accordingly. The items after the array's
		 * last item and before the new item are left
		 * uninitialized.
		 *
		 * This call may invalidate any reference to the
		 * array's items.
		 *
		 * @param idx position in the item
		 * @param createArgs
		 * @return IteratorT
		 */
		IteratorT emplaceAt(uint64 idx, auto&& ...createArgs)
		{
			if (idx == numItems)
			{
				// Push back
				return emplaceBack(FORWARD(createArgs)...);
			}

			// Grow buffer if necessary
			growToInsert(idx);

			// Construct item
			auto* item = initItem(buffer + idx, FORWARD(createArgs)...);
			numItems = max(numItems, idx) + 1;

			return {item};
		}

		/* IteratorT emplaceAt(ConstIteratorT pos, auto&& ...createArgs)
		{
			return emplaceAt(pos - buffer, FORWARD(createArgs)...);
		} */

		/* FORCE_INLINE IteratorT insertAt(ConstIterator pos, T const& item)
		{
			return emplaceAt(pos, item);
		}

		FORCE_INLINE IteratorT insertAt(ConstIterator pos, T&& item)
		{
			return emplaceAt(pos, move(item));
		} */

		/**
		 * @brief Append an item to the end of the array.
		 *
		 * @see emplaceBack
		 *
		 * @param item item to append
		 * @return iterator pointing to inserted item
		 * @{
		 */
		FORCE_INLINE IteratorT append(T const& item)
		{
			return emplaceBack(item);
		}

		FORCE_INLINE IteratorT append(T&& item)
		{
			return emplaceBack(move(item));
		}
		/** @} */

		/**
		 * @brief Insert an item at the given position of
		 * the array.
		 *
		 * @see emplaceAt
		 *
		 * @param idx position in the array
		 * @param item item to insert
		 * @return iterator pointing to inserted item
		 * @{
		 */
		FORCE_INLINE IteratorT insertAt(uint64 idx, T const& item)
		{
			return emplaceAt(idx, item);
		}

		FORCE_INLINE IteratorT insertAt(uint64 idx, T&& item)
		{
			return emplaceAt(idx, move(item));
		}
		/** @} */

		//IteratorT removeAt(ConstIteratorT pos);
		/**
		 * @brief Remove and return the last item of the
		 * array.
		 *
		 * If the array is empty, the result is undefined.
		 *
		 * @return last item of the array
		 */
		T pop()
		{
			KORIN_ASSERT(numItems != 0)

			// Move out item
			T& item = buffer[numItems - 1];
			T outItem{move(item)};

			// Destroy item and reduce size
			destroyItems(buffer + numItems - 1, 1);
			numItems--;

			return outItem;
		}

		/**
		 * @brief Removes from the array a single item at
		 * the given position and returns it.
		 *
		 * If the array is empty, or the position is beyond
		 * the last item of the array, the result is
		 * undefined.
		 *
		 * @param idx position of the item to remove
		 * @return the removed item
		 */
		T removeAt(uint64 idx)
		{
			KORIN_ASSERT(idx < numItems)

			// Move out item
			T& item = buffer[idx];
			T outItem{move(item)};

			// Remove item
			removeAt(idx, 1);

			return outItem;
		}

		/**
		 * @brief Removes from the array one or more items
		 * at the given position.
		 *
		 * If the array is empty, or the position is beyond
		 * the last item of the array, or the number of
		 * items to remove exceeds the number of items in
		 * the array (starting from the given position), the
		 * result is undefined.
		 *
		 * @param idx
		 * @param num
		 */
		void removeAt(uint64 idx, uint64 num)
		{
			KORIN_ASSERT(idx + num <= numItems)

			// move back next items and reduce size
			moveItems(buffer + idx, buffer + idx + num, numItems - (idx + num));
			numItems--;
		}

		/**
		 * @brief Removes all items from the array.
		 */
		FORCE_INLINE void clear()
		{
			// Destroy all items and reset num items
			destroyItems(buffer, numItems);
			numItems = 0;
		}

		/**
		 * @brief Reserve space for the desired number of
		 * items.
		 *
		 * @param desiredNumItems the desired number of
		 *                        items
		 */
		FORCE_INLINE void reserve(uint64 desiredNumItems)
		{
			return growToFit(desiredNumItems);
		}

		/**
		 * @brief Reduces the array allocation to fit the
		 * current number of items.
		 *
		 * @return true if the array buffer was reallocated
		 * @return false otherwise
		 */
		FORCE_INLINE bool shrinkToFit()
		{
			if (numItems == 0)
			{
				destroy();
				return true;
			}

			sizet const minSize = computeMinSize(numItems);

			if (size != minSize)
			{
				KORIN_CHECK(buffer != nullptr)

				// Allocate a new buffer and move items
				void* newBuffer = allocBuffer(minSize, allocator);
				moveConstructItems(newBuffer, buffer, numItems);
				freeBuffer(buffer);

				// Set new buffer and size
				buffer = newBuffer;
				size = minSize;

				return true;
			}

			return false;
		}

	protected:
		/* The buffer that contains the items of the array. */
		T* buffer;

		/* The size of the buffer (in number of items). */
		sizet size;

		/* The number of items in the array. */
		uint64 numItems;

		/* The allocator used by this array. */
		MallocBase* allocator;

		/**
		 * @brief Construct a new array with an initial
		 * capacity.
		 *
		 * @param initialSize initial size (in number of
		 *                    items)
		 */
		ArrayBase(uint64 initialSize)
			: ArrayBase{}
		{
			// Create initial buffer, but do not initialize items
			growToFit(initialSize);
		}

		/**
		 * @brief Construct a new array by copying another
		 * array, and reserve extra space.
		 *
		 * @param other the other array to copy
		 * @param slack extra space to reserve
		 */
		ArrayBase(ArrayBase const& other, uint64 slack)
			: ArrayBase{}
		{
			// Create buffer to fit other's items and copy them
			growToFit(other.numItems + slack);
			copyConstructItems(buffer, other.buffer, other.numItems);
			numItems = other.numItems;
		}

		/**
		 * @brief Remove all items, and deallocate the buffer
		 *
		 * @return FORCE_INLINE
		 */
		FORCE_INLINE void destroy()
		{
			if (buffer)
			{
				// Destroy items
				clear();

				// Destroy buffer
				freeBuffer(buffer, allocator);
				size = 0;
			}
		}

		/**
		 * @brief Construct an item of the array.
		 *
		 * @param item pointer to the item to construct
		 * @param createArgs arguments passed to the item
		 *                   constructor
		 * @return pointer to the item
		 */
		FORCE_INLINE T* initItem(T* item, auto&& ...createArgs)
		{
			return new(item) T{FORWARD(createArgs)...};
		}

		/**
		 * @brief Init zero or more consecutive items of the
		 * array.
		 *
		 * @param begin first item to construct
		 * @param item first item to copy/move
		 * @param items other items to copy/move
		 */
		FORCE_INLINE void initItems(T* begin, auto&& item, auto&& ...items)
		{
			// Init item in current position
			initItem(begin, FORWARD(item));

			if constexpr (sizeof...(items) != 0)
				// Init rest of items recursively
				initItems(begin + 1, FORWARD(items)...);
		}

		/**
		 * @brief Returns the minimum size the buffer needs
		 * to be to fit all the items.
		 *
		 * @param desiredNumItems the desired number of
		 *                        items to fit
		 * @return the minimum size of the buffer
		 */
		FORCE_INLINE sizet computeMinSize(uint64 desiredNumItems)
		{
			return PlatformMath::closestLargerPowerOf2(desiredNumItems);
		}

		/**
		 * @brief Grow the buffer of the array to insert one
		 * or more items at the given position.
		 *
		 * @param idx insert position in the array
		 * @param num number of items to insert
		 * @return true if buffer was resized
		 * @return false otherwise
		 */
		bool growToInsert(uint64 idx, uint64 num = 1)
		{
			KORIN_CHECK(idx < count);

			sizet const newSize = computeMinSize(numItems + num);
			if (newSize != size)
			{
				// Allocate a new buffer
				T* newBuffer = allocBuffer(newSize, allocator);
				KORIN_ASSERT(newBuffer != nullptr)

				if (buffer)
				{
					// Move items before and after insert index
					moveConstructItems(newBuffer, buffer, idx);
					moveConstructItems(newBuffer + idx + num, buffer + idx, numItems - idx);
					freeBuffer(buffer, allocator);
				}

				// Set new buffer and size
				buffer = newBuffer;
				size = newSize;

				return true;
			}

			// Just move items after insert idx
			// The last num items must be constructed, the others just moved
			moveConstructItems(buffer + numItems, buffer + numItems - num, num);
			moveItems(buffer + idx + num, buffer + idx, numItems - idx - num);

			return false;
		}

		/**
		 * @brief Grow buffer to fit the desired number of
		 * items.
		 *
		 * @param desiredNumItems number of items to fit
		 * @return true if buffer was resized
		 * @return false otherwise
		 */
		bool growToFit(uint64 desiredNumItems)
		{
			sizet const newSize = computeMinSize(desiredNumItems);
			if (newSize != size)
			{
				// Allocate new buffer
				T* newBuffer = allocBuffer(newSize, allocator);
				KORIN_ASSERT(newBuffer != nullptr)

				if (buffer)
				{
					// Move items to new buffer
					moveConstructItems(newBuffer, buffer, numItems);
					freeBuffer(buffer, allocator);
				}

				// Set new buffer and size
				buffer = newBuffer;
				size = newSize;

				return true;
			}

			return false;
		}

	private:
		/* Allocate a new buffer, using the item alignment. */
		static T* allocBuffer(sizet size, MallocBase* allocator)
		{
			void* mem = allocator->malloc(size * sizeof(T), max(alignof(T), MIN_ALIGNMENT));
			return reinterpret_cast<T*>(mem);
		}

		/* Free buffer. */
		static void freeBuffer(T* buffer, MallocBase* allocator)
		{
			allocator->free(buffer);
		}
	};


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
