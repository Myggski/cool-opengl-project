#include "window.h"
#include <cstdio>

static bool _GLWF_initialized{false};

window *window::create(const window_props &props)
{
  return new window(props);
}

window::window(const window_props &props) { init(props); }

window::~window() { shut_down(); }

void window::init(const window_props &props)
{
  data.title = props.title;
  data.width = props.width;
  data.height = props.height;

  if (!_GLWF_initialized)
  {
    glfwInit();
    _GLWF_initialized = true;
  }

  glfw_window = glfwCreateWindow(
      static_cast<int>(props.width),
      static_cast<int>(props.height),
      data.title.c_str(),
      NULL,
      NULL);
  glfwMakeContextCurrent(glfw_window);
  glfwSetWindowUserPointer(glfw_window, &data);
  set_v_sync(true);

  glfwSetKeyCallback(glfw_window, key_callback);
  glfwSetMouseButtonCallback(glfw_window, mouse_button_callback);
  glfwSetFramebufferSizeCallback(glfw_window, window_resize_callback);
  glfwSetCursorPosCallback(glfw_window, mouse_position_callback);
  glEnable(GL_DEPTH_TEST);
  glewInit();
}

void window::shut_down()
{
  glfwDestroyWindow(glfw_window);
}

void window::on_update()
{
  glfwPollEvents();
  glfwSwapBuffers(glfw_window);
}

void window::set_v_sync(bool enabled)
{
  glfwSwapInterval(static_cast<int>(enabled));
  data.v_sync = enabled;
}

bool window::is_v_sync() const
{
  return data.v_sync;
}

bool window::is_key_pressed(unsigned int key_code)
{
  return glfwGetKey(glfw_window, key_code) == GLFW_PRESS;
}

bool window::is_key_released(unsigned int key_code)
{
  return glfwGetKey(glfw_window, key_code) == GLFW_RELEASE;
}

float window::get_aspect_ratio()
{
  int window_width, window_height;
  glfwGetWindowSize(glfw_window, &window_width, &window_height);
  return window_width / static_cast<float>(window_height);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int modifiers)
{
  if (action != GLFW_PRESS)
    return;

  if (key == GLFW_KEY_ESCAPE)
  {
    glfwDestroyWindow(window);
  }
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int modifiers)
{
  if (button == GLFW_MOUSE_BUTTON_LEFT)
  {
    printf("MOUSE PRESSED\n");
  }

  if (button == GLFW_MOUSE_BUTTON_RIGHT)
  {
    glfwSetInputMode(window, GLFW_CURSOR, action == GLFW_PRESS ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
  }
}

void window_resize_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void mouse_position_callback(GLFWwindow *window, double position_x, double position_y)
{
  window_data *glfw_data = (window_data *)glfwGetWindowUserPointer(window);
  glfw_data->mouse_position.x = position_x;
  glfw_data->mouse_position.y = position_y;

  glfw_data->delta_mouse_position.x = position_x - glfw_data->last_mouse_position.x;
  glfw_data->delta_mouse_position.y = glfw_data->last_mouse_position.y - position_y;

  glfw_data->last_mouse_position.x = position_x;
  glfw_data->last_mouse_position.y = position_y;
}