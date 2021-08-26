#pragma once

#include "hal/platform_memory.h"
#include "templates/utility.h"
#include "containers_types.h"

#ifndef PLATFORM_MATH_MAX
#	define PLATFORM_MATH_MAX
struct PlatformMath
{
	template<typename T>
	static constexpr FORCE_INLINE T max(T const& x, auto const& y)
	{
		return x > y ? x : y;
	}
};

#endif

namespace Containers
{
	/**
	 * @brief Iterator class for @c CircularQueue.
	 * 
	 * @tparam T the type of the queue
	 */
	template<typename T, bool isConst>
	struct CircularQueueIterator
	{
		using QueueT = typename ChooseType<isConst, CircularQueue<T> const, CircularQueue<T>>::Type;
		using RefT = T&;
		using PtrT = T*;
		
		FORCE_INLINE CircularQueueIterator(QueueT& inQueue, uint64 startingIdx)
			: queue{inQueue}
			, idx{startingIdx}
		{
			//
		}

		FORCE_INLINE RefT operator*() const
		{
			return queue.buffer[idx];
		}

		FORCE_INLINE PtrT operator->() const
		{
			return &(**this);
		}

		FORCE_INLINE bool operator==(CircularQueueIterator const& other) const
		{
			CHECK(&queue == &other.queue)
			return idx == other.idx;
		}

		FORCE_INLINE bool operator!=(CircularQueueIterator const& other) const
		{
			return !(*this == other);
		}

		FORCE_INLINE CircularQueueIterator& operator++()
		{
			if (idx++ == queue.size)
				idx = 0;
			
			return *this;
		}

		FORCE_INLINE CircularQueueIterator operator++(int32) const
		{
			CircularQueueIterator self{this};
			*this++;
			return self;
		}

		FORCE_INLINE CircularQueueIterator& operator--()
		{
			idx = idx == 0
				? queue.size - 1
				: idx - 1;
			
			return *this;
		}

		FORCE_INLINE CircularQueueIterator operator--(int32) const
		{
			CircularQueueIterator self{this};
			*this--;
			return self;
		}

		FORCE_INLINE CircularQueueIterator& operator+=(int32 dist)
		{
			if (dist < 0)
			{
				return *this -= -dist;
			}
			
			// Wrap index if necessary
			for (idx += dist; idx >= queue.size; idx -= queue.size);
			return *this;
		}

		FORCE_INLINE CircularQueueIterator operator+(int32 dist)
		{
			return CircularQueueIterator{*this} += dist;
		}

		FORCE_INLINE CircularQueueIterator& operator-=(int32 dist)
		{
			if (dist < 0)
			{
				return *this += -dist;
			}
			
			// Wrap index if necessary
			while (dist > idx) dist -= queue.size;
			idx -= dist;
			return *this;			
		}

		FORCE_INLINE CircularQueueIterator operator-(int32 dist)
		{
			return CircularQueueIterator{*this} -= dist;
		}

	private:
		/* Ref to queue instance. */
		QueueT& queue;
	
		/* Index of item pointed by iterator. */
		uint64 idx;
	};

	/**
	 * @brief 
	 * 
	 * @tparam T the type of the items
	 */
	template<typename T>
	class CircularQueue
	{
		friend CircularQueueIterator<T, true>;
		friend CircularQueueIterator<T, false>;

		using IteratorT = CircularQueueIterator<T, false>;
		using ConstIteratorT = CircularQueueIterator<T, true>;

		/**
		 * @brief Destroy buffer and all items.
		 */
		void destroy()
		{
			if (buffer)
			{
				if (insertIdx > extractIdx)
				{
					// Destroy items
					destroyItems(buffer + extractIdx, insertIdx - extractIdx);
				}
				else if (insertIdx < extractIdx)
				{
					// Destroy items
					destroyItems(buffer + extractIdx, size - extractIdx);
					destroyItems(buffer, insertIdx);
				}
				
				// Free buffer
				gMalloc->free(buffer);
				size = 0;
			}

			extractIdx = insertIdx = 0;
		}

		/**
		 * @brief Resize buffer, rearrange items
		 * accordingly.
		 * 
		 * @param newSize new buffer size
		 */
		void resize(sizet newSize)
		{
			CHECK(newSize > getNumItems())

			// Skip if nothing to resize
			if (size == newSize) return;

			// Alloc new buffer
			constexpr sizet alignment = PlatformMath::max(alignof(T), MIN_ALIGNMENT);
			T* newBuffer = reinterpret_cast<T*>(gMalloc->malloc(newSize * sizeof(T), alignment));

			if (buffer)
			{
				if (extractIdx < insertIdx)
				{
					// Move items
					moveItems(newBuffer + extractIdx, buffer + extractIdx, insertIdx - extractIdx);
				}
				else if (extractIdx > insertIdx)
				{
					// Unwrap if possible
					sizet newInsertIdx = size + insertIdx;

					if (newInsertIdx < newSize)
					{
						// Move items
						moveItems(newBuffer + extractIdx, buffer + extractIdx, size - extractIdx);
						moveItems(newBuffer + size, buffer, insertIdx);
					}
					else
					{
						newInsertIdx = newInsertIdx - newSize;

						// Move Items
						moveItems(newBuffer + extractIdx, buffer + extractIdx, size - extractIdx);
						moveItems(newBuffer + size, buffer, insertIdx - newInsertIdx);
						moveItems(newBuffer, buffer + (insertIdx - newInsertIdx), newInsertIdx);
					}

					insertIdx = newInsertIdx;
				}

				// Free buffer
				gMalloc->free(buffer);
			}

			buffer = newBuffer;
			size = newSize;
		}

	public:
		/**
		 * @brief Construct an empty queue with
		 * null capacity.
		 */
		FORCE_INLINE CircularQueue()
			: buffer{nullptr}
			, size{0}
			, extractIdx{0}
			, insertIdx{0}
		{
			//
		}

		/**
		 * @brief Construct an empty queue with
		 * the given max. capacity.
		 * 
		 * @param inSize size of the queue
		 */
		FORCE_INLINE CircularQueue(sizet inSize)
			: CircularQueue{}
		{
			// Create buffer
			resize(inSize);
		}

		/**
		 * @brief Construct a copy of another buffer.
		 * 
		 * @param other another buffer to copy
		 */
		FORCE_INLINE CircularQueue(CircularQueue const& other)
			: CircularQueue{}
		{
			if (other.buffer)
			{
				resize(other.size);
				extractIdx = other.extractIdx;
				insertIdx = other.insertIdx;
			}
		}

		/**
		 * @brief Construct buffer by moving another
		 * buffer.
		 * 
		 * @param other another buffer to move
		 */
		FORCE_INLINE CircularQueue(CircularQueue&& other)
			: buffer{other.buffer}
			, size{other.size}
			, extractIdx{other.extractIdx}
			, insertIdx{other.insertIdx}
		{
			other.buffer = nullptr;
			other.size = 0;
			other.extractIdx = other.insertIdx = 0;
		}

		// TODO: Copy assignment and move assignment

		FORCE_INLINE ~CircularQueue()
		{
			destroy();
		}

		/**
		 * @brief Returns the number of items in
		 * the queue.
		 */
		FORCE_INLINE sizet getNumItems() const
		{
			return extractIdx > insertIdx
				? size - (extractIdx - insertIdx)
				: insertIdx - extractIdx;
		}

		/**
		 * @brief Return an iterator that points
		 * to the beginning of the queue.
		 * @{
		 */
		ConstIteratorT begin() const
		{
			return {*this, extractIdx};
		}

		IteratorT begin()
		{
			return {*this, extractIdx};
		}
		/** @} */

		/**
		 * @brief Append one or more items to
		 * the end of the queue.
		 * 
		 * @param items items to append to the
		 * queue
		 */
		FORCE_INLINE void push(auto&& item, auto&& ...items)
		{
			// Construct item
			new (buffer + insertIdx) T{FORWARD(item)};
			
			// Update insert idx
			if (insertIdx++ > size)
				insertIdx = 0;
			
			if constexpr (sizeof...(items) > 0)
			{
				// Push next items
				push(FORWARD(items)...);
			}
		}

		/**
		 * @brief Pop the first item from the
		 * queue.
		 * 
		 * @param outItem return the item value
		 */
		FORCE_INLINE bool pop(T& outItem)
		{
			if (extractIdx != insertIdx)
			{
				// Move item
				outItem = move(buffer[extractIdx]);

				if (extractIdx++ > size)
					extractIdx = 0;
				
				return true;
			}

			return false;
		}

		FORCE_INLINE bool pop()
		{
			T _;
			return pop(_);
		}
		/** @} */

	protected:
		/* Ptr to the beginning of the queue. */
		T* buffer;

		/* Size of the allocated buffer. */
		sizet size;

		/* Index of next item to pop. */
		uint64 extractIdx;

		/* Index of next item to insert. */
		uint64 insertIdx;
	};
} // namespace Containers

