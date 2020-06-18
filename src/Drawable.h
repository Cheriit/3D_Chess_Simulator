#pragma once
#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "Drawer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"
#include "Texture.h"
#include "ShaderProgram.h"

// TODO add M matrix and add moving

class Drawable
{
protected:
	Drawable();
	VertexArray *VAO;
	ElementBuffer *EBO;
	ShaderProgram *SP;
	std::vector<Texture*> Tex;
public:
	Drawable(VertexArray* VAO, ElementBuffer* EBO, ShaderProgram* SP);
	~Drawable();
	bool PushTexture(Texture *tex);
	void Draw();
	Drawable* duplicate();
};
#endif