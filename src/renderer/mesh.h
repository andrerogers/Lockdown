#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include <string>

#include "VAO.h"
#include "EBO.h"
#include "camera.h"
#include "texture.h"

using namespace std;

class Mesh {
public:
  vector<Vertex> vertices;
  vector<GLuint> indices;
  vector<Texture> textures;

  // Store VAO in public so it can be used in the Draw function
  VAO VAO;

  // Initializes the mesh
  Mesh(vector<Vertex>& vertices, vector<GLuint>& indices, vector<Texture>& textures);

  // Draws the mesh
  void Draw(Shader& shader, Camera& camera);
};

#endif MESH_CLASS_H
