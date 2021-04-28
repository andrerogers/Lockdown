/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Andre Rogers $
   $Notice: (C) Copyright 2020 by Dre Codes, Inc. All Rights Reserved. $
   ======================================================================== */
#include "bufferObject.h"

// For VBO's
BufferObject::BufferObject(GLfloat *data, GLsizeiptr size) {
  // Create Vertex Buffer Object VBO
  glGenBuffers(1, &ID);

  // Binding in OpenGL refers to the process of
  // making an OpenGL object the current object
  // threefore, whenever a function is executed
  // it will modify the object that is bound i.e
  // the current object
  glBindBuffer(GL_ARRAY_BUFFER, ID);

  // Specify the data the bufferr should hold
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

// For EBO's
BufferObject::BufferObject(GLuint *data, GLsizeiptr size) {
  // Create Vertex Buffer Object EBO
  glGenBuffers(1, &ID);

  // Binding in OpenGL refers to the process of
  // making an OpenGL object the current object
  // threefore, whenever a function is executed
  // it will modify the object that is bound i.e
  // the current object
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);

  // Specify the data the bufferr should hold
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void BufferObject::Bind() {}

void BufferObject::Unbind() {}

void BufferObject::Delete() {}
