#include "Resources.h"
#include "WindowBoilerplate.h"

#include <array>
#include <fmt/base.h>
#include <glad/glad.h>
#include <span>

int main([[maybe_unused]] int argc, char *argv[])
{
    // clang-format off
    constexpr std::array vertices = {
         0.f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // top
         -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
        0.5f, -.5f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom right
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

            // position attribute
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
            glEnableVertexAttribArray(0);

            // color attribute
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        })
        .run([](const WindowBoilerplate &window) {
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            window.shaders().front().use();

            glBindVertexArray(window.VAOs().front());
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        });
    return 0;
}
