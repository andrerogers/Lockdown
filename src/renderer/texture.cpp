/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Andre Rogers $
   $Notice: (C) Copyright 2020 by Dre Codes, Inc. All Rights Reserved. $
   ======================================================================== */
#include "texture.h"

Texture::Texture(const char *image, const char* texType, GLuint slot,
                 GLenum pixelType) {
  type = texType;

  // Texture
  int widthImg, heightImg, numColCh;

  stbi_set_flip_vertically_on_load(true);

  unsigned char *bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);

  if (numColCh == 4)
    format = GL_RGBA;
  else if(numColCh == 3)
    format = GL_RGB;
  else if(numColCh == 1)
    format = GL_RED;

  // Generate a texture unit
  glGenTextures(1, &ID);

  // Assign the texture to a slot
  // in the texture unit
  glActiveTexture(GL_TEXTURE0 + slot);
  unit = slot;

  // Bind the texture with its appropriate
  // type, here 2D texture
  glBindTexture(GL_TEXTURE_2D, ID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  // TODO
  // buggy, if the file has 3 channels, should use GL_RGB, I think,
  // failed when trying render with GL_RGBA
  glTexImage2D(GL_TEXTURE_2D, 0, format, widthImg, heightImg, 0, format, pixelType,
               bytes);
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(bytes);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Bind() {
  glActiveTexture(GL_TEXTURE0 + unit);
  glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Unbind() { glBindTexture(GL_TEXTURE_2D, 0); }

void Texture::Delete() { glDeleteTextures(1, &ID); }

void Texture::SetUniformUnit(Shader &shader, const char *uniform, GLuint unit) {
  GLuint texUnit = glGetUniformLocation(shader.ID, uniform);
  shader.Activate();
  glUniform1i(texUnit, unit);
}
