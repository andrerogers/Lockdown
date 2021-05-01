#ifndef TEXTURE_H
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Andre Rogers $
   $Notice: (C) Copyright 2020 by Dre Codes, Inc. All Rights Reserved. $
   ======================================================================== */

#define TEXTURE_H
#include <glad/glad.h>

#include <stb/stb_image.h>

#include "shader.h"

class Texture {
public:
    GLuint ID;
    GLenum type;
    Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);
    void Bind();
    void Unbind();
    void Delete();
    void SetUniformUnit(Shader& shader, const char* uniform, GLuint unit);
};

#endif
