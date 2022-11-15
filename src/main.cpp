#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/GL.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "camera.h"
#include "shader.h"
#include "renderer.h"
#include "texture.h"
#include "index_buffer.h"
#include "vertex_array.h"
#include "vertex_buffer.h"
#include "vertex_buffer_layout.h"
#include "window.h"
#include "window_props.h"
#include "mesh.h"
#include <algorithm>
#include <memory>
#include <cstdio>

int main()
{
  float vertex_data[] = {
      -0.5f, -0.5f, -1.f, 0.f, 1.f, 1.f, 0.f, 0.f,
      0.5f, -0.5f, -1.f, 1.f, 1.f, 0.f, 1.f, 0.f,
      0.5f, 0.5f, -1.f, 1.f, 0.f, 0.f, 0.f, 1.f,
      -0.5f, 0.5f, -1.f, 0.f, 0.f, 1.f, 1.f, 1.f};

  std::unique_ptr<window> active_window = std::unique_ptr<window>(window::create());
  camera game_camera{active_window->get_aspect_ratio(), 90.f};

  std::unique_ptr<vertex_array> vao = std::make_unique<vertex_array>();
  std::unique_ptr<vertex_buffer> vbo = std::make_unique<vertex_buffer>(vertex_data, sizeof(vertex_data));
  vertex_buffer_layout layout;
  layout.push(3); // 3D Vector
  layout.push(2); // Texture (TexCoord)
  layout.push(3); // Normal
  vao->add_buffer(*vbo, layout);

  GLuint plane_indices[6] = {0, 1, 2, 0, 2, 3};
  std::unique_ptr<index_buffer> ibo = std::make_unique<index_buffer>(plane_indices, std::size(plane_indices));

  auto obj_file = wavefront_load("resources/meshes/sphere.obj");
  std::unique_ptr<mesh> cube = std::make_unique<mesh>(*obj_file);

    auto obj_file2 = wavefront_load("resources/meshes/monkey.obj");
  std::unique_ptr<mesh> monkey = std::make_unique<mesh>(*obj_file2);

  // Load shader
  // GLuint program = load_shader_program("resources/shaders/triangle.vert", "resources/shaders/triangle.frag");
  GLuint program = load_shader_program("resources/shaders/light.vert", "resources/shaders/light.frag");

  std::unique_ptr<texture> texture0 = std::make_unique<texture>("resources/textures/cool-texture.jpg");
  std::unique_ptr<texture> texture1 = std::make_unique<texture>("resources/textures/cool-texture-2.jpg", 1);

  float last_time = glfwGetTime();
  glm::mat4 model = glm::identity<glm::mat4>();

  while (active_window->is_window_open())
  {
    // Calculate delta time
    float current_time = glfwGetTime();
    float delta_time = current_time - last_time;
    last_time = current_time;

    glClearColor(0.12f, 0.12f, 0.12f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
    //{
    //   glm::vec3 camera_rot = glm::vec3(-mouse_delta_y, -mouse_delta_x, 0.f);
    //   camera_rotation += camera_rot * glm::radians(0.2f);
    // }

    glm::vec3 camera_movement{0.f};

    if (active_window->is_key_pressed(GLFW_KEY_A))
    {
      camera_movement.x = 1.f;
    }

    if (active_window->is_key_pressed(GLFW_KEY_D))
    {
      camera_movement.x = -1.f;
    }

    if (active_window->is_key_pressed(GLFW_KEY_W))
    {
      camera_movement.z = 1.f;
    }

    if (active_window->is_key_pressed(GLFW_KEY_S))
    {
      camera_movement.z = -1.f;
    }

    if (active_window->is_key_pressed(GLFW_KEY_SPACE))
    {
      camera_movement.y = 1.f;
    }

    if (active_window->is_key_pressed(GLFW_KEY_LEFT_CONTROL))
    {
      camera_movement.y = -1.f;
    }

    game_camera.move(camera_movement * 3.f * delta_time);
    if (glfwGetInputMode(active_window->get_window(), GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
    {
      game_camera.rotate(active_window->get_mouse_delta() * 0.07f);
    }
    // glUniformMatrix4fv(u_ViewProjection, 1, GL_FALSE, glm::value_ptr(game_camera.get_view_projection()));

    // Moving vertices

    renderer::begin_scene(game_camera, program);
    GLint u_Color = glGetUniformLocation(program, "u_Color");
    GLint u_Time = glGetUniformLocation(program, "u_Time");
    GLint u_Model = glGetUniformLocation(program, "u_Model");
    GLint u_DirectionalLight = glGetUniformLocation(program, "u_DirectionalLight");
    GLint u_EyePosition = glGetUniformLocation(program, "u_EyePosition");
    glUniform1f(u_Time, current_time);
    glUniform4f(u_Color, 0.5f, 0.5f, 1.f, 1.f);
    glm::vec3 directional_light(-1.f, -1.f, -1.f);
    glUniform3fv(u_DirectionalLight, 1, glm::value_ptr(directional_light));
    glUniform3fv(u_EyePosition, 1, glm::value_ptr(game_camera.get_position()));

    glm::mat4 translation_matrix = glm::translate(glm::identity<glm::mat4>(), glm::vec3(0.f, 0.f, -10.f));
    glm::mat4 scale_matrix = glm::scale(glm::identity<glm::mat4>(), glm::vec3(1.0f));
    glm::mat4 rotation_matrix = glm::rotate(glm::identity<glm::mat4>(), glm::radians(current_time * 90.f * 0.f), glm::vec3(0.f, 1.f, 0.f));

    glm::mat4 model_matrix = translation_matrix * rotation_matrix * scale_matrix;

    glUniformMatrix4fv(u_Model, 1, GL_FALSE, glm::value_ptr(model_matrix));
    renderer::draw(*vao);

    model = glm::identity<glm::mat4>() * glm::rotate(glm::mat4(1.f), current_time, glm::vec3(0.f, 1.f, 0.f));

    renderer::draw(*cube, model, program);
    model = glm::translate(glm::identity<glm::mat4>(), glm::vec3(0.f, 0.f, -5.f)) * model;
    renderer::draw(*monkey, model, program);

    active_window->on_update();
  }

  return EXIT_SUCCESS;
}