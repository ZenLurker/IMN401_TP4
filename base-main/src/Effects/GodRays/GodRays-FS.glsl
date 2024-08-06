#version 460

// Shader inspirer de GPU Gems 3 : https://developer.nvidia.com/gpugems/gpugems3/part-ii-light-and-shadows/chapter-13-volumetric-light-scattering-post-process 
layout (binding = 0) uniform sampler2D godRaysFBO;      // La texture de la scène rendue préalablement
uniform vec2 lightPositionOnScreen;                     // Position de la lumière sur l'écran, en coordonnées de texture
uniform float decay;                                    // Décroissance de l'intensité des rayons
uniform float density;
uniform float weight;
uniform float exposure;                                 // Exposition des rayons
int numSamples = 150;                                 // Nombre d'échantillons le long du rayon

in vec2 TexCoord;                                       // Coordonnées de texture venant du vertex shader
layout(location = 0) out vec4 FragColor;

void main() {
    vec2 deltaTexCoord = (TexCoord - lightPositionOnScreen.xy);
    deltaTexCoord *= 1.0 / float(numSamples) * density;           // Calcul du pas entre chaque échantillon

    vec2 coord = TexCoord.xy;
    vec3 sum = texture(godRaysFBO, coord).xyz;
    float illuminationDecay = 1.0;

    for (int i = 0; i < numSamples; i++) {
        coord -= deltaTexCoord;                         // Déplacement du coord vers la lumière
        vec3 samp = texture(godRaysFBO, coord).xyz;
        samp *= illuminationDecay * weight;             // Application de la décroissance
        sum += samp;
        illuminationDecay *= decay;                     // Décroissance exponentielle
    }

    sum *= exposure;                                    // Application de l'exposition
    FragColor = vec4(sum, 1.0);
    //FragColor.rgb = vec3(numSamples/62);              // Pour le débug
}
