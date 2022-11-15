#version 330 core

// Inputs
in vec3 f_World;
in vec3 f_Normal;

// Uniform
uniform vec3 u_DirectionalLight;
uniform vec3 u_EyePosition;

// Light data
const vec3 ambient = vec3(0.4, 1.0, 0.2);
const float ambient_strength = 0.05;
const float spec_exponent = 32.0;
const float spec_intensity = 0.2;

// Ambient lightning

// Output
out vec4 o_Color;

void main()
{
  float diffuse = dot(f_Normal, -u_DirectionalLight);
  diffuse = clamp(diffuse, 0.0, 1.0);

  // Specular lighing (using Blinn-Phong)
  vec3 eye_dir = normalize(u_EyePosition - f_World);
  vec3 middle = normalize(eye_dir - u_DirectionalLight);
  float specular = max(dot(f_Normal, middle), 0);
  specular = pow(specular, spec_exponent) * spec_intensity;

  o_Color = vec4((ambient * ambient_strength) + vec3(diffuse) + specular, 1.0);
  //o_Color = vec4(vec3(specular), 1.0);
  //o_Color = vec4(diffuse, diffuse, diffuse, 1.0);
}
