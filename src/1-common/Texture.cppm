module;

#include <filesystem>
#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

export module Texture;

export class Texture {
    unsigned _id;

public:
    Texture(
        const std::filesystem::path &texturePath, int format = GL_RGB, int wrapS = GL_REPEAT, int wrapT = GL_REPEAT,
        int minFilter = GL_LINEAR_MIPMAP_LINEAR, int magFilter = GL_LINEAR
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

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, textureData);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(textureData);
    }

    unsigned id() const { return _id; }
};
