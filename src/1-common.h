#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <functional>

struct WindowBoilerplate {
    WindowBoilerplate();
    ~WindowBoilerplate();

    void run(std::function<void(void)> &&render_function);

    SDL_Window *window;
    SDL_GLContext context;
};