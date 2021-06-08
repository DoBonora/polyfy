add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
	target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}_lib)
	add_custom_command(TARGET ${PROJECT_NAME}
	                   POST_BUILD
	                   COMMAND ${CMAKE_COMMAND} -E create_symlink $<TARGET_FILE_DIR:${PROJECT_NAME}>/${PROJECT_NAME} ${CMAKE_BINARY_DIR}/${PROJECT_NAME}
	                   COMMENT "Creating symlink for ${PROJECT_NAME}"
	                   VERBATIM)
