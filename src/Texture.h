#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <iostream>
#include <vector>

class Texture
{
private:
	GLuint id;
	const char* uniform_name;
public:
	Texture(const char* filename, const char* uniform_name);
	~Texture();
	const char* getUniformName();
	void Bind(unsigned int index, unsigned int unit_id, unsigned int texture_unit);
	void Unbind(unsigned int texture_unit);
};

#endif