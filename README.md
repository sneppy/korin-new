# Korin

[![Unit Test](https://github.com/sneppy/korin-new/actions/workflows/unit-test.yml/badge.svg)](https://github.com/sneppy/korin-new/actions/workflows/unit-test.yml)

**Korin** is a general-purpose C++ library.

# Installing

Korin can be configured and built using CMake. A C++ compiler that supports C++17 or better is required.

## Build library

To build the library:

```console
$ mkdir build
$ pushd build
$ cmake ..
$ make
$ popd
```

## Build unit tests and benchmarks

To build unit tests configure with `-DKORIN_BUILD_UNITS=ON` and then run:

```console
$ pushd build/test/units
$ ctest
$ popd
```

For benchmarks, configure the project with `-DKORIN_BUILD_BENCHES=ON` and the execute the bench:

```console
$ build/test/benches/bench_containers
```

## Adding as a dependency

You can add the Korin library using CMake's [FetchContent]() module:

```cmake
include(FetchContent)
set(FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/external") # Folder in which to store sources and output
FetchContent_Declare(korin GIT_REPOSITORY https://github.com/sneppy/korin-new.git)
```

And then, when needed:

```cmake
FetchContent_MakeAvailable(korin) # This will download the source files and configure the cmake project
target_link_libraries(${TARGET_NAME} korin)
```
