/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Andre Rogers $
   $Notice: (C) Copyright 2020 by Dre Codes, Inc. All Rights Reserved. $
   ======================================================================== */
#include "vertexBuffer.h"

VBO::VBO(GLfloat *data, GLsizeiptr size) : BufferObject {data, size} {}

void VBO::Bind() { glBindBuffer(GL_ARRAY_BUFFER, ID); }

void VBO::Unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void VBO::Delete() { glDeleteBuffers(1, &ID); }
