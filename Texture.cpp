#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "glad.h"
#include "Globals.h"
#include <iostream>

std::unique_ptr<Texture> Texture::create(const std::string& path, const std::string& directory, const std::string& type)
{
    std::string fileName = directory + '/' + path;

    int width, height, nrComponents;
    unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &nrComponents, 0);
    if (!data)
    {
        std::cout << "Texture failed to load at path: " << fileName << "\n";
        stbi_image_free(data);

        return std::unique_ptr<Texture>();
    }
    else
    {
        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);

        return std::unique_ptr<Texture>(new Texture(textureID, type, path));
    }
}

void Texture::bind() const
{
    glBindTexture(1, ID);
}

Texture::Texture(unsigned int ID, const std::string& type, const std::string& path)
    : ID(ID),
    type(type),
    path(path)
{}

Texture::~Texture()
{
    assert(ID != Globals::INVALID_OPENGL_ID);
    glDeleteTextures(1, &ID);
}
