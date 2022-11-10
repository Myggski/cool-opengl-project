#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/GL.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "shader.h"
#include <algorithm>

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

  if (button == GLFW_MOUSE_BUTTON_RIGHT)
  {
    glfwSetInputMode(window, GLFW_CURSOR, action == GLFW_PRESS ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
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
  glfwSwapInterval(0);

  glewInit();

  glfwSetKeyCallback(window, handle_key_event);
  glfwSetMouseButtonCallback(window, handle_mouse_event);
  glfwSetFramebufferSizeCallback(window, handle_framebuffer_resize);

  glEnable(GL_DEPTH_TEST);

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
  GLint u_Model = glGetUniformLocation(program, "u_Model");
  GLint u_ViewProjection = glGetUniformLocation(program, "u_ViewProjection");
  glUniform4f(u_Color, 0.2f, 0.5f, 1.f, 1.f);
  glUniform1i(u_Sampler1, 1);

  GLuint texture0 = load_texture("resources/textures/cool-texture.jpg");
  glBindTexture(GL_TEXTURE_2D, texture0);

  glActiveTexture(GL_TEXTURE1);
  GLuint texture1 = load_texture("resources/textures/cool-texture-2.jpg");
  glBindTexture(GL_TEXTURE_2D, texture1);

  glm::vec3 camera_position{0.f, 0.f, 3.f};
  glm::vec3 camera_forward{0.f, 0.f, -1.f};
  glm::vec3 camera_right{1.f, 0.f, 0.f};
  glm::vec3 camera_rotation = glm::vec3(0.f, 0.f, 0.f);
  float last_time = glfwGetTime();

  double last_mouse_x, last_mouse_y;
  glfwGetCursorPos(window, &last_mouse_x, &last_mouse_y);

  while (!glfwWindowShouldClose(window))
  {
    // Calculate delta time
    float current_time = glfwGetTime();
    float delta_time = current_time - last_time;
    last_time = current_time;

    double mouse_x, mouse_y;
    glfwGetCursorPos(window, &mouse_x, &mouse_y);

    float mouse_delta_x = mouse_x - last_mouse_x;
    float mouse_delta_y = mouse_y - last_mouse_y;
    last_mouse_x = mouse_x;
    last_mouse_y = mouse_y;

    glClearColor(0.12f, 0.12f, 0.12f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUniform1f(u_Time, current_time);

    if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
    {
      glm::vec3 camera_rot = glm::vec3(-mouse_delta_y, -mouse_delta_x, 0.f);
      camera_rotation += camera_rot * glm::radians(0.2f);
    }

    glm::quat camera_quat = glm::angleAxis(camera_rotation.y, glm::vec3(0.f, 1.f, 0.f));
    camera_quat *= glm::angleAxis(camera_rotation.x, glm::vec3(1.f, 0.f, 0.f));
    camera_quat *= glm::angleAxis(camera_rotation.z, glm::vec3(0.f, 0.f, 1.f));
    camera_forward = camera_quat * glm::vec3(0.f, 0.f, -1.f);
    camera_right = camera_quat * glm::vec3(1.f, 0.f, 0.f);

    glm::vec3 camera_movement{0.f};

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
      camera_movement -= camera_right;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
      camera_movement += camera_right;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
      camera_movement += camera_forward;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
      camera_movement -= camera_forward;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
      camera_movement.y += 1.f;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
      camera_movement.y -= 1.f;
    }

    camera_position += camera_movement * 3.f * delta_time;

    int window_width, window_height;
    glfwGetWindowSize(window, &window_width, &window_height);
    float aspect_ratio = window_width / static_cast<float>(window_height);
    glm::mat4 projection_matrix = glm::perspective(glm::radians(50.f), aspect_ratio, 0.5f, 10.f);
    // glm::mat4 orthographic_matrix = glm::ortho(-0.5f * aspect_ratio, 1.5f * aspect_ratio, -1.f, 1.f, 0.f, 10.f);
    glm::mat4 view_matrix = glm::lookAt(camera_position, camera_position + camera_forward, glm::vec3(0.f, 1.f, 0.f));
    glm::mat4 view_projection_matrix = projection_matrix * view_matrix;

    glUniformMatrix4fv(u_ViewProjection, 1, GL_FALSE, glm::value_ptr(view_projection_matrix));

    // Moving vertices
    glm::mat4 translation_matrix = glm::translate(glm::identity<glm::mat4>(), glm::vec3(0.f));
    glm::mat4 scale_matrix = glm::scale(glm::identity<glm::mat4>(), glm::vec3(1.0f));
    glm::mat4 rotation_matrix = glm::rotate(glm::identity<glm::mat4>(), glm::radians(current_time * 90.f * 0.f), glm::vec3(0.f, 1.f, 0.f));

    glm::mat4 model_matrix = translation_matrix * rotation_matrix * scale_matrix;

    glUniformMatrix4fv(u_Model, 1, GL_FALSE, glm::value_ptr(model_matrix));

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  return 0;
}