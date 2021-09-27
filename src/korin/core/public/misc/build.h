#pragma once

#ifdef PLATFORM_WINDOWS
#	define PLATFORM_WINDOWS 1
#else
#	define PLATFORM_WINDOWS 0
#endif

#ifdef PLATFORM_APPLE
#	define PLATFORM_APPLE 1
#else
#	define PLATFORM_APPLE 0
#endif

#ifdef PLATFORM_LINUX
#	define PLATFORM_LINUX 1
#else
#	define PLATFORM_LINUX 0
#endif

#ifdef BUILD_DEBUG
# define BUILD_DEBUG 1
#else
# define BUILD_DEBUG 0
#endif

#ifdef BUILD_DEVELOPMENT
# define BUILD_DEVELOPMENT 1
#else
# define BUILD_DEVELOPMENT 0
#endif

#ifdef BUILD_RELEASE
# define BUILD_RELEASE 1
#else
# define BUILD_RELEASE 0
#endif

#define KORIN_DEBUG BUILD_DEBUG
#define KORIN_DEVELOPMENT BUILD_DEVELOPMENT
#define KORIN_RELEASE BUILD_RELEASE
