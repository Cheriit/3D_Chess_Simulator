#pragma once

#include <iostream>
#include <GL/glew.h>

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) glClearError();\
    x;\
    ASSERT(glLogCall())

void glClearError();
bool glLogCall();