#pragma once

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <functional>
#include <string>
#include <vector>

class WindowBoilerplate {
public:
    using shaders_t = std::vector<unsigned>;
    using vaos_t = std::vector<unsigned>;
    using vbos_t = std::vector<unsigned>;
    using ebos_t = std::vector<unsigned>;
    using setup_buffers_cb_t = std::function<void(const vaos_t &, const vbos_t &, const ebos_t &)>;
    using render_cb_t = std::function<void(const shaders_t &, const vaos_t &)>;

    WindowBoilerplate();
    ~WindowBoilerplate();

    void run(std::function<void()> &&renderCallback);
    void run(render_cb_t &&renderCallback);
    WindowBoilerplate &addShaderProgram(const std::string &vertexShader, const std::string &fragmentShader);
    WindowBoilerplate &generateObjects(size_t numVAOs, size_t numVBOs, size_t numEBOs) noexcept;
    WindowBoilerplate &setUpBuffers(setup_buffers_cb_t &&setupBuffersCallback) noexcept;

private:
    SDL_Window *window;
    SDL_GLContext context;

    shaders_t _shaderPrograms {};
    vaos_t _VAOs {};
    vbos_t _VBOs {};
    ebos_t _EBOs {};
};
