#include "1-common.h"

#include <filesystem>
#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <stdexcept>

WindowBoilerplate::WindowBoilerplate()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
    window = SDL_CreateWindow("OpenGL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_OPENGL);
    context = SDL_GL_CreateContext(window);
    if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
        throw std::runtime_error("Failed to initialize glad!");
    }
}

WindowBoilerplate::~WindowBoilerplate()
{
    for (const auto &shaderProgram : _shaderPrograms) {
        glDeleteProgram(shaderProgram);
    }
    if (!_VAOs.empty()) {
        glDeleteVertexArrays(static_cast<GLsizei>(_VAOs.size()), _VAOs.data());
    }
    if (!_VBOs.empty()) {
        glDeleteBuffers(static_cast<GLsizei>(_VBOs.size()), _VBOs.data());
    }
    if (!_EBOs.empty()) {
        glDeleteBuffers(static_cast<GLsizei>(_EBOs.size()), _EBOs.data());
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

void WindowBoilerplate::run(std::function<void()> &&render_function)
{
    while (true) {
        if (process_input()) {
            return;
        }

        render_function();
        SDL_GL_SwapWindow(window);
    }
}

void WindowBoilerplate::run(render_cb_t &&render_function)
{
    while (true) {
        if (process_input()) {
            return;
        }

        render_function(_shaderPrograms, _VAOs);
        SDL_GL_SwapWindow(window);
    }
}

namespace {
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
} // namespace

WindowBoilerplate &WindowBoilerplate::addShaderProgram(
    const std::string &vertexShaderFile, const std::string &fragmentShaderFile
)
{
    const auto vertexShaderSource = drainFile(vertexShaderFile);
    const auto fragmentShaderSource = drainFile(fragmentShaderFile);

    unsigned vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const auto *cstr = vertexShaderSource.c_str();
    std::cout << cstr << std::endl;
    glShaderSource(vertexShader, 1, &cstr, nullptr);
    glCompileShader(vertexShader);
    ensureShaderCompilationSuccess(vertexShader, vertexShaderFile);

    unsigned fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    cstr = fragmentShaderSource.c_str();
    std::cout << cstr << std::endl;
    glShaderSource(fragmentShader, 1, &cstr, nullptr);
    glCompileShader(fragmentShader);
    ensureShaderCompilationSuccess(fragmentShader, fragmentShaderFile);

    unsigned shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    ensureShaderLinkingSuccess(shaderProgram, vertexShaderFile + " + " + fragmentShaderFile);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    _shaderPrograms.push_back(shaderProgram);

    return *this;
}

WindowBoilerplate &WindowBoilerplate::generateObjects(size_t numVAOs, size_t numVBOs, size_t numEBOs) noexcept
{
    _VAOs.resize(numVAOs);
    _VBOs.resize(numVBOs);
    _EBOs.resize(numEBOs);

    glGenVertexArrays(static_cast<GLsizei>(numVAOs), _VAOs.data());
    glGenBuffers(static_cast<GLsizei>(numVBOs), _VBOs.data());
    glGenBuffers(static_cast<GLsizei>(numEBOs), _EBOs.data());

    return *this;
}

WindowBoilerplate &WindowBoilerplate::setUpBuffers(setup_buffers_cb_t &&setupBuffersCallback) noexcept
{
    setupBuffersCallback(_VAOs, _VBOs, _EBOs);
    return *this;
}
