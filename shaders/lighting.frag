#version 460 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float attConstant;
    float attLinear;
    float attQuadratic;
};

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shine;
};

uniform Light light;
uniform Material material;

uniform vec3 cameraPos;

out vec4 FragColor;

void main()
{
    // Ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    // Diffuse
    vec3 lightDirection = light.position - FragPos;
    vec3 normalizedLightDirection = normalize(lightDirection);
    vec3 normalizedNormal = normalize(Normal);

    float diffuseValue = max(dot(normalizedNormal, normalizedLightDirection), 0.0);
    vec3 diffuse = light.diffuse * diffuseValue * vec3(texture(material.diffuse, TexCoords));

    // Specular
    vec3 cameraDirection = normalize(cameraPos - FragPos);
    vec3 reflectDirection = reflect(-normalizedLightDirection, normalizedNormal);

    float specularValue = pow(max(dot(cameraDirection, reflectDirection), 0.0), material.shine);
    vec3 specular = light.specular * specularValue * vec3(texture(material.specular, TexCoords));

    // Compute attenuation value
    float distanceFromLight = length(lightDirection);
    float attenuation = 1.0 / (light.attConstant + (light.attLinear * distanceFromLight) + light.attQuadratic * (distanceFromLight * distanceFromLight));

    // Apply attenuation
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}