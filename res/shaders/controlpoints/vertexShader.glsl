#version 460 core

layout (location = 0) in vec4 vPos;
layout (location = 1) in float vConstness;

uniform mat4 u_mvp;

void main() {
    gl_Position = u_mvp * vPos;
    gl_PointSize = vConstness * 8.0;
}