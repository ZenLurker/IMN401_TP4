#version 460

// Shader inspirer de GPU Gems 3 : https://developer.nvidia.com/gpugems/gpugems3/part-ii-light-and-shadows/chapter-13-volumetric-light-scattering-post-process 
layout (binding = 0) uniform sampler2D godRaysFBO;      // La texture de la sc�ne rendue pr�alablement
uniform vec2 lightPositionOnScreen;                     // Position de la lumi�re sur l'�cran, en coordonn�es de texture
uniform float decay;                                    // D�croissance de l'intensit� des rayons
uniform float density;
uniform float weight;
uniform float exposure;                                 // Exposition des rayons
int numSamples = 150;                                 // Nombre d'�chantillons le long du rayon

in vec2 TexCoord;                                       // Coordonn�es de texture venant du vertex shader
layout(location = 0) out vec4 FragColor;

void main() {
    vec2 deltaTexCoord = (TexCoord - lightPositionOnScreen.xy);
    deltaTexCoord *= 1.0 / float(numSamples) * density;           // Calcul du pas entre chaque �chantillon

    vec2 coord = TexCoord.xy;
    vec3 sum = texture(godRaysFBO, coord).xyz;
    float illuminationDecay = 1.0;

    for (int i = 0; i < numSamples; i++) {
        coord -= deltaTexCoord;                         // D�placement du coord vers la lumi�re
        vec3 samp = texture(godRaysFBO, coord).xyz;
        samp *= illuminationDecay * weight;             // Application de la d�croissance
        sum += samp;
        illuminationDecay *= decay;                     // D�croissance exponentielle
    }

    sum *= exposure;                                    // Application de l'exposition
    FragColor = vec4(sum, 1.0);
    //FragColor.rgb = vec3(numSamples/62);              // Pour le d�bug
}
