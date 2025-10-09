#version 330 core

layout (location = 0) out vec4 color;

in vec2 texCoord;

uniform vec4 u_color;
uniform sampler2D u_Texture;

void main()
{
    vec4 texColor = texture(u_Texture, texCoord);
    color = u_color * texColor;
}