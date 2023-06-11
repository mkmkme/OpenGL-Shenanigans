#include "1-common.h"

#include <array>
#include <glad/glad.h>
#include <span>

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
    // clang-format off
    constexpr std::array vertices = {
         0.5f,  0.5f, 0.0f, // top right
         0.5f, -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f, // top left
    };
    constexpr std::array indices = {
        0, 1, 3, // first triangle
        1, 2, 3, // second triangle
    };
    // clang-format on

    WindowBoilerplate()
        .withShaders(argv[0])
        .withVertices([vertices = std::span { vertices }, indices = std::span { indices }](auto &shaderData) {
            glGenVertexArrays(1, &shaderData.VAO);
            glGenBuffers(1, &shaderData.VBO);
            glGenBuffers(1, &shaderData.EBO);

// #define WIREFRAME_MODE
#ifdef WIREFRAME_MODE
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

            glBindVertexArray(shaderData.VAO);

            glBindBuffer(GL_ARRAY_BUFFER, shaderData.VBO);
            glBufferData(GL_ARRAY_BUFFER, vertices.size_bytes(), vertices.data(), GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shaderData.EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size_bytes(), indices.data(), GL_STATIC_DRAW);

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
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        });
    return 0;
}
