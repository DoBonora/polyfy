cmake_minimum_required(VERSION 3.13)
project(polyfy
        LANGUAGES CXX
        )


set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#build type settings
set(default_build_type "Release)"
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
	set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "Choose the type of build." FORCE)
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

# Add path for custom modules
list (APPEND CMAKE_MODULE_PATH
      "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_subdirectory(src)

add_subdirectory(apps)

