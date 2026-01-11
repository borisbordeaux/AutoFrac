#version 330 core

layout (location = 0) out vec4 fragColor;

in vec3 vertexPos;
in vec3 vertexNormal;

uniform vec3 lightPos;
uniform vec3 cameraPos;

void main()
{
    vec3 color;
    if (gl_FrontFacing) {
        color = vec3(100.0/255.0, 100.0/255.0, 100.0/255.0);
    } else {
        color = vec3(104.0/255.0, 146.0/255.0, 59.0/255.0);
    }
    vec3 ambientColor = 0.3 * color;
    vec3 diffuseColor = color;
    vec3 specularColor = vec3(0.5);
    vec3 N = normalize(vertexNormal);
    vec3 L = normalize(lightPos - vertexPos);

    // Lambert's cosine law
    float lambertian = abs(dot(N, L));
    float specular = 0.0;
    if (lambertian > 0.0) {
        vec3 R = reflect(-L, N); // Reflected light vector
        vec3 V = normalize(cameraPos - vertexPos); // Vector to viewer
        // Compute the specular term
        float specAngle = max(dot(R, V), 0.0);
        specular = pow(specAngle, 20.0);
    }
    fragColor = vec4(ambientColor + lambertian * diffuseColor + specular * specularColor, 1.0);
}