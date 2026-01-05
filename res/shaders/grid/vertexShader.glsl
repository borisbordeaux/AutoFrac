#version 330 core

layout (location = 0) in vec4 vPos;
layout (location = 1) in vec3 vColor;

uniform mat4 u_mvp;

out vec3 color;

void main()
{
    gl_Position = u_mvp * vPos;
    color = vColor;
}