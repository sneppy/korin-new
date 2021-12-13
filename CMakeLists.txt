cmake_minimum_required(VERSION 3.12)

project(korin CXX)

# Append cmake paths
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

# Get project version from VERSION file
file(READ "${CMAKE_CURRENT_SOURCE_DIR}/VERSION" VERSION)
string(REGEX MATCH "v?[0-9]+\\.[0-9]+\\.[0-9]+" KORIN_VERSION ${VERSION})
message(STATUS "Korin version: ${KORIN_VERSION}")

if(MSVC)
	set(KORIN_BUILD_32_BITS OFF CACHE BOOL "If true build a 32-bit application - this option is disabled for MSVC")
else(MSVC)
	option(KORIN_BUILD_32_BITS "If true build a 32-bit application" OFF)
endif(MSVC)

# Configure compiler
include(AddCXXCompilerFlag)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(KORIN_BUILD_32_BITS)
	add_cxx_compiler_flag("-m32")
endif(KORIN_BUILD_32_BITS)

if(MSVC)
	# TODO
else(MSVC)
	# Turn on compiler warnings
	add_cxx_compiler_flag("-werror" RELEASE)
	add_cxx_compiler_flag("-wall")
	add_cxx_compiler_flag("-wextra")
	add_cxx_compiler_flag("-wshadow")
endif(MSVC)

# Check platform
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPLATFORM_WINDOWS")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPLATFORM_APPLE")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPLATFORM_LINUX")
else(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	message(FATAL_ERROR "Unsupported platform (${CMAKE_SYSTEM})")
endif(CMAKE_SYSTEM_NAME STREQUAL "Windows")

# Set build macros
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DKORIN_VERSION=${KORIN_VERSION}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -DBUILD_DEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O1 -DBUILD_DEVELOPMENT")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast -DBUILD_RELEASE")

# Add third party projects
include(DeclareExternalDependencies)

# Add source
add_subdirectory(src)

# Add tests
add_subdirectory(test)