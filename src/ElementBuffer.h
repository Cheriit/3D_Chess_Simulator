#pragma once
#ifndef ELEMENT_BUFFER_H
#define ELEMENT_BUFFER_H

#include <GL/glew.h>
class ElementBuffer
{
private:
	GLuint id;
	unsigned int count;
public:
	ElementBuffer(const unsigned int* data, unsigned int count);
	~ElementBuffer();

	void Bind();
	void Unbind();

	inline unsigned int getCount() { return count; }
};

#endif