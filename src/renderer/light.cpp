#include <cmath>

#include "light.h"

Light::Light(vec4 color, vec3 pos, Mesh &mesh)
    : color{color}, pos{pos}, mesh{mesh} {}

void Light::Scale(float size) {
    model = glm::scale(model, glm::vec3(size));
}

void Light::Translate(vec3 dst) {
    model = glm::translate(model, dst);
}

void Light::HandleInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
      pos += speed * vec3(0.0f, 1.0f, 0.0f);
    }

    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
      pos += speed * vec3(0.0f, 0.0f, -1.0f);
    }

    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
      pos += speed * vec3(0.0f, 0.0f, 1.0f);
    }

    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
      pos += speed * vec3(0.0f, -1.0f, 0.0f);
    }
}

void Light::Draw(Shader &shader, Camera &camera) {
  // Bind shader to be able to access uniforms
  shader.Activate();
  glUniform4f(glGetUniformLocation(shader.ID, "lightColor"), color.x, color.y, color.z, color.w);
  glUniform3f(glGetUniformLocation(shader.ID, "translate"), pos.x, pos.y, pos.z);
  glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

  mesh.Draw(shader, camera);
} 
