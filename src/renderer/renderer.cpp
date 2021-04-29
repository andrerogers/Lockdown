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

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

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

    // Vertices coordinates
    GLfloat vertices[] =
    { //     COORDINATES     /        COLORS/
        -0.5f, -0.5f,  0.0f,	1.0f, 0.0f, 0.0f,    0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f,		0.0f, 1.0f, 0.0f,    0.0f, 1.0f,
        0.5f, 0.5f, 0.0f,		0.0f, 0.0f, 1.0f,    1.0f, 1.0f,
        0.5f, -0.5f,  0.0f,		1.0f, 1.0f, 1.0f,    1.0f, 0.0f
    };

    // Indices for vertices order
    GLuint indices[] =
    {
        0, 2, 1,
        0, 3, 2 
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

    // Vertex Array Objects store pointers to the different
    // attributes contained in a Vertex Buffer Object
    VAO vao;

    // VAO should be bound before VBO's it points to
    vao.Bind();

    // Vertex Buffer Objects contain that data that descibes
    // a the different attributes of a 3D object
    VBO vbo(vertices, sizeof(vertices));
    EBO ebo(indices, sizeof(indices));

    ebo.Bind();

    vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    vao.LinkAttrib(vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    
    // Clean up so we don't make any changes
    // to the currently bound VAO or VBO
    // good practice..
    vao.Unbind();

    // Make sure to unbind EBO after VAO
    ebo.Unbind();

    // Texture
    int widthImg, heightImg, numColCh;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* bytes = stbi_load("..\\src\\resources\\textures\\tex0.png", &widthImg, &heightImg, &numColCh, 0);

    GLuint texture;

    // Generate a texture unit
    glGenTextures(1, &texture);

    // Assign the texture to a slot
    // in the texture unit
    glActiveTexture(GL_TEXTURE0);

    // Bind the texture with its appropriate
    // type, here 2D texture
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // TODO
    // buggy, if the file has 3 channels, should use GL_RGB, I think,
    // failed when trying render with GL_RGBA
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthImg, heightImg, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(bytes);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint texUni = glGetUniformLocation(shader.ID, "tex0");

    // Update loop
    while (!glfwWindowShouldClose(window)) {
      // Specify the bg color
      glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

      // Clear the color and depth buffer
      glClear(GL_COLOR_BUFFER_BIT);

      shader.Activate();

      // Uniforms for a shader can only be set after
      // the shader has been activated
      glUniform1i(texUni, 0);
      glBindTexture(GL_TEXTURE_2D, texture);

      vao.Bind();

      // glDrawArrays(GL_TRIANGLES, 0, 3); 
      glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

      // Swap the back buffer with the front buffer
      glfwSwapBuffers(window);

      // Poll GLFW Events
      glfwPollEvents();
    }

    // Delete all objects created
    vao.Delete();
    vbo.Delete();
    ebo.Delete();
    shader.Delete();
    glDeleteTextures(1, &texture);

    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}
