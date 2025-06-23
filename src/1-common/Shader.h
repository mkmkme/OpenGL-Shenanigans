#pragma once

#include <filesystem>
#include <glm/fwd.hpp>



class Shader {
public:
    Shader(const std::filesystem::path &vertexShaderPath, const std::filesystem::path &fragmentShaderPath);
    ~Shader();

    void use() const;

    void setUniformBool(std::string_view name, bool value) const;
    void setUniformInt(std::string_view name, int value) const;
    float getUniformFloat(std::string_view name) const noexcept;
    void setUniformFloat(std::string_view name, float value) const;
    void setUniformVec4(std::string_view name, const glm::vec4 &value) const;
private:
    unsigned _shaderProgram;
};
