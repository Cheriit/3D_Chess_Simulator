#pragma once
#include <GL/glew.h>
class IndexBuffer
{
private:
	GLuint id;
	unsigned int count;
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind();
	void Unbind();

	inline unsigned int getCount() { return count; }
};

