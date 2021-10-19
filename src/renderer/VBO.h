#ifndef VBO_H
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Andre Rogers $
   $Notice: (C) Copyright 2020 by Dre Codes, Inc. All Rights Reserved. $
   ======================================================================== */

#define VBO_H
#include <vector>

#include "bufferObject.h"

using namespace std;

class VBO : BufferObject {
public:
    VBO(GLfloat* data, GLsizeiptr size);
    VBO(vector<Vertex>& vertices);

    void Bind();
    void Unbind();
    void Delete();
};

#endif
