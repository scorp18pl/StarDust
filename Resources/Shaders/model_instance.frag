#version 460 core

layout (location = 0) out vec4 color;

in vec2 i_TexCoord;
in vec4 i_color;
in vec4 i_position;
in vec4 i_normal;

uniform vec4 u_lightColor;
uniform vec3 u_lightDir;

uniform float u_ambientStrenght;
uniform float u_diffuseStrenght;
uniform float u_specularStrenght;
uniform float u_shininess;

uniform vec4 u_colorPalette[64];
uniform int u_colorPaletteCount;

uniform vec3 u_viewPosition;

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

float calculateAmbientLight()
{
    return u_ambientStrenght;
}

float calculateDiffuseLight()
{
    return max(dot(i_normal.xyz, -u_lightDir), 0.0f) * u_diffuseStrenght;
}

float calculateSpecularLight(float shininess)
{
    vec3 viewDir = normalize(u_viewPosition - i_position.xyz);

    vec3 reflectDir = normalize(reflect(u_lightDir, i_normal.xyz));

    return pow(max(dot(reflectDir, viewDir), 0.0f), shininess) * u_specularStrenght;
}

vec4 calculateCombinedLight()
{
    float ambient = calculateAmbientLight();
    float diffuse = calculateDiffuseLight();

    float totalLight = ambient + diffuse;

    if (diffuse > 0.0)
    {
        totalLight += calculateSpecularLight(u_shininess);
    }

    return totalLight * u_lightColor;
}

void main()
{
    color = calculateCombinedLight() * i_color;


    if (u_colorPaletteCount > 0)
    {
        color = SnapToPalette(color);
    }
}