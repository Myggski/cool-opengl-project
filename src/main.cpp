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
#include "vertex_array.h"
#include "vertex_buffer.h"
#include "window.h"
#include "window_props.h"
#include <algorithm>
#include <memory>

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

int main()
{
  float vertex_data[] = {
      -0.5f, -0.5f, 1.f, 0.f, 0.f, 0.f, 1.f,
      0.5f, -0.5f, 0.f, 1.f, 0.f, 1.f, 1.f,
      0.5f, 0.5f, 0.f, 0.f, 1.f, 1.f, 0.f,
      -0.5f, 0.5f, 1.f, 1.f, 1.f, 0.f, 0.f};

  window* win = window::create();
  glewInit();
  vertex_buffer *vbo = new vertex_buffer(vertex_data, sizeof(vertex_data));

  // Element buffer object
  GLuint ebo;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

  GLuint element_data[] = {
      0, 1, 2, 0, 2, 3};

  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(element_data), element_data, GL_STATIC_DRAW);

  vertex_array *vao = new vertex_array();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  vbo->bind();

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

  while (win->is_window_open())
  {
    // Calculate delta time
    float current_time = glfwGetTime();
    float delta_time = current_time - last_time;
    last_time = current_time;

    glClearColor(0.12f, 0.12f, 0.12f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUniform1f(u_Time, current_time);

    // if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
    //{
    //   glm::vec3 camera_rot = glm::vec3(-mouse_delta_y, -mouse_delta_x, 0.f);
    //   camera_rotation += camera_rot * glm::radians(0.2f);
    // }

    glm::quat camera_quat = glm::angleAxis(camera_rotation.y, glm::vec3(0.f, 1.f, 0.f));
    camera_quat *= glm::angleAxis(camera_rotation.x, glm::vec3(1.f, 0.f, 0.f));
    camera_quat *= glm::angleAxis(camera_rotation.z, glm::vec3(0.f, 0.f, 1.f));
    camera_forward = camera_quat * glm::vec3(0.f, 0.f, -1.f);
    camera_right = camera_quat * glm::vec3(1.f, 0.f, 0.f);

    glm::vec3 camera_movement{0.f};

    if (win->is_key_pressed(GLFW_KEY_A))
    {
      camera_movement -= camera_right;
    }

    if (win->is_key_pressed(GLFW_KEY_D))
    {
      camera_movement += camera_right;
    }

    if (win->is_key_pressed(GLFW_KEY_W))
    {
      camera_movement += camera_forward;
    }

    if (win->is_key_pressed(GLFW_KEY_S))
    {
      camera_movement -= camera_forward;
    }

    if (win->is_key_pressed(GLFW_KEY_SPACE))
    {
      camera_movement.y += 1.f;
    }

    if (win->is_key_pressed(GLFW_KEY_LEFT_CONTROL))
    {
      camera_movement.y -= 1.f;
    }

    camera_position += camera_movement * 3.f * delta_time;

    float aspect_ratio = win->get_aspect_ratio();
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

    win->on_update();
  }

  return EXIT_SUCCESS;
}