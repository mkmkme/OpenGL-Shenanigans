#include "Shader.h"

#include <filesystem>
#include <fmt/format.h>
#include <fstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec4.hpp>

namespace {
std::string drainFile(const std::filesystem::path &path)
{
    auto file = std::ifstream(path);
    fmt::print("Draining file: {}\n", path.string());

    return std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
}

enum class ProcessType { Compile, Link };

void ensureShaderProcessSuccess(unsigned shader, const std::string &shaderName, ProcessType processType)
{
    int success;
    char infoLog[512];

    if (processType == ProcessType::Compile) {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            throw std::runtime_error(fmt::format("Failed to compile {}: {}", shaderName, infoLog));
        }
    } else if (processType == ProcessType::Link) {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 512, nullptr, infoLog);
            throw std::runtime_error(fmt::format("Failed to link {}: {}", shaderName, infoLog));
        }
    }
}
} // namespace

Shader::Shader(const std::filesystem::path &vertexShaderPath, const std::filesystem::path &fragmentShaderPath)
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

Shader::~Shader()
{
    glDeleteProgram(_shaderProgram);
}

void Shader::use() const
{
    glUseProgram(_shaderProgram);
}

void Shader::setUniformBool(std::string_view name, bool value) const noexcept
{
    glUniform1i(glGetUniformLocation(_shaderProgram, name.data()), value);
}

void Shader::setUniformInt(std::string_view name, int value) const noexcept
{
    glUniform1i(glGetUniformLocation(_shaderProgram, name.data()), value);
}

float Shader::getUniformFloat(std::string_view name) const noexcept
{
    float value;
    glGetUniformfv(_shaderProgram, glGetUniformLocation(_shaderProgram, name.data()), &value);
    return value;
}

void Shader::setUniformFloat(std::string_view name, float value) const noexcept
{
    glUniform1f(glGetUniformLocation(_shaderProgram, name.data()), value);
}

void Shader::setUniformVec4(std::string_view name, const glm::vec4 &value) const noexcept
{
    glUniform4f(glGetUniformLocation(_shaderProgram, name.data()), value.r, value.g, value.b, value.a);
}

void Shader::setUniformMat4(std::string_view name, const glm::mat4 &value) const noexcept
{
    glUniformMatrix4fv(glGetUniformLocation(_shaderProgram, name.data()), 1, GL_FALSE, glm::value_ptr(value));
}
