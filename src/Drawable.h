#pragma once
#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "Drawer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"
#include "Texture.h"
#include "ShaderProgram.h"
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/type_ptr.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

// TODO add M matrix and add moving

class Drawable
{
protected:
	Drawable();
	VertexArray *VAO;
	ElementBuffer *EBO;
	ShaderProgram *SP;
	std::vector<Texture*> Tex;
	glm::vec3 Position;
	glm::vec3 Rotation;
public:
	Drawable(VertexArray* VAO, ElementBuffer* EBO, ShaderProgram* SP);
	~Drawable();
	bool PushTexture(Texture *tex);
    std::vector<Texture*> GetTextures();
	void Draw(glm::mat4 M);
	void SetPosition(glm::vec3 pos);
	void Move(glm::vec3 pos);
	glm::vec3 GetPosition();
	void Rotate(glm::vec3 rot);
	void SetRotation(glm::vec3 rot);
	glm::vec3 GetRotation();
};
#endif