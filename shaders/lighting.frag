#version 460 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

struct Light
{
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

struct DirectedLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float attConstant;
    float attLinear;
    float attQuadratic;
};

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shine;
};

uniform Material material;
uniform Light light;
uniform DirectedLight dirLight;

#define POINT_LIGHTS 4
uniform PointLight pointLights[POINT_LIGHTS];

uniform vec3 cameraPos;

out vec4 FragColor;

vec3 applyDirectedLight(DirectedLight light, vec3 normal, vec3 viewDirection)
{
    vec3 lightDirection = normalize(-light.direction);

    float diffuseValue = max(dot(normal, lightDirection), 0.0);

    vec3 reflectDirection = reflect(-lightDirection, normal);
    float specularValue = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shine);

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diffuseValue * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * specularValue * vec3(texture(material.specular, TexCoords));

    return (ambient + diffuse + specular);
}

vec3 applyPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDirection)
{
    vec3 lightDirection = light.position - FragPos;
    vec3 normalizedLightDirection = normalize(lightDirection);

    // Diffuse
    float diffuseValue = max(dot(normal, normalizedLightDirection), 0.0);

    // Specular
    vec3 reflectDirection = reflect(-normalizedLightDirection, normal);
    float specularValue = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shine);

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diffuseValue * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * specularValue * vec3(texture(material.specular, TexCoords));

    float distanceFromLight = length(lightDirection);
    float attenuation = 1.0 / (light.attConstant + (light.attLinear * distanceFromLight) + (light.attQuadratic * distanceFromLight * distanceFromLight));

    vec3 result = ambient + diffuse + specular;
    vec3 attenuatedResult = result * attenuation;

    return attenuatedResult;
}

void main()
{
    vec3 normalizedNormal = normalize(Normal);
    vec3 cameraDirection = normalize(cameraPos - FragPos);

    vec3 result = vec3(0.0);

    // Directional light
    result += applyDirectedLight(dirLight, normalizedNormal, cameraDirection);

    // Point lights
    for (int i = 0; i < POINT_LIGHTS; ++i)
    {
        result += applyPointLight(pointLights[i], normalizedNormal, FragPos, cameraDirection);
    }

    // Spotlight
    // result += applySpotLight(spotLight, normalizedNormal, FragPos, cameraDirection);

    FragColor = vec4(result, 1.0);

    // // `light.direction` is the vector from the spotlight to the exact location it is aiming at,
    // // while `normalizedLightDirection` is the normalized vector from the fragment to the spotlight position.
    // // Theta is the cosine of the angle between them (dot product)
    // float theta = dot(normalizedLightDirection, normalize(-light.direction));

    // // Light edge smoothing
    // float epsilon = light.cutOff - light.outerCutOff;

    // // Fade out the light effect by interpolating the intensity between the inner cone
    // // and the outer cone, within a range of 0-1. This maximizes the intensity inside the inner cone,
    // // does not illuminate fragments outside the outer cone, and interpolates the intensity between the two cones.
    // float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // diffuse *= intensity;
    // specular *= intensity;
}