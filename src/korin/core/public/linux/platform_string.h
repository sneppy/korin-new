#pragma once

#include "unix/platform_string.h"

struct LinuxPlatformString : public UnixPlatformString
{
	//
};

using PlatformString = LinuxPlatformString;
