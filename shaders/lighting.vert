#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 FragPos;
out vec3 normal;

void main()
{
   gl_Position = proj * view * model * vec4(aPos, 1.0);
   FragPos = vec3(model * vec4(aPos, 1.0));

   // Compute normal matrix to adjust for non-uniform scaling
   // (Expensive to compute in the shader, but fine for demo purposes)
   normal = mat3(transpose(inverse(model))) * aNormal;
}