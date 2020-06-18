#include "Skybox.h"
#include "stb_image/stb_image.h"
#include "../res/models/skybox.h"

Skybox::Skybox()
    : Drawable()
{
	SP = new ShaderProgram("./res/shaders/v_skybox.glsl", NULL, "./res/shaders/f_skybox.glsl");
    SP->use();
    GenerateVAO();
    GenerateTexture();
}

Skybox::~Skybox()
{
    glDeleteTextures(1, &Tex);
    delete VAO;
    delete SP;
}

void Skybox::GenerateTexture()
{
    std::vector<const char*> faces = {
        "./res/textures/skybox/right.png",
        "./res/textures/skybox/left.png",
        "./res/textures/skybox/top.png",
        "./res/textures/skybox/bottom.png",
        "./res/textures/skybox/front.png",
        "./res/textures/skybox/back.png",
    };
    std::vector<unsigned char> image;
    int width, height, chanels;
    stbi_set_flip_vertically_on_load(1);
    glGenTextures(1, &Tex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, Tex);

    for (int i = 0; i < 6; i++)
    {
        stbi_uc* buffer = stbi_load(faces[i], &width, &height, &chanels, 3);
        if (!buffer)
            std::cout << "Error during loading " << faces[i] << std::endl;
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);
        stbi_image_free(buffer);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Skybox::GenerateVAO()
{
    VAO = new VertexArray();
    VAO->Bind();
    VertexBuffer* VBO = new VertexBuffer(skyboxVertices, 108);
    VAO->AddLayout(*VBO, 0, 3, 3, 0);
}

void Skybox::Draw(glm::mat4 V, glm::mat4 P)
{
    glDepthFunc(GL_LEQUAL);
    SP->use();
    glUniformMatrix4fv(SP->u("P"), 1, false, glm::value_ptr(P));
    glUniformMatrix4fv(SP->u("V"), 1, false, glm::value_ptr(V));

    VAO->Bind();

    GLCall(glUniform1i(SP->u("skybox"), 0));
    GLCall(glActiveTexture(GL_TEXTURE0))
    glBindTexture(GL_TEXTURE_CUBE_MAP, Tex);

    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthFunc(GL_LESS);
}