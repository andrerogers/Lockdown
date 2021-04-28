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

class BufferObject {
public:
  GLuint ID;
  BufferObject(GLfloat *data, GLsizeiptr size);
  BufferObject(GLuint *data, GLsizeiptr size);

  virtual void Bind();
  virtual void Unbind();
  virtual void Delete();
};

#endif
