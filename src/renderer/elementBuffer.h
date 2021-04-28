#ifndef ELEMENT_BUFFER_H
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Andre Rogers $
   $Notice: (C) Copyright 2020 by Dre Codes, Inc. All Rights Reserved. $
   ======================================================================== */

#define ELEMENT_BUFFER_H
#include "bufferObject.h"

class EBO : BufferObject {
public:
    EBO(GLuint* data, GLsizeiptr size);

    void Bind();
    void Unbind();
    void Delete();
};

#endif
