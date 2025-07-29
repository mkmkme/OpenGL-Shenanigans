#pragma once

#include "Shader.h"
#include "Texture.h"

#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_video.h>
#include <functional>
#include <glm/vec2.hpp>
#include <vector>

class WindowBoilerplate {
public:
    using vaos_t = std::vector<unsigned>;
    using vbos_t = std::vector<unsigned>;
    using ebos_t = std::vector<unsigned>;
    using shaders_t = std::vector<Shader>;
    using textures_t = std::vector<Texture>;
    using setup_buffers_cb_t = std::function<void(const vaos_t &, const vbos_t &, const ebos_t &)>;
    using render_cb_t = std::function<void(WindowBoilerplate &)>;
    using handle_key_cb_t = std::function<bool(WindowBoilerplate &)>;

    WindowBoilerplate(int width = 800, int height = 600);
    ~WindowBoilerplate();

    [[nodiscard]] int width() const noexcept { return SDL_GetWindowSurface(window)->w; }
    [[nodiscard]] int height() const noexcept { return SDL_GetWindowSurface(window)->h; }

    [[nodiscard]] uint32_t deltaTime() const noexcept { return _deltaTime; }
    [[nodiscard]] float cameraSpeed() const noexcept { return _deltaTime / 100.f; }

    [[nodiscard]] float pitch() const noexcept { return _pitch; }
    [[nodiscard]] float yaw() const noexcept { return _yaw; }

    const vaos_t &VAOs() const { return _VAOs; }
    const vbos_t &VBOs() const { return _VBOs; }
    const ebos_t &EBOs() const { return _EBOs; }
    const shaders_t &shaders() const { return _shaders; }
    const textures_t &textures() const { return _textures; }

    void run(std::function<void()> &&renderCallback);
    void run(render_cb_t &&renderCallback);

    WindowBoilerplate &addKeyCallback(SDL_Keycode keycode, handle_key_cb_t &&handleKeyCallback);
    WindowBoilerplate &addShader(
        const std::filesystem::path &vertexShaderPath, const std::filesystem::path &fragmentShaderPath
    );
    WindowBoilerplate &addTexture(
        const std::filesystem::path &texturePath, Texture::Format format = Texture::Format::RGB,
        Texture::Wrap wrapS = Texture::Wrap::Repeat, Texture::Wrap wrapT = Texture::Wrap::Repeat,
        Texture::Filter minFilter = Texture::Filter::LinearMipmapLinear,
        Texture::Filter magFilter = Texture::Filter::Linear
    );
    WindowBoilerplate &generateObjects(size_t numVAOs = 1, size_t numVBOs = 1, size_t numEBOs = 1) noexcept;
    WindowBoilerplate &setUpBuffers(setup_buffers_cb_t &&setupBuffersCallback) noexcept;

    void toggleWireframeMode() noexcept { _wireframeMode = !_wireframeMode; }
    void setWireframeMode(bool wireframeMode = true) noexcept { _wireframeMode = wireframeMode; }

private:
    SDL_Window *window;
    SDL_GLContext context;

    vaos_t _VAOs {};
    vbos_t _VBOs {};
    ebos_t _EBOs {};
    shaders_t _shaders {};
    textures_t _textures {};

    uint32_t _lastFrameTime = 0;
    uint32_t _deltaTime = 0;

    glm::vec2 _mousePosition { 0.f, 0.f };
    bool _firstMouseMove = true;

    float _yaw = -90.f;
    float _pitch = 0.0f;

    std::unordered_map<SDL_Keycode, handle_key_cb_t> _keyCallbacks {};

    bool _wireframeMode = false;

    bool processInput();
    bool renderCommon() noexcept;
};
