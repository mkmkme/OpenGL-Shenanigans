add_executable(1.3-color-interpolation)
target_sources(1.3-color-interpolation PRIVATE 1.3-color-interpolation.cpp)
target_link_libraries(1.3-color-interpolation PRIVATE common)

copy_shaders_trivial(1.3-color-interpolation)

add_executable(1.3-shaders)
target_sources(1.3-shaders PRIVATE 1.3-shaders.cpp)
target_link_libraries(1.3-shaders PRIVATE common)

copy_shaders_trivial(1.3-shaders)
