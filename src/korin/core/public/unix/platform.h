#pragma once

#include "generic/platform.h"

/**
 * @brief Unix-platform C++ types.
 */
struct UnixPlatformTypes : public GenericPlatformTypes
{
	//
};

#include "misc/build.h"

#define PLATFORM_POSIX 1
#define PLATFORM_USE_GCEM_LIB 1

#if BUILD_RELEASE && defined(__GNUC__)
# define FORCE_INLINE inline __attribute__((always_inline))
#endif

#ifdef __GNUC__
# define UNLIKELY(x) __builtin_expect(!!(x), 0)
# define LIKELY(x) __builtin_expect(!!(x), 1)
# define RESTRICT __restrict__
# define LOAD_DEBUG_SCRIPT(filename, type) asm(".pushsection \".debug_gdb_scripts\", \"MS\", @progbits, 1\n"\
                                               ".byte " type "\n"\
											   ".asciz \"" filename "\"\n"\
											   ".popsection\n");
# define GDB_SCRIPT_PYTHON_FILE "1"
# define GDB_SCRIPT_PYTHON_INLINE "3"
# define GDB_SCRIPT_SCHEME_FILE "5"
# define GSB_SCRIPT_SCHEME_INLINE "7"
#endif
