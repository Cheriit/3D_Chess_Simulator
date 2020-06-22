#include "Drawable.h"

Drawable::Drawable() {
    VAO = nullptr;
    EBO = nullptr;
    SP = nullptr;
}

Drawable::Drawable(VertexArray *VAO, ElementBuffer *EBO, ShaderProgram *SP)
        : VAO(VAO), EBO(EBO), SP(SP) {
    Position = glm::vec3(0, 0, 0);
    Rotation = glm::vec3(0, 0, 0);
}

Drawable::~Drawable() {
}

bool Drawable::PushTexture(Texture *tex) {
    bool is_in_vector = false;
    for (int i = 0; i < Tex.size(); i++)
        is_in_vector = Tex[i] == tex ? true : is_in_vector;
    if (!is_in_vector)
        Tex.push_back(tex);
    return is_in_vector;
}

std::vector<Texture *> Drawable::GetTextures() {
    return Tex;
}

void Drawable::Draw(glm::mat4 M) {
    SP->use();

    M = glm::translate(M, Position);
    M = glm::rotate(M, Rotation.x, glm::vec3(1, 0, 0));
    M = glm::rotate(M, Rotation.y, glm::vec3(0, 1, 0));
    M = glm::rotate(M, Rotation.z, glm::vec3(0, 0, 1));
    glUniformMatrix4fv(SP->u("M"), 1, false, glm::value_ptr(M));

    VAO->Bind();
    EBO->Bind();
    for (int i = 0; i < Tex.size(); i++) {
        Tex[i]->Bind(SP->u(Tex[i]->getUniformName()), i, GL_TEXTURE0 + i);
    }
    glDrawElements(GL_TRIANGLES, EBO->getCount(), GL_UNSIGNED_INT, nullptr);
    VAO->Unbind();
    EBO->Unbind();
    for (int i = 0; i < Tex.size(); i++) {
        Tex[i]->Bind(SP->u(Tex[i]->getUniformName()), i, GL_TEXTURE0 + i);
    }
}

void Drawable::SetPosition(glm::vec3 pos) {
    Position = pos;
}

void Drawable::Move(glm::vec3 pos) {
    Position = Position + pos;
}

glm::vec3 Drawable::GetPosition() {
    return Position;
}

void Drawable::Rotate(glm::vec3 rot) {
    Rotation = Rotation + rot;
}

void Drawable::SetRotation(glm::vec3 rot) {
    Rotation = rot;
}

glm::vec3 Drawable::GetRotation() {
    return Rotation;
}
