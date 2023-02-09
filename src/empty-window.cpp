#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <iostream>

// Returns true when it's time to stop
bool process_input()
{
    SDL_Event windowEvent;
    return SDL_PollEvent(&windowEvent) && windowEvent.type == SDL_QUIT;
}

void main_loop(SDL_Window *window)
{
    while (true) {
        if (process_input()) {
            std::cout << "Goodbye!" << std::endl;
            return;
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        SDL_GL_SwapWindow(window);
    }
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    auto *window =
        SDL_CreateWindow("OpenGL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_OPENGL);
    auto context = SDL_GL_CreateContext(window);

    main_loop(window);

    SDL_GL_DeleteContext(context);
    SDL_Quit();
    return 0;
}