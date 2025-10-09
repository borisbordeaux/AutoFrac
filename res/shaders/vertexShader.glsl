#version 330 core

layout (location = 0) in vec4 vPos;
layout (location = 1) in vec2 vTexCoord;

out vec2 texCoord;

uniform mat4 u_mvp;

void main()
{
    gl_Position = u_mvp * vPos;
    texCoord = vTexCoord;
}