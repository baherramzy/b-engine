#version 460 core

out vec4 FragColor;

in vec3 vertexColor;
in vec2 texCoord;

uniform sampler2D myTexture;

void main()
{
   FragColor = texture(myTexture, texCoord);
}