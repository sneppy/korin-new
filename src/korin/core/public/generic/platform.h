#pragma once

/**
 * @brief Sets Type to the appropriate type
 * according to the pointer type size.
 * 
 * @tparam T32 Type to set for 32-bits
 * @tparam T64 Type to set for 64-bits
 * @tparam n Size (in Bytes) of a pointer
 */
template<typename T32, typename T64, unsigned long long n>
struct SelectIntPointerType
{
	//
};

template<typename T32, typename T64> struct SelectIntPointerType<T32, T64, 4> { using Type = T32; };
template<typename T32, typename T64> struct SelectIntPointerType<T32, T64, 8> { using Type = T64; };

/**
 * @brief Generic basic C++ types.
 */
struct GenericPlatformTypes
{
	using int8 = signed char;
	using int16 = signed short;
	using int32 = signed int;
	using int64 = signed long long;

	using uint8 = unsigned char;
	using uint16 = unsigned short;
	using uint32 = unsigned int;
	using uint64 = unsigned long long;	

	using byte = signed char;
	using ubyte = unsigned char;

	using intp = typename SelectIntPointerType<int32, int64, sizeof(void*)>::Type;
	using uintp = typename SelectIntPointerType<uint32, uint64, sizeof(void*)>::Type;
	using ssizet = intp;
	using sizet = uintp;

	using float32 = float;
	using float64 = double;

	using ansichar = char;
	using widechar = wchar_t;
};