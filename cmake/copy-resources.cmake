function(prepare_dirs)
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/shaders RESULT result)

    if(result)
        message(WARNING "Failed to create directory ${CMAKE_BINARY_DIR}/shaders")
    endif()

    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/textures RESULT result)

    if(result)
        message(WARNING "Failed to create directory ${CMAKE_BINARY_DIR}/textures")
    endif()
endfunction()

function(copy_shaders)
    foreach(shader IN LISTS ARGN)
        file(GENERATE
            OUTPUT ${CMAKE_BINARY_DIR}/shaders/${shader}
            INPUT ${CMAKE_CURRENT_SOURCE_DIR}/${shader}
        )
    endforeach()
endfunction()

function(copy_shaders_trivial name)
    copy_shaders(${name}.frag ${name}.vert)
endfunction()

function(copy_textures)
    foreach(texture IN LISTS ARGN)
        file(COPY
            ${CMAKE_CURRENT_SOURCE_DIR}/${texture}
            DESTINATION ${CMAKE_BINARY_DIR}/textures
        )
    endforeach()
endfunction()
