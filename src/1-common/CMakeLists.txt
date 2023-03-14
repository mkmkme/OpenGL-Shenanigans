add_library(gui INTERFACE)
target_link_libraries(gui INTERFACE SDL2::SDL2 SDL2::SDL2main glad::glad)

add_library(1-common STATIC 1-common.cpp)
target_link_libraries(1-common PUBLIC gui)

target_include_directories(1-common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
