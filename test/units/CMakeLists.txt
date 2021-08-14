# Declare all units
set(KORIN_UNITS

	"containers"
	"memory"
)

foreach(UNIT_NAME ${KORIN_UNITS})
	get_filename_component(PUBLIC_FILE "./unit_${UNIT_NAME}.h" ABSOLUTE)
	get_filename_component(PRIVATE_FILE "./unit_${UNIT_NAME}.cpp" ABSOLUTE)

	# Create unit if it doesn't exist
	if(NOT EXISTS ${PUBLIC_FILE})
		configure_file("./unit.h.in" ${PUBLIC_FILE} @ONLY)
	endif(NOT EXISTS ${PUBLIC_FILE})

	if(NOT EXISTS ${PRIVATE_FILE})
		configure_file("./unit.cpp.in" ${PRIVATE_FILE} @ONLY)	
	endif(NOT EXISTS ${PRIVATE_FILE})

	# Create unit executable
	set(UNIT_TARGET_NAME unit_${UNIT_NAME})
	add_executable(${UNIT_TARGET_NAME}
	
		${PUBLIC_FILE}
		${PRIVATE_FILE}
	)
	add_executable(${PROJECT_NAME}::${UNIT_TARGET_NAME} ALIAS ${UNIT_TARGET_NAME})

	# Link project library
	target_link_libraries(${UNIT_TARGET_NAME}
	
		${PROJECT_NAME}
		gtest
		${CMAKE_THREAD_LIBS_INIT}
	)
endforeach(UNIT_NAME ${KORIN_UNITS})
