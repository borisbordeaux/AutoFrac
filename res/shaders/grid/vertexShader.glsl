#version 330 core

layout (location = 0) in vec4 vPos;

uniform mat4 u_mvp;

void main()
{
    gl_Position = u_mvp * vPos;
}