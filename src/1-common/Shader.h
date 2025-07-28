#pragma once

#include <filesystem>
#include <glm/fwd.hpp>

class Shader {
public:
    Shader(const std::filesystem::path &vertexShaderPath, const std::filesystem::path &fragmentShaderPath);
    ~Shader();

    void use() const;

    void setUniformBool(std::string_view name, bool value) const noexcept;
    void setUniformInt(std::string_view name, int value) const noexcept;
    float getUniformFloat(std::string_view name) const noexcept;
    void setUniformFloat(std::string_view name, float value) const noexcept;
    void setUniformVec4(std::string_view name, const glm::vec4 &value) const noexcept;
    void setUniformMat4(std::string_view name, const glm::mat4 &value) const noexcept;

private:
    unsigned _shaderProgram;
};
