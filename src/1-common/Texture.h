#pragma once

#include <filesystem>

class Texture {

public:
    enum class Format { RGB, RGBA };
    enum class Wrap { Repeat, ClampToEdge };
    enum class Filter { Linear, LinearMipmapLinear };

    Texture(
        const std::filesystem::path &texturePath, Format format = Format::RGB, Wrap wrapS = Wrap::Repeat,
        Wrap wrapT = Wrap::Repeat, Filter minFilter = Filter::LinearMipmapLinear, Filter magFilter = Filter::Linear
    );

    unsigned id() const { return _id; }

private:
    unsigned _id;
};
