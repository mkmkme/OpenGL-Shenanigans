add_executable(1.2-hello-triangle)

target_sources(1.2-hello-triangle PRIVATE 1.2-hello-triangle.cpp)
target_link_libraries(1.2-hello-triangle PRIVATE common)

add_custom_command(TARGET 1.2-hello-triangle POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    -t ${CMAKE_BINARY_DIR}/shaders
    ${CMAKE_CURRENT_SOURCE_DIR}/1.2-hello-triangle.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/1.2-hello-triangle.vert
)
