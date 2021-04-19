find_package(Git)

# Returns a string with the git tag
function(get_git_tag OUT_TAG)
	if(GIT_EXECUTABLE)
		# Get matching tag
		execute_process(
			COMMAND "${GIT_EXECUTABLE} describe --match \"v[0-9]*.[0-9]*.[0-9]*\""
			WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
			RESULT_VARIABLE EXIT_STATUS
			OUTPUT_VARIABLE GIT_TAG
			ERROR_QUIET
		)
		
		if(EXIT_STATUS)
			set(GIT_TAG "v0.0.0")
		else(EXIT_STATUS)
			string(STRIP ${GIT_TAG} GIT_TAG)
		endif(EXIT_STATUS)
	else(GIT_EXECUTABLE)
		set(GIT_TAG "v0.0.0")
	endif(GIT_EXECUTABLE)

	set(${OUT_TAG} ${GIT_TAG} PARENT_SCOPE)
endfunction(get_git_tag var)
