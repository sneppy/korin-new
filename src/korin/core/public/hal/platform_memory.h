#pragma once

#include "core_types.h"

#if PLATFORM_WINDOWS
#	include "windows/platform_memory.h"
#elif PLATFORM_APPLE
#	include "apple/platform_memory.h"
#elif PLATFORM_LINUX
#	include "linux/platform_memory.h"
#else
#	warning "Unknown platform"
#endif

#include "malloc.h"
