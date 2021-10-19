#ifndef EBO_H
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Andre Rogers $
   $Notice: (C) Copyright 2020 by Dre Codes, Inc. All Rights Reserved. $
   ======================================================================== */

#define EBO_H
#include <vector>
#include "bufferObject.h"

using namespace std;

class EBO : BufferObject {
public:
    EBO(vector<GLuint>& indices);
    EBO(GLuint* data, GLsizeiptr size);

    void Bind();
    void Unbind();
    void Delete();
};

#endif
