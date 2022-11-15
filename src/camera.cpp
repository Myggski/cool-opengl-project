
#include "camera.h"
#include <algorithm>
camera::camera(float aspect_ratio, float fov_y) : position(glm::vec3(0.0F)),
                                                  forward(0.0F, 0.0F, -1.0F),
                                                  fov(fov_y),
                                                  current_fov(fov)
{
  set_aspect_ratio(aspect_ratio);
  update_matrix();
}

void camera::rotate(glm::vec2 mouse_delta)
{
  if (mouse_delta.length() < 0.1f)
  {
    return;
  }

  glm::vec3 new_forward(0.0F);
  this->yaw += mouse_delta.x;
  this->pitch += mouse_delta.y;

  if (this->pitch > 89.0f)
    this->pitch = 89.0f;
  if (this->pitch < -89.0f)
    this->pitch = -89.0f;

  new_forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  new_forward.y = sin(glm::radians(pitch));
  new_forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  this->forward = glm::normalize(new_forward);
}

void camera::zoom(float delta)
{
  this->current_fov -= delta;
  this->current_fov = std::min(this->current_fov, this->fov);
  this->current_fov = std::max(this->current_fov, MIN_FOV);
}

void camera::move(glm::vec3 local_move)
{
  auto const right_vector = glm::normalize(glm::cross(glm::vec3(0.0F, 1.0F, 0.0F), forward));

  this->position += forward * local_move.z;
  this->position += right_vector * local_move.x;
  this->position.y += local_move.y;
}

void camera::set_aspect_ratio(float aspect_ratio)
{
  this->aspect = aspect_ratio;
}

void camera::set_position(glm::vec3 const &position)
{
  this->position = position;
}

glm::mat4 const &camera::get_view_projection()
{
  this->update_matrix();
  return this->view_projection;
}

void camera::update_matrix()
{
  auto const projection = glm::perspective(glm::radians(this->current_fov), this->aspect, 0.1F, 100.F);
  auto const view = glm::lookAt(this->position, this->position + this->forward, glm::vec3(0.0F, 1.0F, 0.0F));
  this->view_projection = projection * view;
}