#version 330 core

in vec2 f_TexCoord;
in vec3 f_Position;
in vec3 f_Normal;

out vec4 o_Color;

uniform sampler2D u_Texure0;
uniform sampler2D u_Texure1;

uniform float u_Time;
uniform vec4 u_Color;

void main()
{
    //o_Color = vec4(f_Normal.x, f_Normal.y, f_Normal.z, 1.0);
    //o_Color = vec4(f_Normal, 1.0);
    o_Color = texture(u_Texure0, f_TexCoord);
    //o_Color = mix(col_a, u_Color, sin(u_Time + f_Position.x + f_Position.y ) * 0.5 + 0.5) * u_Color;
}
