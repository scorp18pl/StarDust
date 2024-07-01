#version 460 core

in vec3 position;
in vec3 normal;
in vec2 texCoord;

in vec4 modelMatrixRow0;
in vec4 modelMatrixRow1;
in vec4 modelMatrixRow2;

in vec4 normalMatrixRow0;
in vec4 normalMatrixRow1;
in vec4 normalMatrixRow2;

in vec4 color;

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