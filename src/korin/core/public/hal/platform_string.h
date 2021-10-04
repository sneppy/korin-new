#pragma once

#include "core_types.h"

#if PLATFORM_WINDOWS
#	include "windows/platform_string.h"
#elif PLATFORM_APPLE
#	include "apple/platform_string.h"
#elif PLATFORM_LINUX
#	include "linux/platform_string.h"
#else
#	warning "Unknown platform"
#endif
