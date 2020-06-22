#pragma once
#include "Animated.h"

Animated::Animated(glm::vec3 Position, glm::vec3 Rotation, bool white, VertexArray* VAO, ElementBuffer* EBO, ShaderProgram* SP)
	: Drawable(VAO, EBO, SP)
{
    is_animation_in_progress = false;
    this->is_white = white;
	SetPosition(Position);
	SetRotation(Rotation);
}
Animated::~Animated()
{
	Drawable::~Drawable();
}

bool Animated::isAnimationInProgress(){
    return is_animation_in_progress;
}

bool Animated::isWhite(){
    return is_white;
}

void Animated::startAnimation(){
    is_animation_in_progress = true;
}

void Animated::endAnimation(){
    is_animation_in_progress = false;
}
