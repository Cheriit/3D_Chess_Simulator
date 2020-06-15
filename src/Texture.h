#pragma once
#include <GL/glew.h>
#include <iostream>
#include "libs/lodepng.h"

class Texture
{
public:
	GLuint id;
	Texture(const char* filename);
	~Texture();
	void Bind(unsigned int index, unsigned int unit_id, unsigned int texture_unit);
	void Unbind(unsigned int texture_unit);
};

