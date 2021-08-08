option(KORIN_BUILD_UNITS "If true build unit tests" OFF)
if(KORIN_BUILD_UNITS)
	# Make sure googletest is available
	FetchContent_MakeAvailable(googletest)

	# Add units
	add_subdirectory(units)
endif(KORIN_BUILD_UNITS)

option(KORIN_BUILD_BENCHES "If true build benchmarks" OFF)
if(KORIN_BUILD_BENCHES)
	# Make sure benchmark library is available
	FetchContent_MakeAvailable(benchmark)

	# Add benchmarks
	add_subdirectory(benches)
endif(KORIN_BUILD_BENCHES)
