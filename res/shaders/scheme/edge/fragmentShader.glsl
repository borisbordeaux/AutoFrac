#version 330 core

in vec3 vColor;
in float vDistance;

out vec4 fragColor;

void main() {
    float dashSize = 10.0;
    float gapSize = 10.0;
    if (fract(vDistance / (dashSize + gapSize)) > dashSize/(dashSize + gapSize)) {
        discard;
    }
    fragColor = vec4(vColor, 1.0);
}