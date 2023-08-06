#version 460 core

layout (location = 0) out vec4 color;

in vec2 i_TexCoord;
in vec4 i_color;
in vec4 i_position;

uniform sampler2D u_texture;

void main()
{
    vec2 pos = i_position.xy;
    float val = floor(2.0 * length(pos - vec2(0.5)));

    if (i_TexCoord != vec2(-1.0, -1.0))
    {
        color = texture(u_texture, i_TexCoord.xy);
    }
    else
    {
        color = i_color;
    }

    color = color * vec4(vec3(1.0 - val), 1.0);
}