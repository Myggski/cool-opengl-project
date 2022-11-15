#include "vertex_array.h"
#include <GL/glew.h>

vertex_array::vertex_array()
{
  glGenVertexArrays(1, &renderer_id);
  glBindVertexArray(renderer_id);
}

vertex_array::vertex_array(std::vector<vertex> const &vertex_data,
                           vertex_buffer_layout const &layout) : count(vertex_data.size())
{
  glGenVertexArrays(1, &renderer_id);
  glBindVertexArray(renderer_id);

  unsigned int vbo{};
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * vertex_data.size(), vertex_data.data(), GL_STATIC_DRAW);

  auto const &elements = layout.get_elements();
  std::ptrdiff_t offset = 0;
  for (auto i = 0; i < elements.size(); ++i)
  {
    auto const &element = elements[i];
    glEnableVertexAttribArray(i);
    glVertexAttribPointer(i, element.count, element.type,
                          element.normalized, layout.get_stride(), (const void *)offset);

    offset += element.count * vertex_buffer_layout_element::get_size_of_type(element.type);
  }

  glBindVertexArray(renderer_id);
}

vertex_array::~vertex_array()
{
  glDeleteVertexArrays(1, &renderer_id);
}

void vertex_array::add_buffer(vertex_buffer &vbo, const vertex_buffer_layout &layout)
{
  bind();
  vbo.bind();

  const auto &elements = layout.get_elements();
  std::ptrdiff_t offset = 0;
  count = 6;
  for (unsigned int i = 0; i < elements.size(); i++)
  {
    const auto &element = elements[i];
    int stride = layout.get_stride();
    glEnableVertexAttribArray(i);
    glVertexAttribPointer(i, element.count, element.type, GL_FALSE,
                          stride, (const void *)offset);
    offset += element.count * vertex_buffer_layout_element::get_size_of_type(element.type);
  }
}

void vertex_array::bind() const
{
  glBindVertexArray(renderer_id);
}

void vertex_array::unbind() const
{
  glBindVertexArray(0);
}