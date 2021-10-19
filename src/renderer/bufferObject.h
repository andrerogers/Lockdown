#ifndef BUFFER_OBJECT_H
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Andre Rogers $
   $Notice: (C) Copyright 2020 by Dre Codes, Inc. All Rights Reserved. $
   ======================================================================== */

#define BUFFER_OBJECT_H
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

using namespace std;
using namespace glm;

struct Vertex
{
  vec3 position;
  vec3 normal;
  vec3 color;
  vec2 texUV;
};

class BufferObject {
public:
  GLuint ID;
  BufferObject(vector<Vertex>& vertices);
  BufferObject(GLfloat *data, GLsizeiptr size);

  BufferObject(vector<GLuint>& indices);
  BufferObject(GLuint *data, GLsizeiptr size);

  virtual void Bind();
  virtual void Unbind();
  virtual void Delete();
};

#endif
