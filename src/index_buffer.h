#pragma once

#include <GL/glew.h>

class index_buffer
{
public:
  index_buffer(const unsigned int *data, unsigned int count);
  ~index_buffer();

  void bind();
  void unbind();

  inline unsigned int get_count() const { return count; }

private:
  unsigned int renderer_id;
  unsigned int count;
};