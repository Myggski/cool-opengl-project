#pragma once

#include <string>

struct window_data
{
  window_data();

  bool v_sync{false};
  unsigned int width{0};
  unsigned int height{0};
  double mouse_x{0.f};
  double mouse_y{0.f};
  float last_mouse_x{0.f};
  float last_mouse_y{0.f};
  float delta_mouse_x{0.f};
  float delta_mouse_y{0.f};
  std::string title{""};
};