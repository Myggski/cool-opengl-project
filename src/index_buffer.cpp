#include "index_buffer.h"

index_buffer::index_buffer(const unsigned int *data, unsigned int count) : count(count)
{
  renderer_id = 0;
  glGenBuffers(1, &renderer_id);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}
index_buffer::~index_buffer()
{
  glDeleteBuffers(1, &renderer_id);
}
void index_buffer::bind()
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id);
}
void index_buffer::unbind()
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}