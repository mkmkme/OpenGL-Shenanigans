add_library(gui INTERFACE)
target_link_libraries(gui INTERFACE SDL2::SDL2 SDL2::SDL2main glu::glu)

add_library(1-common STATIC 1-common.cpp)
target_link_libraries(1-common PUBLIC gui)

add_executable(1.1-empty-window 1.1-empty-window.cpp)
target_link_libraries(1.1-empty-window 1-common)
