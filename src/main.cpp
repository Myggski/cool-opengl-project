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

  // Vertex buffer object
  GLuint data_buffer;
  glGenBuffers(1, &data_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, data_buffer);

  float vertex_data[] = {
      -0.5f, -0.5f, 1.f, 0.f, 0.f,
      0.5f, -0.5f, 0.f, 1.f, 0.f,
      0.5f, 0.5f, 0.f, 0.f, 1.f,
      -0.5f, 0.5f, 1.f, 1.f, 1.f};

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

  // Element buffer object
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

  glBindBuffer(GL_ARRAY_BUFFER, data_buffer);

  // 2D vector (Position)
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);

  // RGB (Color)
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(2 * sizeof(float)));

  // Load shader
  GLuint program = load_shader_program("resources/shaders/triangle.vert", "resources/shaders/cool.frag");
  glUseProgram(program);

  GLint u_Color = glGetUniformLocation(program, "u_Color");
  GLint u_Time = glGetUniformLocation(program, "u_Time");
  glUniform4f(u_Color, 0.2f, 0.5f, 1.f, 1.f);

  while (!glfwWindowShouldClose(window))
  {
    float Time = glfwGetTime();
    glUniform1f(u_Time, Time);

    glClearColor(0.12f, 0.12f, 0.12f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    // glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  return 0;
}