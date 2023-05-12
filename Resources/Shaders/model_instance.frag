#version 460 core

struct LightData
{
    // Direction for directional light, position for point light
    vec3 vector;
    vec4 color;
    float intensity;
};

layout (location = 0) out vec4 color;

in vec2 i_TexCoord;
in vec4 i_color;
in vec4 i_position;
in vec4 i_normal;

uniform vec3 u_viewPosition;

uniform float u_shininess;

uniform float u_ambientStrenght;
uniform float u_diffuseStrenght;
uniform float u_specularStrenght;

uniform float u_pointLightConstant;
uniform float u_pointLightLinear;
uniform float u_pointLightQuadratic;

uniform LightData u_pointLights[64];
uniform int u_pointLightCount;

uniform LightData u_directionalLights[16];
uniform int u_directionalLightCount;

uniform vec4 u_colorPalette[64];
uniform int u_colorPaletteCount;

float calculateDistance(vec4 color1, vec4 color2)
{
    return distance(color1, color2);
}

vec4 SnapToPalette(vec4 color)
{
    float minDistance = 1000000.0f;
    int minIndex = 0;
    for (int i = 0; i < u_colorPaletteCount; i++)
    {
        float distance = calculateDistance(color, u_colorPalette[i]);
        if (distance < minDistance)
        {
            minDistance = distance;
            minIndex = i;
        }
    }
    return u_colorPalette[minIndex];
}

float calculateDiffuseLight(vec3 lightDir, vec3 normal)
{
    return max(dot(normal, -lightDir), 0.0f) * u_diffuseStrenght;
}

float calculateSpecularLight(vec3 lightDir, float shininess)
{
    vec3 viewDir = normalize(u_viewPosition - i_position.xyz);
    vec3 reflectDir = normalize(reflect(lightDir, i_normal.xyz));

    return pow(max(dot(reflectDir, viewDir), 0.0f), shininess) * u_specularStrenght;
}

vec4 CalculateDirLight(LightData lightData)
{
    vec3 lightDir = normalize(lightData.vector);
    float diffuse = calculateDiffuseLight(lightDir, i_normal.xyz);
    float specular = float(diffuse > 0.0f) * calculateSpecularLight(lightDir, u_shininess);

    return (diffuse + specular) * lightData.color * lightData.intensity;
}

vec4 CalculatePointLight(LightData lightData)
{
    const float distance = length(lightData.vector - i_position.xyz);
    const float attenuation = 1.0f /
        (u_pointLightConstant +
        distance * (u_pointLightLinear + (u_pointLightQuadratic * distance)));

    vec3 lightDir = normalize(i_position.xyz - lightData.vector);
    return CalculateDirLight(
        LightData(lightDir, lightData.color, lightData.intensity)
        ) * attenuation;
}

vec4 calculateCombinedLight()
{
    vec4 dirAndPointLight = vec4(0.0f);
    for (int i = 0; i < u_pointLightCount; i++)
    {
        dirAndPointLight += CalculatePointLight(u_pointLights[i]);
    }

    for (int i = 0; i < u_directionalLightCount; i++)
    {
        dirAndPointLight += CalculateDirLight(u_directionalLights[i]);
    }

    return dirAndPointLight;
}

void main()
{
    color = (1.0f - u_ambientStrenght) * calculateCombinedLight() * i_color + u_ambientStrenght * i_color;
    color = clamp(color, 0.0f, 1.0f);

    if (u_colorPaletteCount > 0)
    {
        color = SnapToPalette(color);
    }
}