add_library(common STATIC
    Resources.cpp
    Shader.cpp
    Texture.cpp
    WindowBoilerplate.cpp
)

target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(common PUBLIC glad::glad SDL2::SDL2 fmt::fmt)
target_include_directories(common PUBLIC ${Stb_INCLUDE_DIR})
