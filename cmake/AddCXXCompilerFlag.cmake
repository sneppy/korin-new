include(CheckCXXCompilerFlag)

# Returns a flag that can be used to check
function(mangle_compiler_flag FLAG OUTPUT_RESULT)
	string(TOUPPER "HAS_CXX_FLAG_${FLAG}" MANGLED_FLAG)
	string(REPLACE "+" "X" MANGLED_FLAG ${MANGLED_FLAG})
	string(REGEX REPLACE "[^\\w]" "_" MANGLED_FLAG ${MANGLED_FLAG})
	string(REGEX REPLACE "_+" "_" MANGLED_FLAG ${MANGLED_FLAG})

	set(${OUTPUT_RESULT} ${MANGLED_FLAG} PARENT_SCOPE)
endfunction(mangle_compiler_flag FLAG OUTPUT_RESULT)


# Adds a compile flag for the specified build mode
function(add_cxx_compiler_flag FLAG)
	# Check if compiler supports cxx flag
	mangle_compiler_flag(${FLAG} MANGLED_FLAG)
	set(OLD_CMAKE_REQUIRED_FLAGS ${CMAKE_REQUIRED_FLAGS})
	set(CMAKE_REQUIRED_FLAGS "${CMAKE_REQUIRED_FLAGS} ${FLAG}")
	check_cxx_compiler_flag(${FLAG} ${MANGLED_FLAG})
	set(CMAKE_REQUIRED_FLAGS ${OLD_CMAKE_REQUIRED_FLAGS})

	if(${MANGLED_FLAG})
		# Get build variant
		set(BUILD_VARIANT ${ARGV1})
		if(BUILD_VARIANT)
			string(TOUPPER "_${BUILD_VARIANT}" BUILD_VARIANT)
		endif(BUILD_VARIANT)

		set(CMAKE_CXX_FLAGS${BUILD_VARIANT} "${CMAKE_CXX_FLAGS${BUILD_VARIANT}} ${FLAG}" PARENT_SCOPE)		
	endif(${MANGLED_FLAG})
endfunction(add_cxx_compiler_flag FLAG)
