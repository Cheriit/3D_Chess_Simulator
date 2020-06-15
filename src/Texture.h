#pragma once
#include <GL/glew.h>
#include <iostream>
#include "libs/lodepng.h"

class Texture
{
private:
	GLuint id;
public:
	Texture(const char* filename);
	~Texture();
	void Bind(unsigned int index, unsigned int texture_unit);
	void Unbind(unsigned int texture_unit);
};

