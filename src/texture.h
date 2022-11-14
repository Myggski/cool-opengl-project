#pragma once

#include <string>

class texture
{
public:
  texture(const std::string &path, unsigned int slot = 0);
  ~texture();

  void bind(unsigned int slot = 0) const;
  void unbind() const;

  inline int get_width() const { return width; }
  inline int get_heigth() const { return height; }

private:
  unsigned int renderer_id;
  std::string file_path;
  unsigned char *local_buffer;
  int width, height, bpp;
};