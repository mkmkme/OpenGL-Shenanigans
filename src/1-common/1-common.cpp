#include "1-common.h"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <glad/glad.h>
#include <iostream>

WindowBoilerplate::WindowBoilerplate()
{
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("OpenGL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_OPENGL);
    context = SDL_GL_CreateContext(window);
    if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
        std::cerr << "Failed to initialize glad!" << std::endl;
        assert(false);
    }
}

WindowBoilerplate::~WindowBoilerplate()
{
    if (_shaderData.has_value()) {
        glDeleteVertexArrays(1, &_shaderData->VAO);
        glDeleteBuffers(1, &_shaderData->VBO);
        glDeleteProgram(_shaderData->shaderProgram);
    }

    SDL_GL_DeleteContext(context);
    SDL_Quit();
}

namespace {
bool process_input()
{
    SDL_Event windowEvent;
    return SDL_PollEvent(&windowEvent) && windowEvent.type == SDL_QUIT;
}
} // namespace

void WindowBoilerplate::run(std::function<void(const std::optional<ShaderData> &)> &&render_function)
{
    while (true) {
        if (process_input()) {
            std::cout << "Goodbye!" << std::endl;
            return;
        }

        render_function(_shaderData);
        SDL_GL_SwapWindow(window);
    }
}

namespace {
std::string drainFile(const std::filesystem::path &path)
{
    auto file = std::ifstream(path);
    assert(file);

    return std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
}
} // namespace

WindowBoilerplate &WindowBoilerplate::withShaders(const char *argv0)
{
    auto arg0 = std::filesystem::path(argv0).filename();
    const auto vert_shader_source = drainFile(arg0.replace_extension(".vert"));
    const auto frag_shader_source = drainFile(arg0.replace_extension(".frag"));

    auto vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    const auto *cstr = vert_shader_source.c_str();
    std::cout << "vertex_shader content: " << cstr << std::endl;
    glShaderSource(vertex_shader, 1, &cstr, nullptr);
    glCompileShader(vertex_shader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, nullptr, infoLog);
        std::cerr << "Failed to compile vertex shader: " << infoLog << std::endl;
        assert(false);
    }

    auto fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    cstr = frag_shader_source.c_str();
    std::cout << "frag_shader content: " << cstr << std::endl;
    glShaderSource(fragment_shader, 1, &cstr, nullptr);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, 512, nullptr, infoLog);
        std::cerr << "Failed to compile fragment shader: " << infoLog << std::endl;
        assert(false);
    }

    const auto shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertex_shader);
    glAttachShader(shaderProgram, fragment_shader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Failed to link shader program: " << infoLog << std::endl;
        assert(false);
    }
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    _shaderData.emplace(shaderProgram);

    return *this;
}

WindowBoilerplate &WindowBoilerplate::withVertices(std::function<void(ShaderData &)> &&vertex_setup)
{
    vertex_setup(_shaderData.value());
    return *this;
}
