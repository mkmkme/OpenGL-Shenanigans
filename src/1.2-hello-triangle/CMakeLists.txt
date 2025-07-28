add_executable(1.2-hello-triangle)

target_sources(1.2-hello-triangle PRIVATE 1.2-hello-triangle.cpp)
target_link_libraries(1.2-hello-triangle PRIVATE common)

copy_shaders_trivial(1.2-hello-triangle)
