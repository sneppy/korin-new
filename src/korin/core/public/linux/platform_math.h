#pragma once

#include "unix/platform_math.h"

/**
 * @brief Common math functions implementation
 * for UNIX systems.
 */
struct LinuxPlatformMath : public UnixPlatformMath
{
	//
};

using PlatformMath = LinuxPlatformMath;
