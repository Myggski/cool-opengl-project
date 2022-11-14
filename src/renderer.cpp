#include "renderer.h"
#include <GL/glew.h>
#include "glm/gtc/type_ptr.hpp"

void renderer::begin_scene(camera &game_camera, unsigned int const &program)
{
  glUseProgram(program);
  GLint const u_ViewProjection = glGetUniformLocation(program, "u_ViewProjection");
  glUniformMatrix4fv(u_ViewProjection, 1, GL_FALSE, glm::value_ptr(game_camera.get_view_projection()));
}

void renderer::draw(vertex_array const &vao, bool const draw_arrays)
{
  vao.bind();
  if (draw_arrays)
  {
    glDrawArrays(GL_TRIANGLES, 0, vao.Count());
  }
  else
  {
    glDrawElements(GL_TRIANGLES, vao.Count(), GL_UNSIGNED_INT, nullptr);
  }
}

void renderer::draw(mesh const &mesh)
{
  renderer::draw(mesh.vao, true);
}

void renderer::draw(mesh const &mesh, glm::mat4 const &model, unsigned int const &program)
{
  GLint const u_Model = glGetUniformLocation(program, "u_Model");
  glUniformMatrix4fv(u_Model, 1, GL_FALSE, glm::value_ptr(model));
  renderer::draw(mesh.vao, true);
}