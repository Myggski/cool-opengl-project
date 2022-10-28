
#version 330 core
in vec3 f_Color;

uniform float u_Time;
uniform vec4 u_Color;

out vec4 o_Color;

void main()
{
  /*vec4 color_a = vec4(0.5, 0.2, 0.8, 1.0);
  vec4 color_b = vec4(0.1, 0.9, 0.3, 1.0);

  o_Color = mix(color_a, color_b, sin(u_Time));*/

  o_Color = vec4(f_Color, 1.0);
}