module;

#include <SDL2/SDL.h>
#include <fstream>
#include <glad/glad.h>
#include <filesystem>
#include <iostream>

export module WindowBoilerplate:Utils;

bool process_input()
{
    SDL_Event windowEvent;
    return SDL_PollEvent(&windowEvent) && windowEvent.type == SDL_QUIT;
}

std::string drainFile(const std::filesystem::path &path)
{
    auto file = std::ifstream(path);

    return std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
}

char infoLog[512];
void ensureShaderCompilationSuccess(unsigned shader, const std::string &shaderName)
{
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        throw std::runtime_error("Failed to compile " + shaderName + ": " + infoLog);
    }
}

void ensureShaderLinkingSuccess(unsigned shaderProgram, const std::string &shaderProgramName)
{
    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        throw std::runtime_error("Failed to link " + shaderProgramName + ": " + infoLog);
    }
}