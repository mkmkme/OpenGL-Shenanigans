module;

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL_keycode.h>
#include <filesystem>
#include <fmt/format.h>
#include <glad/glad.h>
#include <stdexcept>

export module WindowBoilerplate;
import Shader;
import Texture;

export class WindowBoilerplate {
public:
    using vaos_t = std::vector<unsigned>;
    using vbos_t = std::vector<unsigned>;
    using ebos_t = std::vector<unsigned>;
    using shaders_t = std::vector<Shader>;
    using textures_t = std::vector<Texture>;
    using setup_buffers_cb_t = std::function<void(const vaos_t &, const vbos_t &, const ebos_t &)>;
    using render_cb_t = std::function<void(WindowBoilerplate &)>;
    using handle_key_cb_t = std::function<bool(WindowBoilerplate &)>;

private:
    SDL_Window *window;
    SDL_GLContext context;

    vaos_t _VAOs {};
    vbos_t _VBOs {};
    ebos_t _EBOs {};
    shaders_t _shaders {};
    textures_t _textures {};

    std::unordered_map<SDL_Keycode, handle_key_cb_t> _keyCallbacks {};

    bool _wireframeMode = false;

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

    const vaos_t &VAOs() const { return _VAOs; }
    const vbos_t &VBOs() const { return _VBOs; }
    const ebos_t &EBOs() const { return _EBOs; }
    const shaders_t &shaders() const { return _shaders; }
    const textures_t &textures() const { return _textures; }

    void run(std::function<void()> &&renderCallback)
    {
        while (true) {
            if (renderCommon()) {
                return;
            }

            renderCallback();

            SDL_GL_SwapWindow(window);
        }
    }

    WindowBoilerplate &addKeyCallback(SDL_Keycode keycode, handle_key_cb_t &&handleKeyCallback)
    {
        _keyCallbacks[keycode] = std::move(handleKeyCallback);
        return *this;
    }

    void run(render_cb_t &&renderCallback)
    {
        while (true) {
            if (renderCommon()) {
                return;
            }

            renderCallback(*this);
            SDL_GL_SwapWindow(window);
        }
    }

    WindowBoilerplate &addShader(
        const std::filesystem::path &vertexShaderPath, const std::filesystem::path &fragmentShaderPath
    )
    {
        _shaders.emplace_back(vertexShaderPath, fragmentShaderPath);
        return *this;
    }

    WindowBoilerplate &addTexture(
        const std::filesystem::path &texturePath, int format = GL_RGB, int wrapS = GL_REPEAT, int wrapT = GL_REPEAT,
        int minFilter = GL_LINEAR_MIPMAP_LINEAR, int magFilter = GL_LINEAR
    )
    {
        _textures.emplace_back(texturePath, format, wrapS, wrapT, minFilter, magFilter);
        return *this;
    }

    WindowBoilerplate &generateObjects(size_t numVAOs = 1, size_t numVBOs = 1, size_t numEBOs = 1) noexcept
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

    void toggleWireframeMode() noexcept { _wireframeMode = !_wireframeMode; }
    void setWireframeMode(bool wireframeMode = true) noexcept { _wireframeMode = wireframeMode; }

private:
    bool processInput()
    {
        SDL_Event windowEvent;
        while (SDL_PollEvent(&windowEvent)) {
            switch (windowEvent.type) {
                case SDL_QUIT:
                    return true;
                case SDL_KEYDOWN:
                    switch (windowEvent.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            return true;
                        case SDLK_q:
                            return true;
                        case SDLK_SPACE:
                            toggleWireframeMode();
                            break;
                        default:
                            if (auto it = _keyCallbacks.find(windowEvent.key.keysym.sym); it != _keyCallbacks.end()) {
                                if (it->second(*this)) {
                                    return true;
                                }
                            }
                            break;
                    }
                    break;
            }
        }
        return false;
    }

    bool renderCommon() noexcept
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
};
