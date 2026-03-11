#version 330 core

in float vertexSelected;

out vec4 fragColor;

void main() {
    vec3 color = vec3(0.5);
    if (vertexSelected > 0.0) {
        color = vec3(0.8);
    }
    if (!gl_FrontFacing){
        color = vec3(0.2);
    }
    fragColor = vec4(color, 1.0);
}