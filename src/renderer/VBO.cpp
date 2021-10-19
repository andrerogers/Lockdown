/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Andre Rogers $
   $Notice: (C) Copyright 2020 by Dre Codes, Inc. All Rights Reserved. $
   ======================================================================== */
#include "VBO.h"

// Vertex Buffer Objects contain that data that descibes
// a the different attributes of a 3D object


VBO::VBO(GLfloat *data, GLsizeiptr size) : BufferObject {data, size} {}

VBO::VBO(vector<Vertex>& vertices) : BufferObject {vertices} {}

void VBO::Bind() { glBindBuffer(GL_ARRAY_BUFFER, ID); }

void VBO::Unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void VBO::Delete() { glDeleteBuffers(1, &ID); }
