#pragma once

#include <filesystem>

namespace Resources {

inline std::filesystem::path shaderDir(const std::filesystem::path &argv0)
{
    return argv0.parent_path() / "shaders";
}

inline std::filesystem::path textureDir(const std::filesystem::path &argv0)
{
    return argv0.parent_path() / "textures";
}

std::tuple<std::filesystem::path, std::filesystem::path> shaderFiles(const std::filesystem::path &argv0) noexcept;

} // namespace Resources
