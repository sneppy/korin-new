#pragma once

#include "unix/platform_memory.h"

/**
 * @brief Linux memory abstraction layer.
 */
struct LinuxPlatformMemory : public UnixPlatformMemory
{
	//
};

using PlatformMemory = LinuxPlatformMemory;
