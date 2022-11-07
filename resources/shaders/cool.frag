
#version 330 core
in vec2 f_Position;
in vec3 f_Color;
in vec2 f_TexCoord;

uniform sampler2D u_Sampler0;
uniform sampler2D u_Sampler1;
uniform float u_Time;
uniform vec4 u_Color;

out vec4 o_Color;

void main()
{
  /*vec4 color_a = vec4(0.5, 0.2, 0.8, 1.0);
  vec4 color_b = vec4(0.1, 0.9, 0.3, 1.0);

  o_Color = mix(color_a, color_b, sin(u_Time));*/

  //o_Color = vec4(f_Color, 1.0);

  vec4 color_a = texture(u_Sampler0, f_TexCoord);
  vec4 color_b = texture(u_Sampler1, f_TexCoord);

  o_Color = mix(color_a, color_b, sin(u_Time + f_Position.x) * 0.5 + 0.5);
}