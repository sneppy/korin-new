#pragma once

#ifdef PLATFORM_WINDOWS
# define PLATFORM_WINDOWS 1
#else
# define PLATFORM_WINDOWS 0
#endif

#ifdef PLATFORM_APPLE
# define PLATFORM_APPLE 1
#else
# define PLATFORM_APPLE 0
#endif

#ifdef PLATFORM_LINUX
# define PLATFORM_LINUX 1
#else
# define PLATFORM_LINUX 0
#endif

#if PLATFORM_WINDOWS
# include "windows/platform.h"
#elif PLATFORM_APPLE
# include "apple/platform.h"
#elif PLATFORM_LINUX
# include "linux/platform.h"
#else
# error "Unknown platform"
#endif

// Declare all C++ types in global scope
using int8 = PlatformTypes::int8;
using int16 = PlatformTypes::int16;
using int32 = PlatformTypes::int32;
using int64 = PlatformTypes::int64;

using uint8 = PlatformTypes::uint8;
using uint16 = PlatformTypes::uint16;
using uint32 = PlatformTypes::uint32;
using uint64 = PlatformTypes::uint64;

using byte = PlatformTypes::byte;
using ubyte = PlatformTypes::ubyte;

using intp = PlatformTypes::intp;
using uintp = PlatformTypes::uintp;
using ssizet = PlatformTypes::ssizet;
using sizet = PlatformTypes::sizet;

using float32 = PlatformTypes::float32;
using float64 = PlatformTypes::float64;

using ansichar = PlatformTypes::ansichar;
using widechar = PlatformTypes::widechar;
