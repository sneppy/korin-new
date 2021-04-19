# Declare all modules
set(KORIN_MODULES

	"core"
)

foreach(MODULE_NAME ${KORIN_MODULES})
	# Get module directory
	get_filename_component(MODULE_DIR "./${MODULE_NAME}" ABSOLUTE)

	# Create the directory and the required
	# files if it doesn't exist
	if(NOT EXISTS ${MODULE_DIR})
		# Create directory and subfolders
		file(MAKE_DIRECTORY ${MODULE_DIR})
		file(MAKE_DIRECTORY "${MODULE_DIR}/public")
		file(MAKE_DIRECTORY "${MODULE_DIR}/private")

		# Create module files
		configure_file("./module.h.in" "${MODULE_DIR}/public/${MODULE_NAME}.h" @ONLY)
		configure_file("./module.cpp.in" "${MODULE_DIR}/private/${MODULE_NAME}.cpp" @ONLY)
		configure_file("./module.cmake.in" "${MODULE_DIR}/CMakeLists.txt" @ONLY)

		# Create module config file
	endif(NOT EXISTS ${MODULE_DIR})

	# Add module
	add_subdirectory(${MODULE_DIR})
endforeach(MODULE_NAME ${MODULES})

# Create korin library
option(KORIN_BUILD_MODULES_ONLY "If true don't create a macro library")
if(NOT KORIN_BUILD_MODULES_ONLY)
	# Create project macro library
	add_library(${PROJECT_NAME})
	target_link_libraries(${PROJECT_NAME} ${KORIN_MODULES})
endif(NOT KORIN_BUILD_MODULES_ONLY)

# Build main
option(KORIN_BUILD_MAIN "If true build main executable" ON)
if(KORIN_BUILD_MAIN)
	# Create main executable
	add_executable(${PROJECT_NAME}_main "./main.cpp")
	target_link_libraries(${PROJECT_NAME}_main ${KORIN_MODULES})
endif(KORIN_BUILD_MAIN)

