add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
add_executable(aig_to_poly ${CMAKE_CURRENT_SOURCE_DIR}/aig_to_poly.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}_lib)
target_link_libraries(aig_to_poly PUBLIC ${PROJECT_NAME}_parse_lib)
add_custom_command(TARGET ${PROJECT_NAME}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E create_symlink $<TARGET_FILE_DIR:${PROJECT_NAME}>/${PROJECT_NAME} ${CMAKE_BINARY_DIR}/${PROJECT_NAME}
  COMMENT "Creating symlink for ${PROJECT_NAME}"
  VERBATIM)
