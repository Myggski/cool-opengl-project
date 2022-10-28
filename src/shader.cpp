#include "shader.h"
#include <stdio.h>
#include "string"
#include <fstream>
#include <sstream>

static std::string read_shader_file(const char *shader_file)
{
  std::ifstream file(shader_file);
  if (!file)
    return std::string();

  file.ignore(std::numeric_limits<std::streamsize>::max());
  auto size = file.gcount();

  if (size > 0x10000)
    return std::string();

  file.clear();
  file.seekg(0, std::ios_base::beg);

  std::stringstream sstr;
  sstr << file.rdbuf();
  file.close();

  return sstr.str();
}

GLuint load_shader(const char *path, GLenum type)
{
  std::string shader_source = read_shader_file(path);

  printf(shader_source.c_str());

  const char *shader_source_ptr = shader_source.c_str();
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &shader_source_ptr, NULL);
  glCompileShader(shader);

  return shader;
}

GLuint load_shader_program(const char *vert_path, const char *frag_path)
{
  GLuint vertex_shader = load_shader(vert_path, GL_VERTEX_SHADER);
  GLuint fragment_shader = load_shader(frag_path, GL_FRAGMENT_SHADER);
  GLuint program = glCreateProgram();

  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);

  return program;
}