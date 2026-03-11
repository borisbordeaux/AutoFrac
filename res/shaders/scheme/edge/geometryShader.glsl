#version 330 core

layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;

in vec3 vertexColor[];
in float vertexDistance[];

out vec3 vColor;
out float vDistance;

uniform vec2 u_invViewport;

vec2 toScreenSpace(vec4 vertex)
{
    return vec2(vertex.xy / vertex.w) / u_invViewport;
}

vec4 toWorldSpace(vec4 vertex)
{
    return vec4((vertex.xy * vertex.w) * u_invViewport, vertex.zw);
}

void main()
{
    vec2 p0 = toScreenSpace(gl_in[0].gl_Position);
    vec2 p1 = toScreenSpace(gl_in[1].gl_Position);
    vec2 v0 = normalize(p1 - p0);
    vec2 n0 = vec2(-v0.y, v0.x) * 4.0;

    vColor = vertexColor[0];
    vDistance = vertexDistance[0];

    gl_Position = toWorldSpace(vec4(p0 + n0, gl_in[0].gl_Position.zw));
    EmitVertex();
    gl_Position = toWorldSpace(vec4(p0 - n0, gl_in[0].gl_Position.zw));
    EmitVertex();

    vDistance = vertexDistance[1];

    gl_Position = toWorldSpace(vec4(p1 + n0, gl_in[1].gl_Position.zw));
    EmitVertex();
    gl_Position = toWorldSpace(vec4(p1 - n0, gl_in[1].gl_Position.zw));
    EmitVertex();

    EndPrimitive();
}