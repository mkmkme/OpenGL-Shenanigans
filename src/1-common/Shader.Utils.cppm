module;

#include <filesystem>
#include <fmt/format.h>
#include <fstream>
#include <glad/glad.h>
#include <string>

export module Shader:Utils;

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
