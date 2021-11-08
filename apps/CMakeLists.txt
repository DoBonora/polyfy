add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
add_executable(aig_to_poly ${CMAKE_CURRENT_SOURCE_DIR}/aig_to_poly.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}_lib)
target_link_libraries(aig_to_poly PUBLIC ${PROJECT_NAME}_parse_lib)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")
add_custom_command(TARGET ${PROJECT_NAME}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E create_symlink $<TARGET_FILE_DIR:${PROJECT_NAME}>/${PROJECT_NAME} ${CMAKE_BINARY_DIR}/${PROJECT_NAME}
  COMMENT "Creating symlink for ${PROJECT_NAME}"
  VERBATIM)
