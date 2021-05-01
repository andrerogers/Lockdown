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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "vertexBuffer.h"
#include "elementBuffer.h"
#include "attribObject.h"
#include "texture.h"
#include "camera.h"

using namespace std;

const int WIDTH = 800;
const int HEIGHT = 800;

// Vertices coordinates
GLfloat vertices[] =
{ //     COORDINATES     /        COLORS        /    TexCoord    /       NORMALS     //
	-1.0f, 0.0f,  1.0f,		0.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
	-1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 0.0f,		0.0f, 1.0f,		0.0f, 1.0f, 0.0f,
	 1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 0.0f,		1.0f, 1.0f,		0.0f, 1.0f, 0.0f,
	 1.0f, 0.0f,  1.0f,		0.0f, 0.0f, 0.0f,		1.0f, 0.0f,		0.0f, 1.0f, 0.0f
};

// Indices for vertices order
GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3
};

// // Pyramind
// // Vertices coordinates
// GLfloat vertices[] = {
//     // COORDINATES      /        COLORS        /   TEXCOORDS  /       NORMALS
//     -0.5f, 0.0f, 0.5f,    0.83f, 0.70f, 0.44f,    0.0f, 0.0f,    0.0f, -1.0f, 0.0f, // Bottom Side
//     -0.5f, 0.0f, -0.5f,    0.83f, 0.70f, 0.44f,    0.0f, 5.0f,    0.0f, -1.0f, 0.0f, // Bottom Side
//     0.5f, 0.0f, -0.5f,    0.83f, 0.70f, 0.44f,    5.0f, 5.0f,    0.0f, -1.0f, 0.0f, // Bottom Side
//     0.5f, 0.0f, 0.5f,    0.83f, 0.70f, 0.44f,    5.0f, 0.0f,    0.0f, -1.0f, 0.0f, // Bottom Side

//     -0.5f, 0.0f, 0.5f,    0.83f, 0.70f, 0.44f,    0.0f, 0.0f,    -0.8f, 0.5f, 0.0f, // Left Side
//     -0.5f, 0.0f, -0.5f,    0.83f, 0.70f, 0.44f,    5.0f, 0.0f,    -0.8f, 0.5f, 0.0f, // Left Side
//     0.0f, 0.8f, 0.0f,    0.92f, 0.86f, 0.76f,    2.5f, 5.0f,    -0.8f, 0.5f, 0.0f, // Left Side

//     -0.5f, 0.0f, -0.5f,    0.83f, 0.70f, 0.44f,    5.0f, 0.0f,    0.0f, 0.5f, -0.8f, // Non-facing Side
//     0.5f, 0.0f, -0.5f,    0.83f, 0.70f, 0.44f,    0.0f, 0.0f,    0.0f, 0.5f, -0.8f, // Non-facing Side
//     0.0f, 0.8f, 0.0f,    0.92f, 0.86f, 0.76f,    2.5f, 5.0f,    0.0f, 0.5f, -0.8f, // Non-facing Side

//     0.5f, 0.0f, -0.5f,    0.83f, 0.70f, 0.44f,    0.0f, 0.0f,    0.8f, 0.5f, 0.0f, // Right Side
//     0.5f, 0.0f, 0.5f,    0.83f, 0.70f, 0.44f,    5.0f, 0.0f,    0.8f, 0.5f, 0.0f, // Right Side
//     0.0f, 0.8f, 0.0f,    0.92f, 0.86f, 0.76f,    2.5f, 5.0f,    0.8f, 0.5f, 0.0f, // Right Side

//     0.5f, 0.0f, 0.5f,    0.83f, 0.70f, 0.44f,    5.0f, 0.0f,    0.0f, 0.5f, 0.8f, // Facing Side
//     -0.5f, 0.0f, 0.5f,    0.83f, 0.70f, 0.44f,    0.0f, 0.0f,    0.0f, 0.5f, 0.8f, // Facing Side
//     0.0f, 0.8f, 0.0f,    0.92f, 0.86f, 0.76f,    2.5f, 5.0f,    0.0f, 0.5f, 0.8f, // Facing Side
//     };

// // Indices for vertices order
// GLuint indices[] = {
//     0, 1, 2, // Bottom Side
//     0, 2, 3, // Bottom Side
//     4, 6, 5, // Left Side
//     7, 9, 8, // Non-facing Side
//     10, 12, 11, // Right Side
//     13, 15, 14 //Facing Side
//     };

// // Pyramind
// // Vertices coordinates
// GLfloat vertices[] =
// { //     COORDINATES     /        COLORS/
//     -0.5f, 0.0f,  0.5f,	    0.83f, 0.70f, 0.44f,    0.0f, 0.0f,
//     -0.5f, 0.0f, -0.5f,		0.83f, 0.70f, 0.44f,    5.0f, 0.0f,
//     0.5f, 0.0f, -0.5f,		0.83f, 0.70f, 0.44f,    0.0f, 0.0f,
//     0.5f, 0.0f,  0.5f,		0.83f, 0.70f, 0.44f,    5.0f, 0.0f,
//     0.0f, 0.8f,  0.0f,		0.92f, 0.86f, 0.76f,    2.5f, 5.0f
// };

// // Indices for vertices order
// GLuint indices[] =
// {
//     0, 1, 2,
//     0, 2, 3,
//     0, 1, 4,
//     1, 2, 4,
//     2, 3, 4,
//     3, 0, 4
// };

// // Square
// // Vertices coordinates
// GLfloat vertices[] =
// { //     COORDINATES     /        COLORS/
//     -0.5f, -0.5f,  0.0f,	1.0f, 0.0f, 0.0f,    0.0f, 0.0f,
//     -0.5f, 0.5f, 0.0f,		0.0f, 1.0f, 0.0f,    0.0f, 1.0f,
//     0.5f, 0.5f, 0.0f,		0.0f, 0.0f, 1.0f,    1.0f, 1.0f,
//     0.5f, -0.5f,  0.0f,		1.0f, 1.0f, 1.0f,    1.0f, 0.0f
// };

// // Indices for vertices order
// GLuint indices[] =
// {
//     0, 2, 1,
//     0, 3, 2 
// };

GLfloat lightVertices[] =
{ //     COORDINATES     //
	-0.1f, -0.1f,  0.1f,
	-0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f,  0.1f,
	-0.1f,  0.1f,  0.1f,
	-0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f,  0.1f
};

GLuint lightIndices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
};

int main() {
    // Initialize GLFW
    glfwInit();

    // Tell GLFW what version of OpenGL to user32
    // using 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

    vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
    vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
    vao.LinkAttrib(vbo, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
    vao.LinkAttrib(vbo, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));
    
    // Clean up so we don't make any changes
    // to the currently bound VAO or VBO
    // good practice..
    vao.Unbind();

    // Make sure to unbind EBO after VAO
    ebo.Unbind();

    Shader light("..\\src\\resources\\shaders\\light.vs",
                  "..\\src\\resources\\shaders\\light.fs");

    VAO lightVAO;
    lightVAO.Bind();

    VBO lightVBO(lightVertices, sizeof(lightVertices));
    EBO lightEBO(lightIndices, sizeof(lightIndices));
    lightEBO.Bind();

    lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

    lightVAO.Unbind();

    lightEBO.Unbind();

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(0.25f, 0.25f, 0.5f);
    glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, lightPos);

    glm::vec3 objectPos = glm::vec3(0.0f, -0.5f, 0.0f);
    glm::mat4 objectModel = glm::mat4(1.0f);
    objectModel = glm::translate(objectModel, objectPos);

    light.Activate();
    glUniformMatrix4fv(glGetUniformLocation(light.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
    glUniform4f(glGetUniformLocation(light.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    
    shader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
    glUniform4f(glGetUniformLocation(shader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    glUniform3f(glGetUniformLocation(shader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

    Texture woodTexture("..\\src\\resources\\textures\\wood.png", GL_TEXTURE_2D, 0, GL_UNSIGNED_BYTE);
    Texture woodSpecTexture("..\\src\\resources\\textures\\woodSpec.png", GL_TEXTURE_2D, 1, GL_UNSIGNED_BYTE);
    woodTexture.SetUniformUnit(shader, "tex0", 0);
    woodSpecTexture.SetUniformUnit(shader, "tex1", 1);

    // float rotation = 0.0f;
    // double prevTime = glfwGetTime();

    glEnable(GL_DEPTH_TEST); 

    Camera camera(WIDTH, HEIGHT, glm::vec3(0.0f, 0.0f, 2.0f));

    // Update loop
    while (!glfwWindowShouldClose(window)) {
      // Specify the bg color
      glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

      // Clear the color and depth buffer
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // double currentTime = glfwGetTime();
      // if(currentTime - prevTime >= 1 / 60) {
      //     rotation += 0.5f;
      //     prevTime = currentTime;
      // }

      camera.Inputs(window);
      camera.UpdateMatrix(45.0f, 0.1f, 100.0f);

      shader.Activate();
    glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.m_position.x, camera.m_position.y, camera.m_position.z);
      camera.SetUniform(shader, "camMatrix");
      woodTexture.Bind();
      woodSpecTexture.Bind();
      vao.Bind();
      // glDrawArrays(GL_TRIANGLES, 0, 3); 
      glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

      light.Activate();
      camera.SetUniform(light, "camMatrix");
      lightVAO.Bind();
      glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);

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
    woodTexture.Delete();
    woodSpecTexture.Delete();

    lightVAO.Delete();
    lightVBO.Delete();
    lightEBO.Delete();
    light.Delete();

    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}
