add_executable(1.4-container)

target_sources(1.4-container PRIVATE 1.4-container.cpp)
target_link_libraries(1.4-container PRIVATE common)

copy_shaders_trivial(1.4-container)
copy_textures(1.4-container.jpg 1.4-awesome.png)

