#include "Drawable.h"

Drawable::Drawable(VertexArray* VAO, VertexBuffer* VBO, IndexBuffer* EBO, ShaderProgram* SP)
	: VAO(VAO), VBO(VBO), EBO(EBO), SP(SP)
{
}
Drawable::~Drawable()
{
}
bool Drawable::PushTexture(Texture tex)
{
	return true;
}
bool Drawable::PopTexture(Texture tex)
{
	return true;
}
void Drawable::Draw()
{

}