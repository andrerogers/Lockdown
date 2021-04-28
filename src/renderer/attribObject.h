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
#include "vertexBuffer.h"

class VAO {
public:
  GLuint ID;
  VAO();

  void LinkVBO(VBO vbo, GLuint layout);
  void Bind();
  void Unbind();
  void Delete();
};

#endif
