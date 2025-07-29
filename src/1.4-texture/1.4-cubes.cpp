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
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    // clang-format on

    // clang-format off
    const std::array cubePositions = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f),
    };
    // clang-format on

    const auto [vertexShaderPath, fragmentShaderPath] = Resources::shaderFiles(argv[0]);

    const auto texturePath = Resources::textureDir(argv[0]) / "1.4-container.jpg";
    const auto texturePath2 = Resources::textureDir(argv[0]) / "1.4-awesome.png";

    const auto start = std::chrono::system_clock::now();

    glm::vec3 viewCoords { 0.0f, 0.0f, -3.0f };
    glm::vec3 rotateCoords { 1.0f, 0.3f, 0.5f };

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
        .setUpBuffers([vertices = std::span { vertices }](
                          const WindowBoilerplate::vaos_t &VAOs, const WindowBoilerplate::vbos_t &VBOs,
                          const WindowBoilerplate::ebos_t &EBOs
                      ) {
            glEnable(GL_DEPTH_TEST);

            glBindVertexArray(VAOs.front());

            glBindBuffer(GL_ARRAY_BUFFER, VBOs.front());
            glBufferData(GL_ARRAY_BUFFER, vertices.size_bytes(), vertices.data(), GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));

            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);

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
            'a',
            [&viewCoords](WindowBoilerplate &window) {
                viewCoords.x -= 0.1f;
                return false;
            }
        )
        .addKeyCallback(
            'd',
            [&viewCoords](WindowBoilerplate &window) {
                viewCoords.x += 0.1f;
                return false;
            }
        )
        .addKeyCallback(
            's',
            [&viewCoords](WindowBoilerplate &window) {
                viewCoords.y -= 0.1f;
                return false;
            }
        )
        .addKeyCallback(
            'w',
            [&viewCoords](WindowBoilerplate &window) {
                viewCoords.y += 0.1f;
                return false;
            }
        )
        .addKeyCallback(
            'e',
            [&viewCoords](WindowBoilerplate &window) {
                viewCoords.z -= 0.1f;
                return false;
            }
        )
        .addKeyCallback(
            'r',
            [&viewCoords](WindowBoilerplate &window) {
                viewCoords.z += 0.1f;
                return false;
            }
        )
        .addKeyCallback(
            'j',
            [&rotateCoords](WindowBoilerplate &window) {
                rotateCoords.x -= 0.1f;
                return false;
            }
        )
        .addKeyCallback(
            'l',
            [&rotateCoords](WindowBoilerplate &window) {
                rotateCoords.x += 0.1f;
                return false;
            }
        )
        .addKeyCallback(
            'i',
            [&rotateCoords](WindowBoilerplate &window) {
                rotateCoords.y += 0.1f;
                return false;
            }
        )
        .addKeyCallback(
            'k',
            [&rotateCoords](WindowBoilerplate &window) {
                rotateCoords.y -= 0.1f;
                return false;
            }
        )
        .addKeyCallback(
            'o',
            [&rotateCoords](WindowBoilerplate &window) {
                rotateCoords.z -= 0.1f;
                return false;
            }
        )
        .addKeyCallback(
            'p',
            [&rotateCoords](WindowBoilerplate &window) {
                rotateCoords.z += 0.1f;
                return false;
            }
        )
        .run([start, &viewCoords, &rotateCoords,
              cubePositions = std::span { cubePositions }](const WindowBoilerplate &window) {
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

            auto view = glm::mat4(1.0f);
            view = glm::translate(view, viewCoords);

            auto projection =
                glm::perspective(glm::radians(45.f), window.width() / static_cast<float>(window.height()), 0.1f, 100.f);

            shader.setUniformMat4("view", view);
            shader.setUniformMat4("projection", projection);

            glBindVertexArray(window.VAOs().front());
            for (size_t i = 0; i < cubePositions.size(); ++i) {
                auto model = glm::mat4(1.0f);
                model = glm::translate(model, cubePositions[i]);
                float angle = (i + 1) * 20.f;
                model = glm::rotate(model, time * glm::radians(angle) / 1000.f, rotateCoords);

                shader.setUniformMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        });

    return 0;
}
