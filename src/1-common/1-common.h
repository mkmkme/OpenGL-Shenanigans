#pragma once

#include <SDL2/SDL.h>
#include <functional>
#include <optional>

struct WindowBoilerplate {
    struct ShaderData {
        unsigned shaderProgram {};
        unsigned VAO {};
        unsigned VBO {};
        unsigned EBO {};
    };

    WindowBoilerplate();
    ~WindowBoilerplate();

    void run(std::function<void(const std::optional<ShaderData> &)> &&render_function);
    WindowBoilerplate &withShaders(const char *argv0);
    WindowBoilerplate &withVertices(std::function<void(ShaderData &)> &&vertex_setup);

    SDL_Window *window;
    SDL_GLContext context;

    std::optional<ShaderData> _shaderData;
};
