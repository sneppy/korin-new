#pragma once

#include "misc/build.h"

#if PLATFORM_WINDOWS
# include "windows/platform.h"
#elif PLATFORM_APPLE
# include "apple/platform.h"
#elif PLATFORM_LINUX
# include "linux/platform.h"
#else
# error "Unknown platform"
#endif

#ifndef PLATFORM_POSIX
# define PLATFORM_POSIX 0
#endif

#ifndef FORCE_INLINE
# define FORCE_INLINE inline
#endif

#ifndef UNLIKELY
# define UNLIKELY(x) (x)
#endif

#ifndef LIKELY
# define LIKELY(x) (x)
#endif

#ifndef RESTRICT
# define RESTRICT restrict
#endif

#ifndef LOAD_DEBUG_SCRIPT
# define LOAD_DEBUG_SCRIPT
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
