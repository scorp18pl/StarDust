#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

layout (location = 3) in vec4 modelMatrixRow0;
layout (location = 4) in vec4 modelMatrixRow1;
layout (location = 5) in vec4 modelMatrixRow2;

layout (location = 6) in vec4 normalMatrixRow0;
layout (location = 7) in vec4 normalMatrixRow1;
layout (location = 8) in vec4 normalMatrixRow2;

layout (location = 9) in vec4 color;

out vec2 i_texCoord;
out vec4 i_color;
out vec4 i_position;
out vec4 i_normal;

uniform mat4 u_view;
uniform mat4 u_proj;

mat4 RowsToMatrix(vec4 row0, vec4 row1, vec4 row2)
{
    return transpose(mat4(row0, row1, row2, vec4(0.0, 0.0, 0.0, 1.0)));
}

void main()
{
    mat4 modelMatrix = RowsToMatrix(modelMatrixRow0, modelMatrixRow1, modelMatrixRow2);
    mat4 normalMatrix = RowsToMatrix(normalMatrixRow0, normalMatrixRow1, normalMatrixRow2);

    i_position = modelMatrix * vec4(position, 1.0f);
    i_texCoord = texCoord;

    i_normal = normalMatrix * vec4(normal, 1.0f);

    i_color = color;

    gl_Position = u_proj * u_view * i_position;
}