#include "Texture.h"
#include "stb_image/stb_image.h"

Texture::Texture(const char* filename, const char* uniform_name)
    : uniform_name(uniform_name)
{
    std::vector<unsigned char> image;
    int width, height, chanels;
    stbi_set_flip_vertically_on_load(1);
    stbi_uc* buffer = stbi_load(filename, &width, &height, &chanels, 4); 
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    stbi_image_free(buffer);
}

Texture::~Texture()
{
    glDeleteTextures(1, &id);
}

const char* Texture::getUniformName()
{
    return uniform_name;
}

void Texture::Bind(unsigned int index, unsigned int unit_id, unsigned int texture_unit)
{
    glUniform1i(index, unit_id);
    glActiveTexture(texture_unit);
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::Unbind(unsigned int texture_unit)
{
    glActiveTexture(texture_unit);
    glBindTexture(GL_TEXTURE_2D, 0);
}
