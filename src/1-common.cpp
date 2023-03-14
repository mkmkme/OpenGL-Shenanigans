#include "1-common.h"

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

void WindowBoilerplate::run(std::function<void(void)> &&render_function)
{
    while (true) {
        if (process_input()) {
            std::cout << "Goodbye!" << std::endl;
            return;
        }

        render_function();
        SDL_GL_SwapWindow(window);
    }
}