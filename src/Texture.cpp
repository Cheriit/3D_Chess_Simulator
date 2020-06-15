#include "Texture.h"
#include "Drawer.h"

Texture::Texture(const char* filename)
{
    std::vector<unsigned char> image;
    unsigned int width, height;
    unsigned error = lodepng::decode(image, width, height, filename);
    if (error)
    {
        std::cout << "Error " << error << std::endl;
        exit(EXIT_FAILURE);
    }
    GLCall(glActiveTexture(GL_TEXTURE0));
    GLCall(glGenTextures(1, &id));
    GLCall(glBindTexture(GL_TEXTURE_2D, id));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data()));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Texture::~Texture()
{
    glDeleteTextures(1, &id);
}

void Texture::Bind(unsigned int index, unsigned int unit_id, unsigned int texture_unit)
{
    GLCall(glUniform1i(index, unit_id));
    GLCall(glActiveTexture(texture_unit));
    GLCall(glBindTexture(GL_TEXTURE_2D, id));
}

void Texture::Unbind(unsigned int texture_unit)
{
    glActiveTexture(texture_unit);
    glBindTexture(GL_TEXTURE_2D, 0);
}
