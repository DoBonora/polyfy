cmake_minimum_required(VERSION 3.14...3.19)
project(polyfy
        LANGUAGES CXX
        )
      
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# configuration options
option(DEPLOY "Configure for deployment")
option(BUILD_TESTS "Also build tests for polyfy project")
option(PERF "Add debug info for performance analysis tools")

#build type settings
set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
	set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "Choose the type of build." FORCE)
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

# Add path for custom modules
list (APPEND CMAKE_MODULE_PATH
      "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(cmake/DownloadProject.cmake)
find_package(GMP REQUIRED)
add_library(gmp::gmpxx IMPORTED INTERFACE)
add_library(gmp::libgmpxx IMPORTED INTERFACE)
add_library(gmp::libgmp IMPORTED INTERFACE)

add_subdirectory(src)

set(UPDATE_DISCONNECTED_IF_AVAILABLE "UPDATE_DISCONNECTED 1")
download_project(PROJ                mockturtle
                 GIT_REPOSITORY      https://github.com/lsils/mockturtle.git
                 GIT_TAG             master
                 ${UPDATE_DISCONNECTED_IF_AVAILABLE}
                 )
               add_subdirectory(${mockturtle_SOURCE_DIR}/include)
               add_subdirectory(  ${mockturtle_SOURCE_DIR}/lib)

# add test code
if (BUILD_TESTS)
	enable_testing()
	#include(GoogleTest)
        find_package(GTest REQUIRED)
	add_subdirectory(test)
endif ()

# add apps if this is the main project
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	add_subdirectory(apps)
endif()
