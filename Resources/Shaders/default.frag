#version 460 core

layout (location = 0) out vec4 color;

in vec2 i_TexCoord;
in vec4 i_color;
in vec4 i_position;

uniform sampler2D u_texture;

void main()
{
    color = i_color;
}