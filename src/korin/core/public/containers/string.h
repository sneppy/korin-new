#pragma once

#include "templates/types.h"
#include "hal/platform_memory.h"
#include "hal/platform_string.h"
#include "containers_types.h"
#include "array.h"

// TODO: Just to see how it's done, replace with Korin::Tuple
#include <tuple>

namespace Korin
{
	/**
	 * @brief Base class for string types.
	 * 
	 * @tparam CharT the type of the characters
	 */
	template<typename CharT>
	class StringBase
	{
		static_assert(IsIntegral<CharT>::value, "Char type must be an integral value");

		static constexpr CharT termChar{0};
		
		/**
		 * @brief Helper class that computes the source
		 * and length of any string source (e.g. C strings,
		 * buffer and length, static strings, Korin::String)
		 */
		struct StringSource
		{
			/* Pointer to the string buffer. */
			CharT const* const src;

			/* Length of the string (excluding
			   any terminating character). */
			sizet const len;

			/**
			 * @brief Accept a buffer with length.
			 * 
			 * @param inSrc pointer to string source
			 * @param inLen length of the string
			 */
			constexpr FORCE_INLINE StringSource(CharT const* const inSrc, sizet inLen)
				: src{inSrc}
				, len{inLen}
			{
				//
			}

			/**
			 * @brief Accept a C string.
			 * 
			 * @param cstr pointer to a null-terminated
			 * string
			 */
			constexpr FORCE_INLINE StringSource(CharT const* const cstr)
				: src{cstr}
				, len{PlatformString::len(cstr)}
			{
				//
			}

			/**
			 * @brief Accept a static array.
			 * 
			 * @tparam len length of the string
			 * @param str pointer to the array
			 * @return constexpr FORCE_INLINE 
			 */
			template<sizet len>
			constexpr FORCE_INLINE StringSource(CharT const str[len])
				: StringSource{str, len}
			{
				//
			}

			/**
			 * @brief Accept a managed string.
			 * 
			 * @param other a managed string 
			 */
			constexpr FORCE_INLINE StringSource(StringBase const& other)
				: StringSource{*other, other.getLen()}
			{
				//
			}

		private:
			StringSource() = delete;
		};

	public:
		/**
		 * @brief Construct an empty string.
		 */
		FORCE_INLINE StringBase()
			: array{1, termChar}
		{
			//
		}

		/**
		 * @brief Construct an empty string from
		 * any string source.
		 * 
		 * @param src string source
		 */
		FORCE_INLINE StringBase(StringSource const& src)
			: array{src.len + 1, termChar}
		{
			// Copy source string
			PlatformMemory::memcpy(*array, src.src, src.len);
		}

		/**
		 * @brief Construct a new string from a
		 * null-terminated string.
		 * 
		 * @param cstr pointer to C string
		 */
		FORCE_INLINE StringBase(CharT const* cstr)
			: StringBase{StringSource{cstr}}
		{
			//
		}

		/**
		 * @brief Construct a new string by reading
		 * @c len characters from source.
		 * 
		 * @param src pointer to buffer to read from
		 * @param len number of characters to read
		 */
		FORCE_INLINE StringBase(CharT const* src, sizet len)
			: StringBase{StringSource{src, len}}
		{
			//
		}

		/**
		 * @brief Construct a new string from a
		 * static array.
		 * 
		 * @tparam len length of the string
		 * @param src pointer to array
		 */
		template<sizet len>
		FORCE_INLINE StringBase(CharT const src[len])
			: StringBase{StringSource{src, len}}
		{
			//
		}

		/**
		 * @brief Cosntruct a string by repeating
		 * the given character @c n times.
		 * 
		 * @param c character to repeat
		 * @param n number of repeats
		 */
		FORCE_INLINE explicit StringBase(CharT const& c, sizet n)
			: array{n + 1, c}
		{
			// Terminate the string
			terminate();
		}

		/**
		 * @brief Returns the length of the string.
		 */
		FORCE_INLINE sizet getLen() const
		{
			// Exclude terminating character
			return array.getNumItems() - 1;
		}

		/**
		 * @brief Returns a ref to the i-th
		 * character of the string.
		 * 
		 * @param idx index of the character
		 * @return ref to character
		 * @{
		 */
		FORCE_INLINE CharT const& operator[](int32 idx) const
		{
			CHECK(idx < getLen())
			return array[idx];
		}

		FORCE_INLINE CharT& operator[](int32 idx)
		{
			CHECK(idx < getLen())
			return array[idx];
		}
		/** @} */

		/**
		 * @brief Returns a pointer to the
		 * null-terminated C string.
		 * @{
		 */
		FORCE_INLINE CharT const* operator*() const
		{
			return *array;
		}

		FORCE_INLINE CharT* operator*()
		{
			return *array;
		}
		/** @} */

		/**
		 * @brief Compare two strings.
		 * 
		 * @param other another string
		 * @return true if strings are equal
		 * @return false otherwise
		 */
		FORCE_INLINE bool operator==(StringSource const& other) const
		{
			return PlatformString::cmpn(*array, other.src, other.len) == 0;
		}

		/**
		 * @brief Compare two strings.
		 * 
		 * @param other another string
		 * @return true if strings are not equal
		 * @return false otherwise
		 */
		FORCE_INLINE bool operator!=(StringSource const& other) const
		{
			return !(*this == other);
		}

		/**
		 * @brief Returns true if this string precedes
		 * the other string in alphabetical order.
		 * 
		 * @param other another string
		 * @return true if this string precedes other
		 * @return false otherwise
		 */
		FORCE_INLINE bool operator<(StringSource const& other) const
		{
			return PlatformString::cmpn(*array, other.src, other.len) < 0;
		}

		/**
		 * @brief Returns true if this string succeeds
		 * the other string in alphabetical order.
		 * 
		 * @param other another string
		 * @return true if this string succeeds other
		 * @return false otherwise
		 */
		FORCE_INLINE bool operator>(StringSource const& other) const
		{
			return PlatformString::cmpn(*array, other.src, other.len) > 0;
		}

		/**
		 * @brief Returns true if this string is
		 * equal or preceds the other string.
		 * 
		 * @param other another string
		 * @return true if equal or precedes other
		 * string
		 * @return false otherwise
		 */
		FORCE_INLINE bool operator<=(StringSource const& other) const
		{
			return !(*this > other);
		}

		/**
		 * @brief Returns true if this string is
		 * equal or succeeds the other string.
		 * 
		 * @param other another string
		 * @return true if equal or succeeds other
		 * string
		 * @return false otherwise
		 */
		FORCE_INLINE bool operator>=(StringSource const& other) const
		{
			return !(*this < other);
		}

		/**
		 * @brief Append another string source to this
		 * string.
		 * 
		 * @param other any string source
		 * @return ref to self
		 */
		StringBase& operator+=(StringSource const& other)
		{
			ASSERT(other.src != nullptr)

			// Grow array if necessary
			array.growToFit(array.count + other.len);

			// Copy data
			copyItems(*array + getLen(), other.src, other.len);
			array.count += other.len;
			terminate();

			return *this;
		}

		/**
		 * @brief Return a new string equal to the
		 * result of concatenating two string sources.
		 * 
		 * @param lhs,rhs any two string sources
		 * @return new string
		 */
		friend StringBase operator+(StringSource const& lhs, StringSource const& rhs)
		{
			ASSERT(lhs.src != nullptr)
			ASSERT(rhs.src != nullptr)

			// Create new string
			StringBase newString{lhs.len + rhs.len};

			// Copy characters
			copyItems(*newString, lhs.src, lhs.len);
			copyItems(*newString + lhs.len, rhs.src, rhs.len);

			return newString;
		}

		/**
		 * @brief Format a string source with the
		 * given format arguments.
		 * 
		 * The format string uses the usual C
		 * format placeholders.
		 * 
		 * TODO: Replace @c std::tuple with @c Korin::Tuple
		 * 
		 * @tparam FormatArgsT the type of the
		 * format arguments
		 * @param fmt format string source
		 * @param args tuple of format arguments
		 * @return new formatted string
		 */
		template<typename ...FormatArgsT>
		friend FORCE_INLINE StringBase operator%(StringSource const& fmt, std::tuple<FormatArgsT...> const& args)
		{
			return StringBase::format_Impl(fmt, args, std::index_sequence_for<FormatArgsT...>{});
		}

	protected:
		/**
		 * @brief Create an empty string, with an
		 * initial array size equal to the given
		 * string length.
		 * 
		 * @param len future length of the string
		 */
		FORCE_INLINE StringBase(sizet len)
			: array{len + 1, termChar}
		{
			//
		}

		/**
		 * @brief Append the terminating character
		 * to the end of the string.
		 */
		FORCE_INLINE void terminate()
		{
			array[getLen()] = CharT{0};
		}

		/* The array that holds the string characters. */
		Array<CharT> array;
	
	private:
		/**
		 * @brief Private implementation to format
		 * a string.
		 * 
		 * @tparam idxs index sequence used to iterate
		 * over arguments tuple 
		 * @param fmt format string
		 * @param args tuple of format arguments
		 * @return new string
		 */
		template<typename std::size_t ...idxs>
		static StringBase format_Impl(StringSource const& fmt, auto const& args, std::index_sequence<idxs...>)
		{
			// We don't know a priori the length of the
			// formatted string.
			const sizet newLen = ::snprintf(nullptr, 0, fmt.src, prepareFormatArg(std::get<idxs>(args))...);
			
			// Create new string
			StringBase newString{newLen};

			// Format string
			[[maybe_unused]] sizet printLen = ::snprintf(*newString, newLen + 1, fmt.src, prepareFormatArg(std::get<idxs>(args))...);
			KORIN_ASSERT(printLen == newLen)
			
			return newString;
		}

		/**
		 * @brief Prepares format argument.
		 * 
		 * For most argument types returns the
		 * argument as-is.
		 * 
		 * For string arguments return pointer
		 * to the null-terminated string.
		 * 
		 * @param arg format argument
		 * @return argument ready for format 
		 * @{
		 */
		static constexpr FORCE_INLINE auto const& prepareFormatArg(auto const& arg)
		{
			// Return as is
			return arg;
		}

		static constexpr FORCE_INLINE CharT const* prepareFormatArg(StringBase const& arg)
		{
			// Return pointer to the cstr
			return *arg;
		}
		/** @} */
	};
} // namespace Korin
