add_executable(1.4-container)

target_sources(1.4-container PRIVATE 1.4-container.cpp)
target_link_libraries(1.4-container PRIVATE common)

add_custom_command(TARGET 1.4-container POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    -t ${CMAKE_BINARY_DIR}/shaders
    ${CMAKE_CURRENT_SOURCE_DIR}/1.4-container.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/1.4-container.vert
)

add_custom_command(TARGET 1.4-container POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    -t ${CMAKE_BINARY_DIR}/textures
    ${CMAKE_CURRENT_SOURCE_DIR}/1.4-container.jpg
    ${CMAKE_CURRENT_SOURCE_DIR}/1.4-awesome.png
)
