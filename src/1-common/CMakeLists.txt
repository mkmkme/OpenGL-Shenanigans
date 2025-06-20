add_library(common STATIC)

target_sources(common PUBLIC
    FILE_SET all_my_modules TYPE CXX_MODULES
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
    FILES
    OwnedPtr.cppm
    ResourceUtils.cppm
    Shader.cppm
    Shader.Utils.cppm
    Texture.cppm
    WindowBoilerplate.cppm
)

target_link_libraries(common PUBLIC glad::glad SDL2::SDL2 fmt::fmt)
target_include_directories(common PUBLIC ${Stb_INCLUDE_DIR})
