#pragma once

#include "vertex_buffer.h"

class vertex_array
{
private:
  unsigned int renderer_id;

public:
  vertex_array();
  ~vertex_array();

  void bind() const;
  void unbind() const;
};