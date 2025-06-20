module;

#include <filesystem>

export module ResourceUtils;

export namespace Resources {

std::filesystem::path shaderDir(const std::filesystem::path &argv0)
{
    return argv0.parent_path() / "shaders";
}

std::filesystem::path textureDir(const std::filesystem::path &argv0)
{
    return argv0.parent_path() / "textures";
}

std::tuple<std::filesystem::path, std::filesystem::path> shaderFiles(const std::filesystem::path &argv0)
{
#ifdef _WIN32
    const auto programName = argv0.filename().replace_extension().string();
#else
    const auto programName = argv0.filename().string();
#endif
    const auto shaderDirPath = shaderDir(argv0);
    const auto vertexShaderPath = shaderDirPath / (programName + ".vert");
    const auto fragmentShaderPath = shaderDirPath / (programName + ".frag");
    return { vertexShaderPath, fragmentShaderPath };
}

} // namespace Resources
