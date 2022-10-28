#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/GL.h>
#include "shader.h"

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

  GLuint buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);

  float vertex_data[] = {
      -0.5f,
      -0.5f,
      0.5f,
      -0.5f,
      0.5f,
      0.5f,
      -0.5f,
      0.5f,
  };

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

  GLuint ebo;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

  GLuint element_data[] = {
      0, 1, 2, 0, 2, 3};

  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(element_data), element_data, GL_STATIC_DRAW);

  GLuint vertex_array_object;
  glGenVertexArrays(1, &vertex_array_object);
  glBindVertexArray(vertex_array_object);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

  // 2D vector (Position)
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

  GLuint program = load_shader_program("resources/shaders/triangle.vert", "resources/shaders/triangle.frag");
  glUseProgram(program);

  while (!glfwWindowShouldClose(window))
  {
    glClearColor(0.12f, 0.12f, 0.12f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    // glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  return 0;
}