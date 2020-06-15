#pragma once
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ShaderProgram.h"
#include "Texture.h"

// TODO add M matrix and add moving
// TODO add generating object

class Drawable
{
private:
	VertexArray *VAO;
	VertexBuffer *VBO;
	IndexBuffer *EBO;
	ShaderProgram *SP;
	std::vector<Texture> Tex;
public:
	Drawable(VertexArray* VAO, VertexBuffer* VBO, IndexBuffer* EBO, ShaderProgram* SP);
	~Drawable();
	bool PushTexture(Texture tex);
	bool PopTexture(Texture tex);
	void Draw();
};
