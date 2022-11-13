#pragma once

#include <string>
#include <GLFW/glfw3.h>
#include "window_data.h"
#include "window_props.h"

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int modifiers);
static void window_resize_callback(GLFWwindow *window, int width, int height);
static void mouse_button_callback(GLFWwindow *window, int button, int action, int modifiers);
static void mouse_position_callback(GLFWwindow *window, double position_x, double position_y);

class window
{
public:
  window(const window_props &props);
  ~window();

  void on_update();
  void set_v_sync(bool enabled);
  bool is_v_sync() const;
  bool is_key_pressed(uint16_t key_code);
  bool is_key_released(uint16_t key_code);
  inline bool is_window_open() const { return !glfwWindowShouldClose(glfw_window); }
  inline uint32_t get_width() const { return data.width; }
  inline uint32_t get_height() const { return data.height; }
  float get_aspect_ratio();
  static window* create(const window_props& props = window_props());

private:
  void init(const window_props &props);
  void shut_down();

private:
  GLFWwindow *glfw_window;
  window_data data;
};