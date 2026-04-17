#version 460 core

layout (location = 0) in vec4 vPos;

uniform mat4 u_mvp;

void main() {
    gl_Position = u_mvp * vPos;
    gl_PointSize = 16.0;
}