#include "1-common.h"

#include <glad/glad.h>

int main()
{
    WindowBoilerplate().run([] {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    });
    return 0;
}
