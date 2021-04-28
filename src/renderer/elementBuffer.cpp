/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Andre Rogers $
   $Notice: (C) Copyright 2020 by Dre Codes, Inc. All Rights Reserved. $
   ======================================================================== */
#include "elementBuffer.h"

EBO::EBO(GLuint *data, GLsizeiptr size) : BufferObject {data, size} {}

void EBO::Bind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID); }

void EBO::Unbind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

void EBO::Delete() { glDeleteBuffers(1, &ID); }
