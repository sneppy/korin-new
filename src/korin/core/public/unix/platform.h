#pragma once

#include "generic/platform.h"

/**
 * @brief Unix-platform C++ types.
 */
struct UnixPlatformTypes : GenericPlatformTypes
{
	//
};

#include "misc/build.h"

#define PLATFORM_POSIX 1

#if BUILD_RELEASE && defined(__GNUC__)
#	define FORCE_INLINE inline __attribute__((always_inline))
#endif

#ifdef __GNUC__
#	define UNLIKELY(x) __builtin_expect(!!(x), 0)
#	define LIKELY(x) __builtin_expect(!!(x), 1)
#	define RESTRICT __restrict__
#endif
