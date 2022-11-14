#include "texture.h"
#include "GL/glew.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

texture::texture(const std::string &path, unsigned int slot)
    : file_path(path), local_buffer(nullptr), width(0), height(0), bpp(0)
{
  local_buffer = stbi_load(path.c_str(), &width, &height, &bpp, 4);

  glGenTextures(1, &renderer_id);
  bind(slot);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, local_buffer);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  if (local_buffer)
  {
    stbi_image_free(local_buffer);
  }
}

texture::~texture()
{
  glDeleteTextures(1, &renderer_id);
}

void texture::bind(unsigned int slot) const
{
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, renderer_id);
}

void texture::unbind() const
{
  glBindTexture(GL_TEXTURE_2D, 0);
}