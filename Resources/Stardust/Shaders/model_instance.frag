#version 460 core

layout (location = 0) out vec4 color;

in vec2 i_texCoord;
in vec4 i_color;
in vec4 i_position;
in vec4 i_normal;

uniform vec3 u_viewPosition;

uniform vec4 u_colorPalette[64];
uniform int u_colorPaletteCount;

uniform sampler2D u_texture;

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

void main()
{
    color = i_color;
    if (i_texCoord.x >= 0.0f && i_texCoord.y >= 0.0f)
    {
        color = texture(u_texture, i_texCoord) * color;
    }

    if (u_colorPaletteCount > 0)
    {
        color = SnapToPalette(color);
    }
}