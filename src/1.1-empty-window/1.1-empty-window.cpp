#include "1-common.h"

#include <glad/glad.h>

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
    WindowBoilerplate().run([](const auto &) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    });
    return 0;
}
