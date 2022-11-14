#pragma once

#include "vertex_array.h"
#include "camera.h"
#include "mesh.h"

class renderer
{
public:
  static void begin_scene(camera &game_camera, unsigned int const &program);

  static void draw(vertex_array const &vao, bool const draw_arrays = false);
  static void draw(mesh const &mesh_obj);
  static void draw(mesh const &mesh_obj, glm::mat4 const &model, unsigned int const &program);
};