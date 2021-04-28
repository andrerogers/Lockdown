/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Andre Rogers $
   $Notice: (C) Copyright 2020 by Dre Codes, Inc. All Rights Reserved. $
   ======================================================================== */

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "vertexBuffer.h"
#include "elementBuffer.h"
#include "attribObject.h"

using namespace std;

const int WIDTH = 800;
const int HEIGHT = 800;

int main() {
    // Initialize GLFW
    glfwInit();

    // Tell GLFW what version of OpenGL to user32
    // using 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLfloat vertices[] = {
        -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,    // Lower Left Corner
        0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,     // Lower Right Corner
        0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f,  // Upper Corner
        -0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,    // Inner Left Corner
        0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,     // Inner Right Corner
        0.0f, -0.5f * float(sqrt(3)) * 2 / 3, 0.0f  // Inner Down
    };

    GLuint indices[] = {
        0, 3, 5, // Lower Left Triangle
        3, 2, 4, // Lower Right Triangle
        5, 4, 1  // Upper Triangle
    };

    // Create a GLFW window of WIDTH, and HEIGHT
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Lockdown", NULL, NULL);

    // Check if the window fails to get created 
    if(window == nullptr) {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();

        return -1;
    }

    // Introduce the window into the current context
    glfwMakeContextCurrent(window);

    // Load GLAD so it configures OpenGL
    gladLoadGL();

    // Specify the viewport of OpenGL in the window
 // Here, from 0, 0 to WIDTH, HEIGHT
    glViewport(0, 0, WIDTH, HEIGHT);

    // vertices[] => Vertex Shader -> Shape Assembly
    //            -> Geometry Shader -> Rasterizaton
    //            -> Fragment Shader -> Tests and Blending
    //            => pixel

    Shader shader("..\\src\\resources\\shaders\\default.vs",
                  "..\\src\\resources\\shaders\\default.fs");

    VAO vao;
    vao.Bind();

    VBO vbo(vertices, sizeof(vertices));
    vbo.Bind();

    EBO ebo(indices, sizeof(indices));
    ebo.Bind();

    vao.LinkVBO(vbo, 0);
    
    // Clean up so we don't make any changes
    // to the currently bound VAO or VBO
    // good practice..
    vbo.Unbind();

    // glBindVertexArray(0);
    vao.Unbind();

    // Make sure to unbind EBO after VAO
    ebo.Unbind();

    // Update loop
    while (!glfwWindowShouldClose(window)) {
      // Specify the bg color
      glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

      // Clear the color and depth buffer
      glClear(GL_COLOR_BUFFER_BIT);

      // glUseProgram(shaderProgram);
      shader.Activate();

      // glBindVertexArray(VAO);
      vao.Bind();
      // glDrawArrays(GL_TRIANGLES, 0, 3);
      glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

      // Swap the back buffer with the front buffer
      glfwSwapBuffers(window);

      // Poll GLFW Events
      glfwPollEvents();
    }

    // Delete all objects created
    // glDeleteVertexArrays(1, &VAO);
    vao.Delete();

    // glDeleteBuffers(1, &VBO);
    vbo.Delete();
    // glDeleteBuffers(1, &EBO);
    ebo.Delete();

    // glDeleteProgram(shaderProgram);
    shader.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
