#pragma once

#include "generic/platform_math.h"

/**
 * @brief Common math functions implementation
 * for UNIX systems.
 */
struct UnixPlatformMath : public GenericPlatformMath
{
#ifdef __GNUC__
	static constexpr FORCE_INLINE uint64 log2i(uint64 n)
	{
		return 63ULL - __builtin_clzll(n);
	}

	static constexpr FORCE_INLINE uint64 parity(uint64 n)
	{
		return __builtin_parityll(n);
	}

	static constexpr FORCE_INLINE uint64 closestSmallerPowerOf2(uint64 n)
	{
		return 1ull << log2i(n);
	}

	static constexpr FORCE_INLINE uint64 closestLargerPowerOf2(uint64 n)
	{
		return 1ull << (log2i(n - 1) + 1);
	}
#endif
};
