/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Andre Rogers $
   $Notice: (C) Copyright 2020 by Dre Codes, Inc. All Rights Reserved. $
   ======================================================================== */
#include "shader.h"

string get_file_contents(const char *filename) {
    ifstream in(filename, std::ios::binary);

    if(in) {
        string contents;

        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();

        return(contents);
    }

    throw(errno);
}

Shader::Shader(const char *vertexFile, const char *fragmentFile) {
    string vertexCode = get_file_contents(vertexFile);
    string fragmentCode = get_file_contents(fragmentFile);

    const char* vertexSource = vertexCode.c_str();
    const char* fragmentSource = fragmentCode.c_str();

    // Create a shader object
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Load the vertex shader source code
    glShaderSource(vertexShader, 1, &vertexSource, NULL);

    // Source cannot be compiled on GPU, so compile
    // into machine code now
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    // Create a shader program
    ID = glCreateProgram();

    // Attach the shaders to the shader program object
    // by providing references to each shaders
    // compiled code
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);

    // Wrap up program creation
    glLinkProgram(ID);

    // Clean up
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::Activate() {
    glUseProgram(ID);
}

void Shader::Delete() {
    glDeleteProgram(ID);
}
