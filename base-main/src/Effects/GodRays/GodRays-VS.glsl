#version 460

// Définition des sorties vers le rasterizer
out gl_PerVertex {
    vec4 gl_Position;
    float gl_PointSize;
    float gl_ClipDistance[];
};

layout(location = 0) in vec3 Position; // Position du vertex (généralement en coordonnées NDC)
layout(location = 3) in vec2 texCoord; // Coordonnées de texture du vertex

out vec2 TexCoord; // Coordonnées de texture à transmettre au fragment shader

void main() {
    gl_Position = vec4(Position, 1.0); // Directement en coordonnées NDC

    TexCoord = texCoord; // Transmet les coordonnées de texture
}
