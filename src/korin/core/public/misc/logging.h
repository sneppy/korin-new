#pragma once

#include "build.h"

#define TERM_COLOR_RESET "\u001b[0m"
#define TERM_COLOR_BRIGHT_RED "\u001b[31;1m"
#define TERM_COLOR_BRIGHT_YELLOW "\u001b[33;1m"

#if BUILD_RELEASE
#	define ASSERTF(x, fmt, ...)
#	define CHECKF(x, fmt, ...)
#else
#	define ASSERTF(x, fmt, ...)\
		if (UNLIKELY(!x))\
		{\
			fprintf(stderr, "%s:%d:" TERM_COLOR_BRIGHT_RED " error:" TERM_COLOR_RESET " " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__);\
			exit(1);\
		}
#	define CHECKF(x, fmt, ...)\
		if (UNLIKELY(!x))\
		{\
			fprintf(stderr, "%s:%d:" TERM_COLOR_BRIGHT_YELLOW " warning:" TERM_COLOR_RESET " " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__);\
		}
#endif

#define ASSERT(x) ASSERTF("ASSERT failed: %s.", #x)
#define CHECK(x) CHECKF("CHECK failed: %s.", #x)
