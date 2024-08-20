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
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600


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

        VertexArray va;
        VertexBuffer vb(square, sizeof(square) * 2);
        VertexBufferLayout layout;
        //layout.Push<float>(2);
        layout.Push(GL_FLOAT, 2);
        va.AddBuffer(vb, layout);

        IndexBuffer ib(squareIndices, sizeof(squareIndices) / sizeof(unsigned int));

        Shader shader("shaders/simpleShader.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.8f, 0.8f, 0.8f, 1.0f);

        va.Unbind();
        shader.Unbind();
        vb.Unbind();
        ib.Unbind();

        float r = 0.0f;
        float increment = 0.05f;

        VertexArray vaTriangle;
        VertexBuffer vbTriangle(triangle, sizeof(triangle) * 2);
        VertexBufferLayout layoutTriangle;
        layoutTriangle.Push(GL_FLOAT, 2);
        vaTriangle.AddBuffer(vbTriangle, layoutTriangle);
        IndexBuffer ibTriangle(triangleIndices, sizeof(triangleIndices) / sizeof(unsigned int));
        vaTriangle.Unbind();
        vbTriangle.Unbind();
        ibTriangle.Unbind();

        Renderer renderer;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            shader.Bind();
            shader.SetUniform4f("u_Color", r, 0.8f, 0.8f, 1.0f);

            renderer.Draw(va, ib, shader);
            renderer.Draw(vaTriangle, ibTriangle, shader);

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
    }

    glfwTerminate();
    return 0;
}
