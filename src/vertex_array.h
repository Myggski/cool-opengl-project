#pragma once

#include "vertex_buffer.h"
#include "vertex_buffer_layout.h"
#include "glm/glm.hpp"

struct vertex
{
  vertex(glm::vec3 position, glm::vec2 tex_coord, glm::vec3 normal)
      : position(position), tex_coord(tex_coord), normal(normal) {}
  glm::vec3 position{};
  glm::vec2 tex_coord{};
  glm::vec3 normal{};
};

class vertex_array
{
private:
  unsigned int renderer_id = 0;
  unsigned int count = 0;

public:
  vertex_array();
  vertex_array(std::vector<vertex> const &vertex_data,
               vertex_buffer_layout const &layout);
  ~vertex_array();

  vertex_array &operator=(vertex_array &&rhs) noexcept
  {
    std::swap(renderer_id, rhs.renderer_id);
    count = rhs.count;
    return *this;
  }

  void add_buffer(vertex_buffer &vbo, const vertex_buffer_layout &layout);
  void bind() const;
  void unbind() const;
  inline int Count() const { return count; }
};