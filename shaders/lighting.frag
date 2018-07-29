#version 460 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
};

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shine;
};

uniform Light light;
uniform Material material;

uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform vec3 lightColor;

out vec4 FragColor;

void main()
{
    // Ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    // Diffuse
    vec3 normalizedNormal = normalize(Normal);
    vec3 lightDirection = normalize(light.position - FragPos);

    float diffuseValue = max(dot(normalizedNormal, lightDirection), 0.0);
    vec3 diffuse = light.diffuse * diffuseValue * vec3(texture(material.diffuse, TexCoords));

    // Specular
    vec3 cameraDirection = normalize(cameraPos - FragPos);
    vec3 reflectDirection = reflect(-lightDirection, normalizedNormal);

    float specularValue = pow(max(dot(cameraDirection, reflectDirection), 0.0), material.shine);
    vec3 specular = light.specular * specularValue * vec3(texture(material.specular, TexCoords));

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}