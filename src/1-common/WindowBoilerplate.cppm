module;

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <filesystem>
#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

export module WindowBoilerplate;
import :Utils;

export class WindowBoilerplate {
public:
    using shaders_t = std::vector<unsigned>;
    using vaos_t = std::vector<unsigned>;
    using vbos_t = std::vector<unsigned>;
    using ebos_t = std::vector<unsigned>;
    using setup_buffers_cb_t = std::function<void(const vaos_t &, const vbos_t &, const ebos_t &)>;
    using render_cb_t = std::function<void(const shaders_t &, const vaos_t &)>;

private:
    SDL_Window *window;
    SDL_GLContext context;

    shaders_t _shaderPrograms {};
    vaos_t _VAOs {};
    vbos_t _VBOs {};
    ebos_t _EBOs {};

public:
    WindowBoilerplate()
    {
        SDL_Init(SDL_INIT_VIDEO);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        window =
            SDL_CreateWindow("OpenGL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_OPENGL);
        context = SDL_GL_CreateContext(window);
        if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
            throw std::runtime_error("Failed to initialize glad!");
        }
    }

    ~WindowBoilerplate()
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

    void run(std::function<void()> &&renderCallback)
    {
        while (true) {
            if (process_input()) {
                return;
            }

            renderCallback();
            SDL_GL_SwapWindow(window);
        }
    }

    void run(render_cb_t &&renderCallback)
    {
        while (true) {
            if (process_input()) {
                return;
            }

            renderCallback(_shaderPrograms, _VAOs);
            SDL_GL_SwapWindow(window);
        }
    }

    WindowBoilerplate &addShaderProgram(const std::string &vertexShaderFile, const std::string &fragmentShaderFile)
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

    WindowBoilerplate &generateObjects(size_t numVAOs, size_t numVBOs, size_t numEBOs) noexcept
    {
        _VAOs.resize(numVAOs);
        _VBOs.resize(numVBOs);
        _EBOs.resize(numEBOs);

        glGenVertexArrays(static_cast<GLsizei>(numVAOs), _VAOs.data());
        glGenBuffers(static_cast<GLsizei>(numVBOs), _VBOs.data());
        glGenBuffers(static_cast<GLsizei>(numEBOs), _EBOs.data());

        return *this;
    }

    WindowBoilerplate &setUpBuffers(setup_buffers_cb_t &&setupBuffersCallback) noexcept
    {
        setupBuffersCallback(_VAOs, _VBOs, _EBOs);
        return *this;
    }
};