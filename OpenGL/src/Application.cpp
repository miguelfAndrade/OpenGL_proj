#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// Struct to save the shader
struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

// Loads the shader file and parses it into the shader struct
static ShaderProgramSource ParseShader(const std::string& filepath) {
    std::ifstream stream(filepath);

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        }
        else {
            ss[(int) type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

// 
static unsigned int CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)_alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader\n";
        std::cout << message << "\n";

        glDeleteShader(id);
        return 0;
    }

    return id;
}


static unsigned int CreateShaders(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}


int main()
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    // Sets the OpenGl version to 3 and sets the OpenGL profile to CORE
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // Sets the refresh rate to the monitor refresh rate
    // glfwSwapInterval(2); // 120Hz
    glfwSwapInterval(1); // 60Hz

    GLenum err;
    err = glewInit();

    if (err != GLEW_OK) {
        std::cout << "GLEW ERROR!" << std::endl;
        std::cout << glewGetErrorString(err) << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;
    {
        // Array for the squares vertices
        float square[] = {
            -0.5f, -0.5f, // 0
             0.5f, -0.5f, // 1
             0.5f,  0.5f, // 2
            -0.5f,  0.5f  // 3
        };

        float triangle[] = {
           -0.95f, 0.0f, // 0
           -0.55f, 0.75f, // 1
           -0.55f, 0.0f, // 2
        };

        // Draw order for the vertices of the square
        unsigned int squareIndices[] = {
            0, 1, 2,
            2, 3, 0
        };

        unsigned int triangleIndices[] = {
            0, 1, 2,
        };


        // Creates a Vertex Array Object and binds it to a Vertex Array
        /*unsigned int vao;
        GLCall(glGenVertexArrays(1, &vao));
        GLCall(glBindVertexArray(vao));*/

        VertexArray va;
        VertexBuffer vb(square, sizeof(square) * 2);
        VertexBufferLayout layout;
        //layout.Push<float>(2);
        layout.Push(GL_FLOAT, 2);
        va.AddBuffer(vb, layout);

        IndexBuffer ib(squareIndices, sizeof(squareIndices));

        ShaderProgramSource source = ParseShader("shaders/simpleShader.shader");

        unsigned int programID = CreateShaders(source.VertexSource, source.FragmentSource);
        GLCall(glUseProgram(programID));

        GLCall(int location = glGetUniformLocation(programID, "u_Color"));
        ASSERT(location != -1);
        GLCall(glUniform4f(location, 0.8f, 0.8f, 0.8f, 1.0f));

        va.Unbind();
        GLCall(glUseProgram(0));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));


        float r = 0.0f;
        float increment = 0.05f;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            GLCall(glClear(GL_COLOR_BUFFER_BIT));

            GLCall(glUseProgram(programID));
            GLCall(glUniform4f(location, r, 0.8f, 0.8f, 1.0f));

            // GLCall(glBindVertexArray(vao));
            va.Bind();
            ib.Bind();

            GLCall(glDrawElements(GL_TRIANGLES, sizeof(squareIndices) / sizeof(unsigned int), GL_UNSIGNED_INT, nullptr));

            if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0.0f)
                increment = 0.05f;

            r += increment;

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

        glDeleteProgram(programID);
    }

    glfwTerminate();
    return 0;
}
