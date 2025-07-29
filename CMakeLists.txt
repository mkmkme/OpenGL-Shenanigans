cmake_minimum_required(VERSION 4.0)

project(opengl-shenanigans CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Enable diagnostics colors for clang/gcc
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    add_compile_options("-fdiagnostics-color=always")
    add_compile_options(-Wall -Werror -Wextra -pedantic)
endif()

include(cmake/copy-resources.cmake)

find_package(glad REQUIRED)
find_package(SDL2 REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(Stb REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

prepare_dirs()

add_subdirectory(src)
