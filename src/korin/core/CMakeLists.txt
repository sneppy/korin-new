set(MODULE_NAME core)

# Find source files
file(GLOB_RECURSE PRIVATE_FILES

	"./private/*.c"
	"./private/*.cpp"
	"./private/*.cxx"
	"./private/*.h"
	"./private/*.hpp"
	"./private/*.inl"
)

file(GLOB_RECURSE PUBLIC_FILES

	"./public/*.h"
	"./public/*.hpp"
	"./public/*.inl"
)

# Create target for module library
add_library(${MODULE_NAME} OBJECT

	${PRIVATE_FILES}
	${PUBLIC_FILES}
)
add_library(${PROJECT_NAME}::${MODULE_NAME} ALIAS ${MODULE_NAME})

# Require GCE-Math library for constexpr math
FetchContent_MakeAvailable(gcem)
target_link_libraries(${MODULE_NAME}

	gcem
)

# Include directories
target_include_directories(${MODULE_NAME}

	PUBLIC "./public"
)
