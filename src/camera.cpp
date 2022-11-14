
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
  glm::vec3 forward(0.0F);
  yaw += mouse_delta.x;
  pitch -= mouse_delta.y;

  if (pitch > 89.0f)
    pitch = 89.0f;
  if (pitch < -89.0f)
    pitch = -89.0f;

  forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  forward.y = sin(glm::radians(pitch));
  forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  forward = glm::normalize(forward);
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