#include "1-common.h"

#include <glad/glad.h>
#include <span>

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
    // clang-format off
    constexpr float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f,
    };
    // clang-format on

    WindowBoilerplate()
        .withShaders(argv[0])
        .withVertices([vertices = std::span { vertices }](auto &shaderData) {
            glGenVertexArrays(1, &shaderData.VAO);
            glGenBuffers(1, &shaderData.VBO);

            glBindVertexArray(shaderData.VAO);

            glBindBuffer(GL_ARRAY_BUFFER, shaderData.VBO);
            glBufferData(GL_ARRAY_BUFFER, vertices.size_bytes(), vertices.data(), GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
            glEnableVertexAttribArray(0);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        })
        .run([](const auto &shaderData) {
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glUseProgram(shaderData->shaderProgram);
            glBindVertexArray(shaderData->VAO);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        });
    return 0;
}
