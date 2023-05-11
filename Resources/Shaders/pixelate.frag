#version 460 core

layout (location = 0) out vec4 color;

in vec2 i_TexCoord;

uniform sampler2D screenTexture;
uniform float distortionFactor;
uniform float colorShiftFactor;

vec2 GetUvLensDistorted(vec2 uv, float distortionFactor)
{
    vec2 uvNormalized = uv * 2 - 1;
    float dist = length(uvNormalized);

    uvNormalized = uvNormalized * (1 + dist * dist * dist * distortionFactor);
    vec2 uvCoord = uvNormalized * 0.5 + 0.5;

    return uvCoord;
}

vec4 GetTextureColor(vec2 uvCoord)
{
    if (uvCoord.x < 0 || uvCoord.x > 1 || uvCoord.y < 0 || uvCoord.y > 1)
    {
        return vec4(0, 0, 0, 1);
    }

    return texture(screenTexture, uvCoord);
}

void main()
{
    float redDistortionFactor = distortionFactor * 0.9;
    float blueDistortionFactor = distortionFactor;
    float greenDistortionFactor = distortionFactor * 1.1;

    vec2 uvRed = GetUvLensDistorted(i_TexCoord, redDistortionFactor) + vec2(colorShiftFactor, 0.0f);
    vec2 uvGreen = GetUvLensDistorted(i_TexCoord, greenDistortionFactor);
    vec2 uvBlue = GetUvLensDistorted(i_TexCoord, blueDistortionFactor) + vec2(-colorShiftFactor, 0.0f);

    color.x = GetTextureColor(uvRed).x;
    color.y = GetTextureColor(uvGreen).y;
    color.z = GetTextureColor(uvBlue).z;
}
