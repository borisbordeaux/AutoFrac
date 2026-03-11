#version 330 core

layout (location = 0) in vec4 vPos;
layout (location = 1) in vec3 vColor;
layout (location = 2) in float vDist;

uniform mat4 u_mvp;

out vec3 vertexColor;
out float vertexDistance;

void main() {
    vertexColor = vColor;
    vertexDistance = vDist;
    gl_Position = u_mvp * vPos;
}