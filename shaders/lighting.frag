#version 460 core

in vec3 FragPos;
in vec3 normal;

uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

out vec4 FragColor;

void main()
{
    // Ambient
    float ambientLightStrength = 0.3;
    vec3 ambient = ambientLightStrength * lightColor;

    // Diffuse
    vec3 normalizedNormal = normalize(normal);
    vec3 lightDirection = normalize(lightPos - FragPos);

    float diffuseValue = max(dot(normalizedNormal, lightDirection), 0.0);
    vec3 diffuse = diffuseValue * lightColor;

    // Specular
    float specularStrength = 0.6;
    int shineValue = 32;
    vec3 cameraDirection = normalize(cameraPos - FragPos);
    vec3 reflectDirection = reflect(-lightDirection, normalizedNormal);

    float specularValue = pow(max(dot(cameraDirection, reflectDirection), 0.0), shineValue);
    vec3 specular = specularStrength * specularValue * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}