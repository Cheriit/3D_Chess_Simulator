#pragma once
#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <GL/glew.h>
class VertexBuffer
{
private:
	GLuint id;
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind();
	void Unbind();
};
#endif
