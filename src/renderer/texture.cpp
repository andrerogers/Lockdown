/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Andre Rogers $
   $Notice: (C) Copyright 2020 by Dre Codes, Inc. All Rights Reserved. $
   ======================================================================== */
#include "texture.h"

Texture::Texture(const char *image, GLenum texType, GLuint slot,
                 GLenum pixelType) {
  type = texType;

  // Texture
  int widthImg, heightImg, numColCh;

  stbi_set_flip_vertically_on_load(true);

  unsigned char *bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);

  if (numColCh == 4)
    format = GL_RGBA;
  else
    format = GL_RGB;

  // Generate a texture unit
  glGenTextures(1, &ID);

  // Assign the texture to a slot
  // in the texture unit
  glActiveTexture(GL_TEXTURE0 + slot);
  unit = slot;

  // Bind the texture with its appropriate
  // type, here 2D texture
  glBindTexture(texType, ID);

  glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

  // TODO
  // buggy, if the file has 3 channels, should use GL_RGB, I think,
  // failed when trying render with GL_RGBA
  glTexImage2D(texType, 0, format, widthImg, heightImg, 0, format, pixelType,
               bytes);
  glGenerateMipmap(texType);

  stbi_image_free(bytes);
  glBindTexture(texType, 0);
}

void Texture::Bind() {
  glActiveTexture(GL_TEXTURE0 + unit);
  glBindTexture(type, ID);
}

void Texture::Unbind() { glBindTexture(type, 0); }

void Texture::Delete() { glDeleteTextures(1, &ID); }

void Texture::SetUniformUnit(Shader &shader, const char *uniform, GLuint unit) {
  GLuint texUnit = glGetUniformLocation(shader.ID, uniform);
  shader.Activate();
  glUniform1i(texUnit, unit);
}
