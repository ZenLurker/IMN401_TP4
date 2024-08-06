#ifndef GOD_RAYS_H
#define GOD_RAYS_H

#include "EffectGL.h"
#include <string>

class GodRays : public EffectGL {
public:
    // Constructeur qui initialise l'effet God Rays avec les shaders appropriés
    GodRays(std::string &name);
    // Destructeur
    virtual ~GodRays();
    // Applique l'effet God Rays sur un buffer de frame donné, utilisant src comme la source de lumière et rendant dans target
    virtual void apply(FrameBufferObject *src, FrameBufferObject *target) override;
    // Anime l'effet en fonction du temps écoulé
    virtual void animate(float elapsedTime) override;
    void updateGodRays();
    void displayInterface();
    // Affiche l'interface de contrôle de l'effet pour ajustement en temps réel
   // virtual void displayInterface() override;

    void configureGodRaysParameters();

protected:
    // Identifiants des uniformes dans le shader
    GLuint l_lightPosition; // Position de la source lumineuse
    GLuint l_decay;         // Décroissance de l'intensité
    GLuint l_density;       // Densité des rayons
    GLuint l_weight;        // Poids des rayons
    GLuint l_exposure;      // Exposition globale de l'effet

    // Paramètres contrôlables de l'effet God Rays
    glm::vec2 lightPos;
    float decay;
    float density;
    float weight;
    float exposure;

private:
    GLProgram *vp;
    GLProgram *fpr;
    GLuint m_textureUniform;       // Uniform location for the texture sampler
    GLuint m_lightPositionUniform; // Uniform location for the light position
    GLuint m_intensityUniform;     // Uniform location for the intensity of the god rays

    int windowWidth, windowHeight; // Dimensions of the rendering window
};

#endif // GOD_RAYS_H
