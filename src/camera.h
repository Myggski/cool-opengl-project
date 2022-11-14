#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

class camera
{
public:
  camera(float aspect_ratio, float fov_y);

  void rotate(glm::vec2 mouse_delta);

  void zoom(float delta);

  void move(glm::vec3 local_move);

  void set_aspect_ratio(float aspect_ratio);

  void set_position(glm::vec3 const &position);

  glm::mat4 const &get_view_projection();

  inline glm::vec3 const &get_position() const { return position; }

private:
  void update_matrix();

private:
  glm::mat4 view_projection;
  glm::vec3 position;
  glm::vec3 forward;

  float fov;
  float current_fov;
  float aspect;
  float yaw = -90.0F;
  float pitch = 0.0f;

  static constexpr float MIN_FOV = 10.0F;
};