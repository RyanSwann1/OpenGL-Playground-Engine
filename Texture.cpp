#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "glad.h"
#include <iostream>

std::unique_ptr<Texture> Texture::create(const char* path, const std::string& directory)
{
    std::string fileName = std::string(path);
    fileName = directory + '/' + fileName;

    int width, height, nrComponents;
    unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &nrComponents, 0);
    if (!data)
    {
        std::cout << "Texture failed to load at path: " << fileName << "\n";
        stbi_image_free(data);

        return std::unique_ptr<Texture>();
    }
    if (data)
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

        return std::unique_ptr<Texture>();
    }
}

unsigned int Texture::getID() const
{
    return m_ID;
}

const std::string& Texture::getType()
{
    return m_type;
}

const std::string& Texture::getPath()
{
    return m_path;
}

Texture::Texture(unsigned int ID, const std::string& type, const std::string& path)
    : m_ID(ID),
    m_type(type),
    m_path(path)
{}

Texture::~Texture()
{
    glDeleteTextures(1, &m_ID);
}
