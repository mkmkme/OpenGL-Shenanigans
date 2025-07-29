add_executable(1.4-container 1.4-container.cpp)
target_link_libraries(1.4-container PRIVATE common)
copy_shaders_trivial(1.4-container)
copy_textures(1.4-container.jpg 1.4-awesome.png)

add_executable(1.4-cubes 1.4-cubes.cpp)
target_link_libraries(1.4-cubes PRIVATE common)
copy_shaders_trivial(1.4-cubes)

add_executable(1.4-camera 1.4-camera.cpp)
target_link_libraries(1.4-camera PRIVATE common)
copy_shaders_trivial(1.4-camera)
