#version 460 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

struct Light {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

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

    vec3 lightDirection = light.position - FragPos;
    vec3 normalizedLightDirection = normalize(lightDirection);

    // Diffuse
    vec3 normalizedNormal = normalize(Normal);
    float diffuseValue = max(dot(normalizedNormal, normalizedLightDirection), 0.0);
    vec3 diffuse = light.diffuse * diffuseValue * vec3(texture(material.diffuse, TexCoords));

    // Specular
    vec3 cameraDirection = normalize(cameraPos - FragPos);
    vec3 reflectDirection = reflect(-normalizedLightDirection, normalizedNormal);

    float specularValue = pow(max(dot(cameraDirection, reflectDirection), 0.0), material.shine);
    vec3 specular = light.specular * specularValue * vec3(texture(material.specular, TexCoords));

    // `light.direction` is the vector from the spotlight to the exact location it is aiming at,
    // while `normalizedLightDirection` is the normalized vector from the fragment to the spotlight position.
    // Theta is the cosine of the angle between them (dot product)
    float theta = dot(normalizedLightDirection, normalize(-light.direction));

    // Light edge smoothing
    float epsilon = light.cutOff - light.outerCutOff;

    // Fade out the light effect by interpolating the intensity between the inner cone
    // and the outer cone, within a range of 0-1. This maximizes the intensity inside the inner cone,
    // does not illuminate fragments outside the outer cone, and interpolates the intensity between the two cones.
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse *= intensity;
    specular *= intensity;

    // Compute attenuation value
    float distanceFromLight = length(lightDirection);
    float attenuation = 1.0 / (light.attConstant + (light.attLinear * distanceFromLight) + (light.attQuadratic * distanceFromLight * distanceFromLight));

    vec3 result = ambient + diffuse + specular;
    vec3 attenuatedResult = result * attenuation;
    FragColor = vec4(attenuatedResult, 1.0);
}