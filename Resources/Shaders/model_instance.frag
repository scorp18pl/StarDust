#version 460 core

layout (location = 0) out vec4 color;

in vec2 i_TexCoord;
in vec4 i_color;
in vec4 i_position;
in vec4 i_normal;

void main()
{
    vec3 lightDir = vec3(0.0f, 0.0f, -1.0f);
    vec4 lightColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    float ambientStrength = 0.5f;

    color = i_color * ambientStrength + (1.0f - ambientStrength) * lightColor
    * max(dot(normalize(i_normal.xyz), normalize(-lightDir)), 0.0f);


}