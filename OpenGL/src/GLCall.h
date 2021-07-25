#pragma once

#include <GL/glew.h>


#define ASSERT(x) if (!(x)) __debugbreak(); // Make debugger break if x is false
#define GLCall(x) x;\
ASSERT(GLCheckError(#x, __FILE__, __LINE__)) // Check for GL errors


void GLClearError();
bool GLCheckError(const char* function, const char* file, int line);
