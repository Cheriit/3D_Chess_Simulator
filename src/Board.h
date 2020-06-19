#pragma once
#ifndef BOARD_H
#define BOARD_H

#include "Drawable.h"
#include "Animated.h"

//TODO Board class
class Board :
    public Drawable
{
private:
    Animated* elements[8][8] = { nullptr };
public:
    Board(VertexArray* VAO, ElementBuffer* EBO, ShaderProgram* SP);
    ~Board();
};

#endif