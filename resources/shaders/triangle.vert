#version 330 core
uniform float u_Time;
in vec2 a_Position; // 0
in vec3 a_Color; // 1

out vec3 f_Color; // Sending this to the fragment shader

void main()
{
  /*vec2 pos;
  pos.x = a_Position.x * cos(u_Time) - a_Position.y * sin(u_Time);
  pos.y = a_Position.y * cos(u_Time) + a_Position.x * sin(u_Time);*/

  gl_Position = vec4(a_Position.x, a_Position.y, 0.0, 1.0);
  f_Color = a_Color;
}
