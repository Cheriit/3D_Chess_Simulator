#include "Drawable.h"

Drawable::Drawable(VertexArray* VAO, ElementBuffer* EBO, ShaderProgram* SP)
	: VAO(VAO), EBO(EBO), SP(SP)
{
}

Drawable::~Drawable()
{
}

bool Drawable::PushTexture(Texture *tex)
{
    bool is_in_vector = false;
    for (int i = 0; i < Tex.size(); i++)
        is_in_vector = Tex[i] == tex ? true : is_in_vector;
    if (!is_in_vector)
        Tex.push_back(tex);
	return is_in_vector;
}

void Drawable::Draw()
{
    VAO->Bind();
    EBO->Bind();
    for (int i = 0; i < Tex.size(); i++)
    {
        Tex[i]->Bind(SP->u(Tex[i]->getUniformName()), i, GL_TEXTURE0+i );
    }
    GLCall(glDrawElements(GL_TRIANGLES, EBO->getCount(), GL_UNSIGNED_INT, nullptr));
    VAO->Unbind();
    EBO->Unbind();
    for (int i = 0; i < Tex.size(); i++)
    {
        Tex[i]->Bind(SP->u(Tex[i]->getUniformName()), i, GL_TEXTURE0 + i);
    }
}

Drawable* Drawable::duplicate()
{
    Drawable* duplicate = new Drawable(VAO, EBO, SP);
    for (int i = 0; i < Tex.size(); i++)
    {
        duplicate->PushTexture(Tex[i]);
    }
    return duplicate;
}