#include "Resources.h"
#include "WindowBoilerplate.h"

#include <SDL_scancode.h>
#include <array>
#include <fmt/base.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <span>

int main([[maybe_unused]] int argc, char *argv[])
{
    // clang-format off
    constexpr std::array vertices {
        // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
    };
    constexpr std::array indices {
        0, 1, 3, // first triangle
        1, 2, 3    // second triangle
    };
    // clang-format on
    const auto [vertexShaderPath, fragmentShaderPath] = Resources::shaderFiles(argv[0]);

    const auto texturePath = Resources::textureDir(argv[0]) / "1.4-container.jpg";
    const auto texturePath2 = Resources::textureDir(argv[0]) / "1.4-awesome.png";

    const auto start = std::chrono::system_clock::now();
    float x_position = 0.0f;

    WindowBoilerplate()
        .addShader(vertexShaderPath, fragmentShaderPath)
        .addTexture(
            texturePath, Texture::Format::RGB, Texture::Wrap::Repeat, Texture::Wrap::Repeat,
            Texture::Filter::LinearMipmapLinear, Texture::Filter::Linear
        )
        .addTexture(
            texturePath2, Texture::Format::RGBA, Texture::Wrap::Repeat, Texture::Wrap::Repeat,
            Texture::Filter::LinearMipmapLinear, Texture::Filter::Linear
        )
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

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));

            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        })
        .addKeyCallback(
            SDLK_UP,
            [](WindowBoilerplate &window) {
                const auto &shader = window.shaders().front();
                const float mixValue = shader.getUniformFloat("mixValue");
                fmt::print("mixValue: {}, increasing\n", mixValue);
                shader.setUniformFloat("mixValue", mixValue + 0.1f);
                return false;
            }
        )
        .addKeyCallback(
            SDLK_DOWN,
            [](WindowBoilerplate &window) {
                const auto &shader = window.shaders().front();
                const float mixValue = shader.getUniformFloat("mixValue");
                fmt::print("mixValue: {}, decreasing\n", mixValue);
                shader.setUniformFloat("mixValue", mixValue - 0.1f);
                return false;
            }
        )
        .addKeyCallback(
            SDLK_LEFT,
            [&x_position](WindowBoilerplate &) {
                x_position -= 0.1f;
                return false;
            }
        )
        .addKeyCallback(
            SDLK_RIGHT,
            [&x_position](WindowBoilerplate &) {
                x_position += 0.1f;
                return false;
            }
        )
        .run([start, &x_position](const WindowBoilerplate &window) {
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            const auto &shader = window.shaders().front();
            const auto &textures = window.textures();

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textures[0].id());
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, textures[1].id());

            shader.use();
            shader.setUniformInt("texture1", 0);
            shader.setUniformInt("texture2", 1);

            const auto now = std::chrono::system_clock::now();
            const auto time = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();

            // first rectangle
            auto transform = glm::mat4(1.0f);
            transform = glm::translate(transform, glm::vec3(x_position, -0.5f, 0.0f));
            transform = glm::rotate(transform, glm::radians(time / 10.f), glm::vec3(0.0f, 0.0f, 1.0f));

            shader.setUniformMat4("transform", transform);

            glBindVertexArray(window.VAOs().front());
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            // second rectangle
            transform = glm::mat4(1.0f);
            transform = glm::translate(transform, glm::vec3(-0.5f, 0.5f, 0.0f));
            // NOTE: negative scale inverses the object, set abs to prevent that
            const auto scale_factor = std::sin(time / 250.f);
            transform = glm::scale(transform, glm::vec3(scale_factor, scale_factor, scale_factor));
            shader.setUniformMat4("transform", transform);

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        });

    return 0;
}
