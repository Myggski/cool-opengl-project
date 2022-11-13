#pragma once

#include <string>

struct window_props
{
  std::string title;
  uint16_t width;
  uint16_t height;

  window_props(const std::string &title = "OpenGL Window", uint16_t width = 1280, uint16_t height = 720);
};