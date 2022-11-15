#version 330 core

// Input
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;

// Uniform
uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

out vec3 f_Normal;
out vec3 f_World;

void main()
{
  gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
  f_World = (u_Model * vec4(a_Position, 1.0)).xyz;
  f_Normal = normalize((u_Model * vec4(a_Normal, 0.0)).xyz);
}