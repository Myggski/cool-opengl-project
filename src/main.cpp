#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/GL.h>

void handle_key_event(GLFWwindow *window, int key, int scancode, int action, int modifiers)
{
  if (action != GLFW_PRESS)
    return;

  if (key == GLFW_KEY_ESCAPE)
  {
    glfwDestroyWindow(window);
  }
}

void handle_mouse_event(GLFWwindow *window, int button, int action, int modifiers)
{
  if (button == GLFW_MOUSE_BUTTON_LEFT)
  {
    printf("MOUSE PRESSED\n");
  }
}

int main()
{
  glfwInit();
  GLFWwindow *window = glfwCreateWindow(800, 600, "Cool Window", NULL, NULL);
  glfwMakeContextCurrent(window);

  glewInit();

  glfwSetKeyCallback(window, handle_key_event);
  glfwSetMouseButtonCallback(window, handle_mouse_event);

  glGenBuffers(1, NULL);

  while (!glfwWindowShouldClose(window))
  {
    glClearColor(0.1f, 0.1f, 0.8f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);

    glfwPollEvents();
  }

  return 0;
}