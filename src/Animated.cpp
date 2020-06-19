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

Animated* Animated::Duplicate()
{
    Animated* duplicate = new Animated(GetPosition(), GetRotation(), VAO, EBO, SP);
    //for (int i = 0; i < Tex.size(); i++)
    //{
    //    duplicate->PushTexture(Tex[i]);
    //}
    return duplicate;
}
