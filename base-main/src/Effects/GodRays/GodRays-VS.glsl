#version 460

// D�finition des sorties vers le rasterizer
out gl_PerVertex {
    vec4 gl_Position;
    float gl_PointSize;
    float gl_ClipDistance[];
};

layout(location = 0) in vec3 Position; // Position du vertex (g�n�ralement en coordonn�es NDC)
layout(location = 3) in vec2 texCoord; // Coordonn�es de texture du vertex

out vec2 TexCoord; // Coordonn�es de texture � transmettre au fragment shader

void main() {
    gl_Position = vec4(Position, 1.0); // Directement en coordonn�es NDC

    TexCoord = texCoord; // Transmet les coordonn�es de texture
}
