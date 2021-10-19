#ifndef VAO_H
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Andre Rogers $
   $Notice: (C) Copyright 2020 by Dre Codes, Inc. All Rights Reserved. $
   ======================================================================== */

#define VAO_H
#include <glad/glad.h>
#include "VBO.h"

class VAO {
public:
  GLuint ID;
  VAO();

  void LinkAttrib(VBO& vbo, GLuint layout, GLuint numComponents, GLenum type,
                  GLsizeiptr stride, void* offset);
  void Bind();
  void Unbind();
  void Delete();
};

#endif
