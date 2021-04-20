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

#ifdef __GNUC__
#	if BUILD_RELEASE
# 		define FORCE_INLINE inline __attribute__((always_inline))
#	else
#		define FORCE_INLINE inline
#	endif
#endif
