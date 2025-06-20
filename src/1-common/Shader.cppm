module;

#include <filesystem>
#include <fmt/base.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

export module Shader;
import :Utils;

export namespace ShaderUtils {
std::filesystem::path shaderDir(const std::filesystem::path &argv0);
std::tuple<std::filesystem::path, std::filesystem::path> shaderFiles(const std::filesystem::path &argv0);
} // namespace ShaderUtils

export class Shader {
public:
    Shader(const std::filesystem::path &vertexShaderPath, const std::filesystem::path &fragmentShaderPath)
    {
        const auto vertexShaderSource = drainFile(vertexShaderPath);
        fmt::print("Vertex shader source: {}\n", vertexShaderSource);
        const auto fragmentShaderSource = drainFile(fragmentShaderPath);
        fmt::print("Fragment shader source: {}\n", fragmentShaderSource);

        const auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
        const auto *cstr = vertexShaderSource.c_str();
        glShaderSource(vertexShader, 1, &cstr, nullptr);
        glCompileShader(vertexShader);
        ensureShaderProcessSuccess(vertexShader, vertexShaderPath.string(), ProcessType::Compile);

        const auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        cstr = fragmentShaderSource.c_str();
        glShaderSource(fragmentShader, 1, &cstr, nullptr);
        glCompileShader(fragmentShader);
        ensureShaderProcessSuccess(fragmentShader, fragmentShaderPath.string(), ProcessType::Compile);

        _shaderProgram = glCreateProgram();
        glAttachShader(_shaderProgram, vertexShader);
        glAttachShader(_shaderProgram, fragmentShader);
        glLinkProgram(_shaderProgram);
        ensureShaderProcessSuccess(
            _shaderProgram, vertexShaderPath.string() + " + " + fragmentShaderPath.string(), ProcessType::Link
        );

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    ~Shader() { glDeleteProgram(_shaderProgram); }

    void use() const { glUseProgram(_shaderProgram); }

    void setUniformBool(std::string_view name, bool value) const
    {
        glUniform1i(glGetUniformLocation(_shaderProgram, name.data()), value);
    }

    void setUniformInt(std::string_view name, int value) const
    {
        glUniform1i(glGetUniformLocation(_shaderProgram, name.data()), value);
    }

    float getUniformFloat(std::string_view name) const noexcept
    {
        float value;
        glGetUniformfv(_shaderProgram, glGetUniformLocation(_shaderProgram, name.data()), &value);
        return value;
    }

    void setUniformFloat(std::string_view name, float value) const
    {
        glUniform1f(glGetUniformLocation(_shaderProgram, name.data()), value);
    }

    void setUniformVec4(std::string_view name, const glm::vec4 &value) const
    {
        glUniform4f(glGetUniformLocation(_shaderProgram, name.data()), value.r, value.g, value.b, value.a);
    }

private:
    unsigned _shaderProgram;
};
