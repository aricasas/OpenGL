#include <iostream>
#include "GLCall.h"

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLCheckError(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "OpenGL Error: 0x" << std::hex << error << " from " << function << " in " << file << ":" << std::dec << line << std::endl;
        return false;
    }
    return true;
}
