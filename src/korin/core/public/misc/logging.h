#pragma once

#include "build.h"

#define TERM_COLOR_RESET "\u001b[0m"
#define TERM_COLOR_BRIGHT_RED "\u001b[31;1m"
#define TERM_COLOR_BRIGHT_YELLOW "\u001b[33;1m"

// TODO: Change to KORIN_ASSERT* and KORIN_CHECK*
#if BUILD_RELEASE
#	define KORIN_ASSERTF(x, fmt, ...)
#	define KORIN_CHECKF(x, fmt, ...)
#else
#	define KORIN_ASSERTF(x, fmt, ...)\
		if (UNLIKELY(!(x)))\
		{\
			fprintf(stderr, "%s:%d:" TERM_COLOR_BRIGHT_RED " error:" TERM_COLOR_RESET " " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__);\
			exit(1);\
		}
#	define KORIN_CHECKF(x, fmt, ...)\
		if (UNLIKELY(!(x)))\
		{\
			fprintf(stderr, "%s:%d:" TERM_COLOR_BRIGHT_YELLOW " warning:" TERM_COLOR_RESET " " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__);\
		}
#endif

#define KORIN_ASSERT(x) KORIN_ASSERTF("ASSERT failed: %s.", #x)
#define KORIN_CHECK(x) KORIN_CHECKF("CHECK failed: %s.", #x)

// This are deprecated
#define ASSERTF(...) KORIN_ASSERTF(__VA_ARGS__)
#define ASSERT(...) KORIN_ASSERT(__VA_ARGS__)
#define CHECKF(...) KORIN_CHECKF(__VA_ARGS__)
#define CHECK(...) KORIN_CHECK(__VA_ARGS__)
