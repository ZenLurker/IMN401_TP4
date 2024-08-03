#version 460

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Proj;
uniform float elapsedTime;

out gl_PerVertex {
    vec4 gl_Position;
    float gl_PointSize;
    float gl_ClipDistance[];
};

layout(location = 0) in vec3 Position;
layout(location = 2) in vec3 Normal; // Entrée pour les normales

layout(location = 2) out vec3 FragColor; // Variable de sortie pour la couleur du fragment

void main() {
    // Déformation vis-à-vis le temps 
    float baseSizeFactor = 1.0;
    float amplitudeFactor = 0.4;
    float timeScale = 0.65;     // Scale factor to slow down the rate of pulsation
    vec3 timePosition = Position * (baseSizeFactor + amplitudeFactor * sin(elapsedTime * timeScale));

    gl_Position = Proj * View * Model * vec4(timePosition, 1.0);

    // Valeur absolue de la normale pour la couleur.
    FragColor = abs(Normal); 
}
