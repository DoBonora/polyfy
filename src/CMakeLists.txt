add_library(${PROJECT_NAME}_lib
  ${CMAKE_CURRENT_SOURCE_DIR}/Polynomial.cpp
  ${${PROJECT_NAME}_SOURCE_DIR}/include/Polynomial.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Monomial.cpp
  ${${PROJECT_NAME}_SOURCE_DIR}/include/Monomial.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Term.cpp
  ${${PROJECT_NAME}_SOURCE_DIR}/include/Term.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Ideal.cpp
  ${${PROJECT_NAME}_SOURCE_DIR}/include/Ideal.hpp
  )

add_library(${PROJECT_NAME}_parse_lib
  ${CMAKE_CURRENT_SOURCE_DIR}/Parse.cpp
  ${${PROJECT_NAME}_SOURCE_DIR}/include/Parse.hpp
  )

# set include directories
target_include_directories(${PROJECT_NAME}_lib PUBLIC
  $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include/>
  $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/mockturtle-master/include/>)

target_include_directories(${PROJECT_NAME}_parse_lib PUBLIC
  $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include/>
  $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/mockturtle-master/include/>)

target_compile_features(${PROJECT_NAME}_lib PUBLIC cxx_std_17)
target_compile_features(${PROJECT_NAME}_parse_lib PUBLIC cxx_std_17)

set_target_properties(${PROJECT_NAME}_lib PROPERTIES CMAKE_CXX_STANDARD_REQUIRED ON CXX_EXTENSIONS OFF)
set_target_properties(${PROJECT_NAME}_parse_lib PROPERTIES CMAKE_CXX_STANDARD_REQUIRED ON CXX_EXTENSIONS OFF)

target_link_libraries(${PROJECT_NAME}_lib PUBLIC gmpxx gmp::libgmpxx gmp gmp::libgmp ${PROJECT_NAME}_parse_lib)
target_link_libraries(${PROJECT_NAME}_parse_lib PUBLIC gmpxx gmp::libgmpxx gmp gmp::libgmp mockturtle)

# enable interprocedural optimization if it is supported
include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported)
if(ipo_supported)
	set_target_properties(${PROJECT_NAME}_lib PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

# set compiler flag depending on compiler
if(MSVC)
	target_compile_options(${PROJECT_NAME}_parse_lib PUBLIC /utf-8)
else()
        target_compile_options(${PROJECT_NAME}_parse_lib PUBLIC -Wall -Wextra $<$<CONFIG:DEBUG>:-Og>)
        if(NOT DEPLOY)
		# only include machine-specific optimizations when building for the host machine
		target_compile_options(${PROJECT_NAME}_parse_lib PUBLIC -mtune=native -march=native)
	endif ()
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

# enable interprocedural optimization if it is supported
include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported)
if(ipo_supported)
	set_target_properties(${PROJECT_NAME}_parse_lib PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()
