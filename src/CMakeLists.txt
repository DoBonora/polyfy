add_library(${PROJECT_NAME}_lib
  )

# set include directories
target_include_directories(${PROJECT_NAME}_lib PUBLIC
	                           $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include/>)

# set required C++ standard and disable compiler specific extensions
target_compile_features(${PROJECT_NAME}_lib PUBLIC cxx_std_17)
set_target_properties(${PROJECT_NAME}_lib PROPERTIES CMAKE_CXX_STANDARD_REQUIRED ON CXX_EXTENSIONS OFF)


# enable interprocedural optimization if it is supported
include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported)
if(ipo_supported)
	set_target_properties(${PROJECT_NAME}_lib PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

# set compiler flag depending on compiler
if(MSVC)
	target_compile_options(${PROJECT_NAME}_lib PUBLIC /utf-8)
else()
        target_compile_options(${PROJECT_NAME}_lib PUBLIC -Wall -Wextra $<$<CONFIG:DEBUG>:-Og>)
        if(NOT DEPLOY)
		# only include machine-specific optimizations when building for the host machine
		target_compile_options(${PROJECT_NAME}_lib PUBLIC -mtune=native -march=native)
	endif ()
endif()
