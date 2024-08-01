#pragma once

#include <GL/glew.h>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__));

// Clears all OpenGl erros
void GLClearError();

// Prints the OpenGL error to the console
bool GLLogCall(const char* function, const char* file, int line);
