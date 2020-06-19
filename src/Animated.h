#pragma once
#ifndef ANIMATED_H
#define ANIMATED_H

#include "Drawable.h"

class Animated :
    public Drawable
{
private:
    bool is_animation_in_progress;
public:
    Animated(glm::vec3 Position, glm::vec3 Rotation, VertexArray* VAO, ElementBuffer* EBO, ShaderProgram* SP);
    ~Animated();
    void animate();
    bool isAnimationInProgress();
};

#endif