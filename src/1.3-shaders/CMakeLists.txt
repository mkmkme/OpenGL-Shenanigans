add_executable(1.3-color-interpolation)
target_sources(1.3-color-interpolation PRIVATE 1.3-color-interpolation.cpp)
target_link_libraries(1.3-color-interpolation PRIVATE common)

add_custom_command(TARGET 1.3-color-interpolation POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    -t ${CMAKE_BINARY_DIR}/shaders
    ${CMAKE_CURRENT_SOURCE_DIR}/1.3-color-interpolation.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/1.3-color-interpolation.vert
)

add_executable(1.3-shaders)
target_sources(1.3-shaders PRIVATE 1.3-shaders.cpp)
target_link_libraries(1.3-shaders PRIVATE common)

add_custom_command(TARGET 1.3-shaders POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    -t ${CMAKE_BINARY_DIR}/shaders
    ${CMAKE_CURRENT_SOURCE_DIR}/1.3-shaders.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/1.3-shaders.vert
)
