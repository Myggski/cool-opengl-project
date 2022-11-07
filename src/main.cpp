#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/GL.h>
#include "shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

GLuint load_texture(const char *path)
{
  int width, height;
  int channels;
  stbi_uc *texture_data = stbi_load(path, &width, &height, &channels, 0);

  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  return texture;
}

void handle_framebuffer_resize(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

int main()
{
  glfwInit();
  GLFWwindow *window = glfwCreateWindow(800, 600, "Cool Window", NULL, NULL);
  glfwMakeContextCurrent(window);

  glewInit();

  glfwSetKeyCallback(window, handle_key_event);
  glfwSetMouseButtonCallback(window, handle_mouse_event);
  glfwSetFramebufferSizeCallback(window, handle_framebuffer_resize);

  // Vertex buffer object
  GLuint data_buffer;
  glGenBuffers(1, &data_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, data_buffer);

  float vertex_data[] = {
      -0.5f, -0.5f, 1.f, 0.f, 0.f, 0.f, 1.f,
      0.5f, -0.5f, 0.f, 1.f, 0.f, 1.f, 1.f,
      0.5f, 0.5f, 0.f, 0.f, 1.f, 1.f, 0.f,
      -0.5f, 0.5f, 1.f, 1.f, 1.f, 0.f, 0.f};

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
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), 0);
  // RGB (Color)
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *)(2 * sizeof(float)));
  // Texture (TexCoord)
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *)(5 * sizeof(float)));

  // Load shader
  GLuint program = load_shader_program("resources/shaders/triangle.vert", "resources/shaders/cool.frag");
  glUseProgram(program);

  GLint u_Color = glGetUniformLocation(program, "u_Color");
  GLint u_Time = glGetUniformLocation(program, "u_Time");
  GLint u_Sampler1 = glGetUniformLocation(program, "u_Sampler1");
  glUniform4f(u_Color, 0.2f, 0.5f, 1.f, 1.f);
  glUniform1i(u_Sampler1, 1);

  GLuint texture0 = load_texture("resources/textures/cool-texture.jpg");
  glBindTexture(GL_TEXTURE_2D, texture0);
    
  glActiveTexture(GL_TEXTURE1);
  GLuint texture1 = load_texture("resources/textures/cool-texture-2.jpg");
  glBindTexture(GL_TEXTURE_2D, texture1);

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