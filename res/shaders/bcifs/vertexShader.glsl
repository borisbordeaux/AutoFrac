#version 330 core

layout (location = 0) in vec4 vPos;
layout (location = 1) in vec3 vFrontColor;
layout (location = 2) in vec3 vBackColor;

out vec3 vertexPos;
out vec3 vertexFrontColor;
out vec3 vertexBackColor;

uniform mat4 u_mvp;

void main()
{
    vertexPos = vPos.xyz;
    vertexFrontColor = vFrontColor;
    vertexBackColor = vBackColor;
    gl_Position = u_mvp * vPos;
}