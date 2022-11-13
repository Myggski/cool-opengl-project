#include "vertex_array.h"
#include <GL/glew.h>

vertex_array::vertex_array()
{
  renderer_id = 0;
  glGenVertexArrays(1, &renderer_id);
  glBindVertexArray(renderer_id);
}

vertex_array::~vertex_array()
{
  glDeleteVertexArrays(1, &renderer_id);
}

void vertex_array::bind() const
{
  glBindVertexArray(renderer_id);
}

void vertex_array::unbind() const
{
  glBindVertexArray(0);
}