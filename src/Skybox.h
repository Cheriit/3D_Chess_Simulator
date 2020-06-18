#pragma once
#ifndef SKYBOX_H
#define SKYBOX_H

#include "Drawable.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Skybox :
    public Drawable
{
private:
    GLuint Tex;
    void GenerateTexture();
    void GenerateVAO();
public:
    Skybox();
    ~Skybox();
    void Draw(glm::mat4 V, glm::mat4 P);
};
#endif