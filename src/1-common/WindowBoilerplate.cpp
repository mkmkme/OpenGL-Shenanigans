#include "WindowBoilerplate.h"

#include <SDL_events.h>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL_keycode.h>
#include <fmt/format.h>
#include <glad/glad.h>

WindowBoilerplate::WindowBoilerplate(int width, int height)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    window =
        SDL_CreateWindow("OpenGL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
    context = SDL_GL_CreateContext(window);
    if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
        throw std::runtime_error("Failed to initialize glad!");
    }
}

WindowBoilerplate::~WindowBoilerplate()
{
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

void WindowBoilerplate::run(std::function<void()> &&renderCallback)
{
    while (true) {
        const auto now = SDL_GetTicks();
        _deltaTime = now - _lastFrameTime;
        _lastFrameTime = now;

        if (renderCommon()) {
            return;
        }

        renderCallback();

        SDL_GL_SwapWindow(window);
    }
}

WindowBoilerplate &WindowBoilerplate::addKeyCallback(SDL_Keycode keycode, handle_key_cb_t &&handleKeyCallback)
{
    _keyCallbacks[keycode] = std::move(handleKeyCallback);
    return *this;
}

void WindowBoilerplate::run(render_cb_t &&renderCallback)
{
    while (true) {
        const auto now = SDL_GetTicks();
        _deltaTime = now - _lastFrameTime;
        _lastFrameTime = now;

        if (renderCommon()) {
            return;
        }

        renderCallback(*this);
        SDL_GL_SwapWindow(window);
    }
}

WindowBoilerplate &WindowBoilerplate::addShader(
    const std::filesystem::path &vertexShaderPath, const std::filesystem::path &fragmentShaderPath
)
{
    _shaders.emplace_back(vertexShaderPath, fragmentShaderPath);
    return *this;
}

WindowBoilerplate &WindowBoilerplate::addTexture(
    const std::filesystem::path &texturePath, Texture::Format format, Texture::Wrap wrapS, Texture::Wrap wrapT,
    Texture::Filter minFilter, Texture::Filter magFilter
)
{
    _textures.emplace_back(texturePath, format, wrapS, wrapT, minFilter, magFilter);
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

bool WindowBoilerplate::processInput()
{
    SDL_Event windowEvent;
    while (SDL_PollEvent(&windowEvent)) {
        switch (windowEvent.type) {
            case SDL_QUIT:
                return true;
            case SDL_KEYDOWN:
                switch (windowEvent.key.keysym.sym) {
                    case SDLK_ESCAPE:
                    case SDLK_q:
                        return true;
                    case SDLK_SPACE:
                        toggleWireframeMode();
                        return false;
                    default:
                        if (auto it = _keyCallbacks.find(windowEvent.key.keysym.sym); it != _keyCallbacks.end()) {
                            return it->second(*this);
                        }
                        return false;
                }
                break;
            case SDL_MOUSEMOTION:
                if (_firstMouseMove) {
                    _mousePosition = glm::vec2(windowEvent.motion.x, windowEvent.motion.y);
                    _firstMouseMove = false;
                } else {
                    const auto newMousePosition = glm::vec2(windowEvent.motion.x, windowEvent.motion.y);
                    float xDelta = newMousePosition.x - _mousePosition.x;
                    float yDelta = _mousePosition.y - newMousePosition.y;
                    _mousePosition = newMousePosition;

                    const float mouseSensitivity = 0.1f;
                    xDelta *= mouseSensitivity;
                    yDelta *= mouseSensitivity;

                    _yaw += xDelta;
                    _pitch += yDelta;
                    _pitch = std::clamp(_pitch, -89.f, 89.f);
                }
                return false;
            case SDL_MOUSEWHEEL:
                _fov -= windowEvent.wheel.y;
                _fov = std::clamp(_fov, 1.f, 45.f);
                return false;
        }
    }
    return false;
}

bool WindowBoilerplate::renderCommon() noexcept
{
    if (_wireframeMode) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    if (processInput()) {
        return true;
    }

    return false;
}
