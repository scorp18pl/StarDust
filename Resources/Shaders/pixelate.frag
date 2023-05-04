#version 460 core

layout (location = 0) out vec4 color;

in vec2 i_TexCoord;

uniform sampler2D screenTexture;

void main()
{
    color = texture(screenTexture, i_TexCoord);
}
