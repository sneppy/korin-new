#pragma once

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
