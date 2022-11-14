#include "mesh.h"
#include <GL/glew.h>

mesh::mesh(wavefront_file const &obj)
{
  vertices.reserve(obj.triangles.size() * 3);

  for (auto &triangle : obj.triangles)
  {
    for (auto &vert : triangle.vertices)
    {
      indices.push_back(vert.idx_position);
      vertices.emplace_back(
          obj.positions[vert.idx_position],
          obj.texcoords[vert.idx_texcoord],
          obj.normals[vert.idx_normal]);
    }
  }

  vertex_buffer_layout layout;
  layout.push(3); // 3D Position
  layout.push(2); // Texture (TexCoord)
  layout.push(3); // Normal
  vao = vertex_array(vertices, layout);
}
