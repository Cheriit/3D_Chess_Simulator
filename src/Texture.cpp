#include "Texture.h"

Texture::Texture(const char* filename)
{
    std::vector<unsigned char> image;
    unsigned width, height;
    unsigned error = lodepng::decode(image, width, height, filename);
    if (error)
    {
        std::cout << "Error " << error << std::endl;
        exit(EXIT_FAILURE);
    }
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Texture::~Texture()
{
    glDeleteTextures(1, &id);
}

void Texture::Bind(unsigned int index, unsigned int texture_unit)
{
    glUniform1i(index, texture_unit);
    glActiveTexture(GL_TEXTURE+texture_unit);
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::Unbind(unsigned int texture_unit)
{
    glActiveTexture(GL_TEXTURE + texture_unit);
    glBindTexture(GL_TEXTURE_2D, 0);
}
