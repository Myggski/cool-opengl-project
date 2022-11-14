#pragma once

#include <vector>
#include "vertex_array.h"
#include "wavefront.h"

class mesh
{
  friend class renderer;

public:
  mesh(wavefront_file const &obj);

private:
  std::vector<vertex> vertices;
  std::vector<int> indices;
  vertex_array vao;
};