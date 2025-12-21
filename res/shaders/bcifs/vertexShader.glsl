#version 330 core

layout (location = 0) in vec4 vPos;
layout (location = 1) in vec3 vNormal;

out vec3 vertexPos;
out vec3 vertexNormal;

uniform mat4 u_mvp;

void main()
{
    vertexPos = vPos.xyz;
    vertexNormal = vNormal;
    gl_Position = u_mvp * vPos;
}