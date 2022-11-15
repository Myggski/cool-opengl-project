#pragma once

#include <string>
#include "glm/glm.hpp"

struct window_data
{
  window_data();

  bool v_sync{false};
  unsigned int width{0};
  unsigned int height{0};
  glm::vec2 mouse_position{};
  glm::vec2 delta_mouse_position{};
  glm::vec2 last_mouse_position{};
  std::string title{""};
};