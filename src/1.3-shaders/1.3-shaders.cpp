#include "Resources.h"
#include "WindowBoilerplate.h"

#include <array>
#include <chrono>
#include <cmath>
#include <fmt/base.h>
#include <glad/glad.h>
#include <glm/vec4.hpp>
#include <span>

int main([[maybe_unused]] int argc, char *argv[])
{
    // clang-format off
    constexpr std::array vertices = {
         0.f,  0.5f, 0.0f, // top
         -0.5f, -0.5f, 0.0f, // bottom left
        0.5f, -.5f, 0.0f, // bottom right
    };
    constexpr std::array indices = {
        0, 1, 2, // first triangle
    };
    // clang-format on
    const auto [vertexShaderPath, fragmentShaderPath] = Resources::shaderFiles(argv[0]);

    WindowBoilerplate()
        .addShader(vertexShaderPath, fragmentShaderPath)
        .generateObjects()
        .setUpBuffers([vertices = std::span { vertices }, indices = std::span { indices }](
                          const WindowBoilerplate::vaos_t &VAOs, const WindowBoilerplate::vbos_t &VBOs,
                          const WindowBoilerplate::ebos_t &EBOs
                      ) {
            glBindVertexArray(VAOs.front());

            glBindBuffer(GL_ARRAY_BUFFER, VBOs.front());
            glBufferData(GL_ARRAY_BUFFER, vertices.size_bytes(), vertices.data(), GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs.front());
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size_bytes(), indices.data(), GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
            glEnableVertexAttribArray(0);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        })
        .run([](const WindowBoilerplate &window) {
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            const auto now = std::chrono::system_clock::now();
            const auto time = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
            const auto greenValue = std::sin(time / 250.0) / 2.0f + 0.5f;

            window.shaders().front().use();
            window.shaders().front().setUniformVec4("ourColor", { 0.f, greenValue, 0.0f, 1.0f });
            glBindVertexArray(window.VAOs().front());
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        });
    return 0;
}
