#include "Drawer.h"

void glClearError() {
    while (glGetError() != GL_NO_ERROR);
}

bool glLogCall() {
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL error]: " << error << std::endl;
        return false;
    }
    return true;
}