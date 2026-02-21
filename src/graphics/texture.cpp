#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glad/glad.h>

#include "texture.h"
#include "../core/log.h"


Texture CreateTexture(const char* filepath)
{
    int32_t width, height, channels;
    uint8_t* data = stbi_load(filepath, &width, &height, &channels, 0);

    if (!data)
    {
        std::cout << "CreateTexture: Failed to load texture '" << filepath << "'\n";
        return {};
    }

    Texture texture;
    texture.Width = width;
    texture.Height = height;
    texture.Channels = channels;

    int32_t format = (channels == 4) ? GL_RGBA : GL_RGB;

    GL_CALL(glGenTextures(1, &texture.Index));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, texture.Index));
    GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data));
    GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));

    // Set texture parameters
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

    GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));

    stbi_image_free(data);
    return texture;
}
