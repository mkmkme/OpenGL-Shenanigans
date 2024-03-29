cmake_minimum_required(VERSION 3.27)

set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API aa1f7df0-828a-4fcd-9afc-2dc80491aca7)

project(opengl-shenanigans CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(glad REQUIRED)
find_package(SDL2 REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(modules_list src/1-common/WindowBoilerplate.cppm src/1-common/WindowBoilerplate.Utils.cppm)

add_executable(1.1-hello-window)
target_sources(1.1-hello-window PRIVATE
        FILE_SET all_my_modules TYPE CXX_MODULES
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/src/1-common
        FILES ${modules_list}
)

target_sources(1.1-hello-window PRIVATE src/1.1-empty-window/1.1-empty-window.cpp)
target_link_libraries(1.1-hello-window PRIVATE glad::glad SDL2::SDL2)

add_executable(1.2-hello-triangle)
target_sources(1.2-hello-triangle PRIVATE
        FILE_SET all_my_modules TYPE CXX_MODULES
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/src/1-common
        FILES ${modules_list}
)

target_sources(1.2-hello-triangle PRIVATE src/1.2-hello-triangle/1.2-hello-triangle.cpp)
target_link_libraries(1.2-hello-triangle PRIVATE glad::glad SDL2::SDL2)