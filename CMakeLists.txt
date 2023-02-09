cmake_minimum_required(VERSION 3.15)

project(opengl-shenanigans CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(glu REQUIRED)
find_package(SDL2 REQUIRED)

add_subdirectory(src)