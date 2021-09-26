#pragma once

#include "templates/utility.h"
#include "containers_types.h"

namespace Korin
{
	/**
	 * @brief Container for optional values.
	 * 
	 * @tparam T the type of the value
	 */
	template<typename T>
	struct Optional
	{
		/**
		 * @brief Contruct a new optional without value.
		 */
		FORCE_INLINE Optional()
			: _{}
			, initd{false}
		{
			//
		}

		/**
		 * @brief Construct a new optional and initialize
		 * it with a copy of the given value.
		 * 
		 * @param inValue value to copy 
		 */
		FORCE_INLINE Optional(T const& inValue)
			: value{inValue}
			, initd{true}
		{
			//
		}

		/**
		 * @brief Contruct a new optional by moving
		 * the given value.
		 * 
		 * @param inValue value to move
		 */
		FORCE_INLINE Optional(T&& inValue)
			: value{move(inValue)}
			, initd{true}
		{
			//
		}

		/**
		 * @brief Copy construct optional from another
		 * optional.
		 * 
		 * @param other other optional
		 */
		FORCE_INLINE Optional(Optional const& other)
		{
			if ((initd = other.initd))
			{
				// Copy-initialize value
				new (&value) T{other.value};
			}
		}

		/**
		 * @brief Move construct optional from another
		 * optional.
		 * 
		 * @param other another optional 
		 */
		FORCE_INLINE Optional(Optional&& other)
		{
			if ((initd = other.initd))
			{
				// Move-initialize value
				new (&value) T{move(other.value)};
			}
		}

		/**
		 * @brief Copy assign a new value to this
		 * optional.
		 * 
		 * @param newValue new value to copy
		 */
		FORCE_INLINE Optional& operator=(T const& newValue)
		{
			if (initd)
			{
				// Copy value
				value = newValue;
			}
			else
			{
				// Copy-initialize value
				new (&value) T{newValue};
				initd = true;
			}

			return *this;
		}

		/**
		 * @brief Move assign a new value to this
		 * optional.
		 * 
		 * @param newValue new value to move
		 */
		FORCE_INLINE Optional& operator=(T&& newValue)
		{
			if (initd)
			{
				// Move value
				value = move(newValue);
			}
			else
			{
				// Move-initialize value
				new (&value) T{move(newValue)};
				initd = true;
			}

			return *this;
		}

		/**
		 * @brief Copy assign another optional to
		 * this optional.
		 * 
		 * @param other another optional to copy
		 */
		FORCE_INLINE Optional& operator=(Optional const& other)
		{
			if (!other.initd && this->initd)
			{
				// Destroy value
				value.~T();
				initd = false;
			}
			else if (other.initd && this->initd)
			{
				// Copy value
				value = other.value;
			}
			else if (other.initd && !this->initd)
			{
				// Copy-initialize value
				new (&value) T{other.value};
				initd = true;
			}
			else // if (!other.initd && !this->initd)
				;

			return *this;
		}

		/**
		 * @brief Move assign another optional to
		 * this optional.
		 * 
		 * @param other another optional to move
		 */
		FORCE_INLINE Optional& operator=(Optional&& other)
		{
			if (!other.initd && this->initd)
			{
				// Destroy value
				value.~T();
				initd = false;
			}
			else if (other.initd && this->initd)
			{
				// Move value
				value = move(other.value);
				other.initd = false;
			}
			else if (other.initd && !this->initd)
			{
				// Move-initialize value
				new (&value) T{move(other.value)};
				this->initd = true;
				other.initd = false;
			}
			else // if (!other.initd && !this->initd)
				;
			
			return *this;
		}

		/**
		 * @brief Destroy optional.
		 * 
		 * Destroys the optional value if present.
		 */
		FORCE_INLINE ~Optional()
		{
			if (initd)
			{
				// Destroy value
				value.~T();
				initd = false;
			}
		}

		/**
		 * @brief Returns a ref to the value.
		 * 
		 * Check that the value is present
		 * before using it.
		 * @{
		 */
		FORCE_INLINE T const& operator*() const
		{
			CHECKF(initd, "Value of Optional is not initialized");
			return value;
		}

		FORCE_INLINE T& operator*()
		{
			CHECKF(initd, "Value of Optional is not initialized");
			return value;
		}

		FORCE_INLINE operator T const&() const
		{
			return **this;
		}
		
		FORCE_INLINE operator T&()
		{
			return **this;
		}
		/** @} */

		/**
		 * @brief Returns a pointer to the contained
		 * value, or nullptr if empty.
		 * @{
		 */
		FORCE_INLINE T const* getPtr() const
		{
			return initd ? &value : nullptr;
		}

		FORCE_INLINE T* getPtr()
		{
			return initd ? &value : nullptr;
		}
		/** @} */

		/**
		 * @brief Returns a ptr to the contained
		 * value, in order to access its members.
		 * 
		 * Make sure the value is present before
		 * using it.
		 * @{
		 */
		FORCE_INLINE T const* operator->() const
		{
			return getPtr();
		}

		FORCE_INLINE T* operator->()
		{
			return getPtr();
		}
		/** @} */

		/**
		 * @brief Returns true if the optional is
		 * not empty.
		 */
		FORCE_INLINE bool hasValue() const
		{
			return initd;
		}

		/**
		 * @brief Reset the value of the optional.
		 */
		FORCE_INLINE void reset()
		{
			if (initd)
			{
				// Destroy value
				value.~T();
				initd = false;
			}
		}

		/**
		 * @brief Returns true if the optional is
		 * not empty.
		 * @{
		 */
		FORCE_INLINE operator bool() const
		{
			return initd;
		}

		FORCE_INLINE operator bool()
		{
			return initd;
		}
		/** @} */

	protected:
		union
		{
			/* Value of the container if present. */
			T value;

			/* Value placeholder. */
			byte _;
		};
		
		/* True if value if this optional has a value. */
		bool initd;
	};
} // namespace Korin

