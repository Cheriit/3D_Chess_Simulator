#pragma once
#ifndef ANIMATED_H
#define ANIMATED_H

#include "Drawable.h"

class Animated :
    public Drawable
{
private:
    bool is_animation_in_progress;
    bool is_white;
public:
    Animated(glm::vec3 Position, glm::vec3 Rotation, glm::vec3 Scale, bool white,VertexArray* VAO, ElementBuffer* EBO, ShaderProgram* SP);
    ~Animated();
    bool isAnimationInProgress();
    bool isWhite();
    void startAnimation();
    void endAnimation();
    int Move(glm::vec3 destination, int phase, float height);
};

#endif