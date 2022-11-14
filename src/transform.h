#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtc/type_ptr.hpp"

struct transform
{
  transform(glm::vec3 const &position) : position(position){};

  glm::mat4 const &mat4()
  {
    matrix = glm::mat4(1.0F);
    matrix = glm::translate(matrix, position);
    matrix = glm::rotate(matrix, glm::radians(rotation.x), glm::vec3(1.0F, 0.0F, 0.0F));
    matrix = glm::rotate(matrix, glm::radians(rotation.y), glm::vec3(0.0F, 1.0F, 0.0F));
    matrix = glm::rotate(matrix, glm::radians(rotation.z), glm::vec3(0.0F, 0.0F, 1.0F));
    matrix = glm::scale(matrix, scale);

    return matrix;
  };

  inline void set_position(glm::vec3 const &position)
  {
    this->position = position;
  }

  inline void set_rotation(glm::vec3 const &rotation)
  {
    this->rotation = rotation;
  }

  inline void set_scale(glm::vec3 const &scale)
  {
    this->scale = scale;
  }

  inline glm::vec3 const &get_position()
  {
    return this->position;
  }

  inline glm::vec3 const &get_rotation()
  {
    return this->rotation;
  }

  inline glm::vec3 const &get_scale()
  {
    return this->scale;
  }

private:
  glm::vec3 position{0.f};
  glm::vec3 rotation{0.f};
  glm::vec3 scale{1.f};
  glm::mat4 matrix{1.f};
};