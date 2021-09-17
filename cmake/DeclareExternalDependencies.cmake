# Include FetchContent module
include(FetchContent)

# Set this folder as external project directories
set(FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/external")

# Declare all external dependencies
FetchContent_Declare(googletest

	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG v1.10.x
)
FetchContent_Declare(benchmark

	GIT_REPOSITORY https://github.com/google/benchmark.git
	GIT_TAG v1.5.2
)
FetchContent_Declare(gcem

	GIT_REPOSITORY https://github.com/kthohr/gcem.git
	GIT_TAG v1.13.1
)
