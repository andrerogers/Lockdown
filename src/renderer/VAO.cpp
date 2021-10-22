/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Andre Rogers $
   $Notice: (C) Copyright 2020 by Dre Codes, Inc. All Rights Reserved. $
   ======================================================================== */
#include "VAO.h"

// Vertex Array Objects store pointers to the different
// attributes contained in a Vertex Buffer Object

VAO::VAO() {
    glGenVertexArrays(1, &ID);
}

void VAO::LinkAttrib(VBO& vbo, GLuint layout, GLuint numComponents, GLenum type,
                  GLsizeiptr stride, void* offset) {
    vbo.Bind();
    glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);

    // Enable the VAO,
    // so that OpenGL knows to use it!!
    glEnableVertexAttribArray(layout);

    vbo.Unbind();
}

// VAO should be bound before VBO it points to
void VAO::Bind() { glBindVertexArray(ID); }

void VAO::Unbind() { glBindVertexArray(0); }

void VAO::Delete() { glDeleteVertexArrays(1, &ID); }
