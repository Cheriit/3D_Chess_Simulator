#pragma once
#include "Board.h"

Board::Board(VertexArray* VAO, ElementBuffer* EBO, ShaderProgram* SP)
	: Drawable(VAO, EBO, SP)
{	
}

Board::~Board()
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (elements[i][j] != nullptr)
			{
				delete elements[i][j];
			}
		}
	}
	Drawable::~Drawable();
}
