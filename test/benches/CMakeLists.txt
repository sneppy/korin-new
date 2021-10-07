# Declare all benchmarks
set(KORIN_BENCHES

	"containers"
)

foreach(BENCH_NAME ${KORIN_BENCHES})
	get_filename_component(PUBLIC_FILE "./bench_${BENCH_NAME}.h" ABSOLUTE)
	get_filename_component(PRIVATE_FILE "./bench_${BENCH_NAME}.cpp" ABSOLUTE)

	# Create benchmark files if they don't exist
	if(NOT EXISTS ${PUBLIC_FILE})
		configure_file("./bench.h.in" ${PUBLIC_FILE} @ONLY)
	endif(NOT EXISTS ${PUBLIC_FILE})

	if(NOT EXISTS ${PRIVATE_FILE})
		configure_file("./bench.cpp.in" ${PRIVATE_FILE} @ONLY)	
	endif(NOT EXISTS ${PRIVATE_FILE})

	# Create benchmark executable
	set(BENCH_TARGET_NAME bench_${BENCH_NAME})
	add_executable(${BENCH_TARGET_NAME}

		${PUBLIC_FILE}
		${PRIVATE_FILE}
	)
	add_executable(${PROJECT_NAME}::${BENCH_TARGET_NAME} ALIAS ${BENCH_TARGET_NAME})

	# Link project library
	target_link_libraries(${BENCH_TARGET_NAME}

		${PROJECT_NAME}
		benchmark::benchmark
	)

	# Include common testing headers
	target_include_directories(${BENCH_TARGET_NAME}

		PUBLIC
			"../include"
	)
endforeach(BENCH_NAME in ${KORIN_BENCHES})
