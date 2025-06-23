#include "Resources.h"

std::tuple<std::filesystem::path, std::filesystem::path> Resources::shaderFiles(
    const std::filesystem::path &argv0
) noexcept
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
