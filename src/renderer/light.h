#ifndef LIGHT_H
#define LIGHT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>

#include "mesh.h"

using namespace std;
using namespace glm;

class Light {
public:
  vec4 color;
  vec3 pos;
  mat4 model = mat4(1.0f);
  float speed = 0.04f;

  // Initializes the mesh
  Light(vec4 color, vec3 pos, Mesh& mesh);

  // Transformations
  void Scale(float size);
  void Translate(vec3 dst);

  void HandleInput(GLFWwindow* window);

  // Draws the mesh
  void Draw(Shader& shader, Camera& camera);

private:
  Mesh mesh;
};

#endif LIGHT_H
