#ifndef VERTEX_BUFFER_H
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Andre Rogers $
   $Notice: (C) Copyright 2020 by Dre Codes, Inc. All Rights Reserved. $
   ======================================================================== */

#define VERTEX_BUFFER_H
#include "bufferObject.h"

class VBO : BufferObject {
public:
    VBO(GLfloat* data, GLsizeiptr size);

    void Bind();
    void Unbind();
    void Delete();
};

#endif
