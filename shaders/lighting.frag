#version 460 core

in vec3 FragPos;
in vec3 normal;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

out vec4 FragColor;

void main()
{
    float ambientLightStrength = 0.2;
    vec3 ambient = ambientLightStrength * lightColor;

    vec3 normalizedNormal = normalize(normal);
    vec3 lightDirection = normalize(lightPos - FragPos);

    float diffuseValue = max(dot(normalizedNormal, lightDirection), 0.0);
    vec3 diffuse = diffuseValue * lightColor;

    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);
}