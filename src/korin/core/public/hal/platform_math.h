#pragma once

#include "core_types.h"

#if PLATFORM_WINDOWS
#	include "windows/platform_math.h"
#elif PLATFORM_APPLE
#	include "apple/platform_math.h"
#elif PLATFORM_LINUX
#	include "linux/platform_math.h"
#else
#	error "Unknown platform"
#endif
