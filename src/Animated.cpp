#pragma once
#include "Animated.h"

Animated::Animated(glm::vec3 Position, glm::vec3 Rotation, VertexArray* VAO, ElementBuffer* EBO, ShaderProgram* SP)
	: Drawable(VAO, EBO, SP)
{
    is_animation_in_progress = false;
	SetPosition(Position);
	SetRotation(Rotation);
}
Animated::~Animated()
{
	Drawable::~Drawable();
}

void Animated::animate(){

}

bool Animated::isAnimationInProgress(){
    return is_animation_in_progress;
}

void Animated::startAnimation(){
    is_animation_in_progress = true;
}

void Animated::endAnimation(){
    is_animation_in_progress = false;
}
