#pragma once
#ifndef SKYBOX_H
#define SKYBOX_H

#include "Drawable.h"
#include "ShaderProgram.h"
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/type_ptr.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

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
    void BindTexture(ShaderProgram *SP);
};
#endif