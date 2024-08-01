#include "Renderer.h"

#include <iostream>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__));

// Clears all OpenGl erros
void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

// Prints the OpenGL error to the console
bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (" << error << "): "<< function << " | " << file << ":" << line << '\n';
        return false;
    }
    return true;
}
