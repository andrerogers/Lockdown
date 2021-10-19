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
#include "mesh.h"
#include "texture.h"
#include "camera.h"

using namespace std;

const int WIDTH = 800;
const int HEIGHT = 800;

// Vertices coordinates
// GLfloat vertices[] =
// { //     COORDINATES     /        COLORS        /    TexCoord    /       NORMALS     //
// 	-1.0f, 0.0f,  1.0f,		0.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
// 	-1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 0.0f,		0.0f, 1.0f,		0.0f, 1.0f, 0.0f,
// 	 1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 0.0f,		1.0f, 1.0f,		0.0f, 1.0f, 0.0f,
// 	 1.0f, 0.0f,  1.0f,		0.0f, 0.0f, 0.0f,		1.0f, 0.0f,		0.0f, 1.0f, 0.0f
// };
Vertex vertices[] =
{ //               COORDINATES           /            COLORS          /           TexCoord         /       NORMALS         //
	Vertex{glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
	Vertex{glm::vec3( 1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
	Vertex{glm::vec3( 1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
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

// GLfloat lightVertices[] =
// { //     COORDINATES     //
// 	-0.1f, -0.1f,  0.1f,
// 	-0.1f, -0.1f, -0.1f,
// 	 0.1f, -0.1f, -0.1f,
// 	 0.1f, -0.1f,  0.1f,
// 	-0.1f,  0.1f,  0.1f,
// 	-0.1f,  0.1f, -0.1f,
// 	 0.1f,  0.1f, -0.1f,
// 	 0.1f,  0.1f,  0.1f
// };

Vertex lightVertices[] =
{ //     COORDINATES     //
	Vertex{glm::vec3(-0.1f, -0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f, -0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f,  0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f,  0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f,  0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f,  0.1f,  0.1f)}
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

    Texture textures[] = {
      Texture("..\\src\\resources\\textures\\wood.png", "diffuse", 0, GL_UNSIGNED_BYTE),
      Texture("..\\src\\resources\\textures\\woodSpec.png", "specular", 0, GL_UNSIGNED_BYTE),
    };
    // woodTexture.SetUniformUnit(objectShader, "tex0", 0);
    // woodSpecTexture.SetUniformUnit(objectShader, "tex1", 1);

    // vertices[] => Vertex Shader -> Shape Assembly
    //            -> Geometry Shader -> Rasterizaton
    //            -> Fragment Shader -> Tests and Blending
    //            => pixel

    Shader objectShader("..\\src\\resources\\shaders\\default.vs",
                        "..\\src\\resources\\shaders\\default.fs");
    std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
    std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
    std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));

    // Create floor mesh
    Mesh floor(verts, ind, tex);

    Shader phongLight("..\\src\\resources\\shaders\\phong.vs",
                  "..\\src\\resources\\shaders\\phong.fs");
    std::vector <Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
    std::vector <GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));

    // Crate light mesh
    Mesh light(lightVerts, lightInd, tex);

    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(0.25f, 0.25f, 0.5f);
    glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, lightPos);

    glm::vec3 objectPos = glm::vec3(0.0f, -0.5f, 0.0f);
    glm::mat4 objectModel = glm::mat4(1.0f);
    objectModel = glm::translate(objectModel, objectPos);

    phongLight.Activate();
    glUniformMatrix4fv(glGetUniformLocation(phongLight.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
    glUniform4f(glGetUniformLocation(phongLight.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    
    objectShader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(objectShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
    glUniform4f(glGetUniformLocation(objectShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    glUniform3f(glGetUniformLocation(objectShader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

    glEnable(GL_DEPTH_TEST); 

    Camera camera(WIDTH, HEIGHT, glm::vec3(0.0f, 0.0f, 2.0f));

    // Update loop
    while (!glfwWindowShouldClose(window)) {
      // Specify the bg color
      glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

      // Clear the color and depth buffer
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      camera.Inputs(window);
      camera.UpdateMatrix(45.0f, 0.1f, 100.0f);

      floor.Draw(objectShader, camera);
      light.Draw(phongLight, camera);

      // Swap the back buffer with the front buffer
      glfwSwapBuffers(window);

      // Poll GLFW Events
      glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}
