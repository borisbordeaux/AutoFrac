#version 330 core

layout (location = 0) in vec4 vPos;
layout (location = 1) in float vSelected;

out float vertexSelected;

uniform mat4 u_mvp;

void main() {
    vertexSelected = vSelected;
    gl_Position = u_mvp * vPos;
}