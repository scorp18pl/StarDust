#version 460 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 texCoord;

out vec2 i_TexCoord;
out vec4 i_color;
out vec4 i_position;

uniform mat4 u_VP;

void main()
{
    i_position = u_VP * position;
    i_TexCoord = texCoord;
    i_color = color;

    gl_Position = i_position;
}