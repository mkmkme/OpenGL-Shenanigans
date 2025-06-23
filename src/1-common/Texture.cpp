
#include "Texture.h"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace {
using Format = Texture::Format;
using Wrap = Texture::Wrap;
using Filter = Texture::Filter;

int toGLFormat(Texture::Format format)
{
    switch (format) {
        case Format::RGB:
            return GL_RGB;
        case Format::RGBA:
            return GL_RGBA;
        default:
            throw std::invalid_argument("Unsupported texture format");
    }
}

int toGLWrap(Texture::Wrap wrap)
{
    switch (wrap) {
        case Wrap::Repeat:
            return GL_REPEAT;
        case Wrap::ClampToEdge:
            return GL_CLAMP_TO_EDGE;
        default:
            throw std::invalid_argument("Unsupported texture wrap mode");
    }
}

int toGLFilter(Texture::Filter filter)
{
    switch (filter) {
        case Filter::Linear:
            return GL_LINEAR;
        case Filter::LinearMipmapLinear:
            return GL_LINEAR_MIPMAP_LINEAR;
        default:
            throw std::invalid_argument("Unsupported texture filter mode");
    }
}
} // namespace

Texture::Texture(
    const std::filesystem::path &texturePath, Format format, Wrap wrapS, Wrap wrapT, Filter minFilter, Filter magFilter
)
{
    stbi_set_flip_vertically_on_load(true);

    int width, height, numChannels;
    const auto textureData = stbi_load(texturePath.string().c_str(), &width, &height, &numChannels, 0);
    if (!textureData) {
        throw std::runtime_error("Failed to load texture");
    }

    glGenTextures(1, &_id);
    glBindTexture(GL_TEXTURE_2D, _id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, toGLWrap(wrapS));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, toGLWrap(wrapT));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, toGLFilter(minFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, toGLFilter(magFilter));

    glTexImage2D(
        GL_TEXTURE_2D, 0, toGLFormat(format), width, height, 0, toGLFormat(format), GL_UNSIGNED_BYTE, textureData
    );
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(textureData);
}
