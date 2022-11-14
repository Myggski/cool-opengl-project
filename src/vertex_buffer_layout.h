
#pragma once

#include <vector>
#include <GL/glew.h>

struct vertex_buffer_layout_element
{
public:
  vertex_buffer_layout_element(unsigned int type, unsigned int count, unsigned char normalized)
      : type(type), count(count), normalized(normalized) {}

  static unsigned int get_size_of_type(unsigned int type)
  {
    switch (type)
    {
    case GL_FLOAT:
      return sizeof(float);
    case GL_UNSIGNED_INT:
      return sizeof(int);
    case GL_UNSIGNED_BYTE:
      return sizeof(char);
    }
    return 0;
  }

  unsigned int type = 0;
  unsigned int count = 0;
  unsigned char normalized = 0;
};

class vertex_buffer_layout
{
public:
  vertex_buffer_layout() {}

  inline void push(unsigned int count)
  {
    elements.push_back({GL_FLOAT, count, GL_FALSE});
    stride += vertex_buffer_layout_element::get_size_of_type(GL_FLOAT) * count;
  }
  inline const std::vector<vertex_buffer_layout_element> get_elements() const { return elements; }
  inline const unsigned int get_stride() const { return stride; }

private:
  std::vector<vertex_buffer_layout_element> elements;
  unsigned int stride = 0;
};