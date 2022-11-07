#version 330 core
uniform float u_Time;
layout(location = 0) in vec2 a_Position; // 0
layout(location = 1) in vec3 a_Color; // 1
layout(location = 2) in vec2 a_TexCoord; // 2

// Sending this to the fragment shader
out vec2 f_Position;
out vec3 f_Color; 
out vec2 f_TexCoord;

void main()
{
  vec2 pos;
  pos.x = a_Position.x * cos(u_Time) - a_Position.y * sin(u_Time);
  pos.y = a_Position.y * cos(u_Time) + a_Position.x * sin(u_Time);

  gl_Position = vec4(a_Position, 0.0, 1.0);
  f_Color = a_Color;
  f_Position = a_Position;
  f_TexCoord = a_TexCoord;
}
