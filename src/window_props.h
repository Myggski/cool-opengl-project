#pragma once

#include <string>

struct window_props
{
  std::string title;
  unsigned int width;
  unsigned int height;

  window_props(const std::string &title = "OpenGL Window", unsigned int width = 1280, unsigned int height = 720);
};