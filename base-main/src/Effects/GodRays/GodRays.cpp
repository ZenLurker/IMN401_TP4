#include "GodRays.h"
#include <glm/gtc/type_ptr.hpp>

GodRays::GodRays(std::string &name) : EffectGL(name) {
    vp = new GLProgram(EffectPath + "GodRays/GodRays-VS.glsl", GL_VERTEX_SHADER);
    fpr = new GLProgram(EffectPath + "GodRays/GodRays-FS.glsl", GL_FRAGMENT_SHADER);

    m_ProgramPipeline->useProgramStage(vp, GL_VERTEX_SHADER_BIT);
    m_ProgramPipeline->link();
    m_ProgramPipeline->printInfoLog();

    // Récupérer les uniform locations
    l_lightPosition = glGetUniformLocation(fpr->getId(), "lightPositionOnScreen");
    l_decay = glGetUniformLocation(fpr->getId(), "decay");
    l_density = glGetUniformLocation(fpr->getId(), "density");
    l_weight = glGetUniformLocation(fpr->getId(), "weight");
    l_exposure = glGetUniformLocation(fpr->getId(), "exposure");
    // m_NumSample = glGetUniformLocation(fpr->getId(), "numSamples");

    // Initialiser les valeurs des paramètres
    lightPos = glm::vec3(5, 10, 3);
    decay = 0.0f;     // 0.5f
    density = 0.0f;   // 0.4f
    weight = 0.75f;   // 0.5f
    exposure = 0.85f; // 1.0f
    // numSample = 62;

    updateGodRays();
}

GodRays::~GodRays() {
    delete vp;
    delete fpr;
}

void GodRays::apply(FrameBufferObject *src, FrameBufferObject *target) {
    glBindTextureUnit(0, src->getColorTexture()->getId());
    if (target) {
        target->enable();
    }

    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_ProgramPipeline->useProgramStage(fpr, GL_FRAGMENT_SHADER_BIT);
    m_ProgramPipeline->bind();
    drawQuad();
    // Nettoyage
    m_ProgramPipeline->release();
    if (target) {
        target->disable();
    }
}

void GodRays::animate(const float elapsedTime) {
    // Si besoin de modifier dynamiquement les paramètres
}

void GodRays::updateGodRays() {
    glProgramUniform2fv(fpr->getId(), l_lightPosition, 1, glm::value_ptr(lightPos));
    glProgramUniform1f(fpr->getId(), l_decay, decay);
    glProgramUniform1f(fpr->getId(), l_density, density);
    glProgramUniform1f(fpr->getId(), l_weight, weight);
    glProgramUniform1f(fpr->getId(), l_exposure, exposure);
    // glProgramUniform1f(fpr->getId(), m_NumSample, numSample);
}
void GodRays::displayInterface() {
    bool upd = false;
    bool upd1 = false;
    bool upd2 = false;
    bool upd3 = false;
    bool upd4 = false;
    ImGui::Begin("God Rays option");
    ImGui::Separator();
    upd = ImGui::SliderFloat2("Light Position", glm::value_ptr(lightPos), -10.0f, 10.0f);
    upd1 = ImGui::SliderFloat("decay", &decay, 0, 1, "decay = %.3f");
    upd2 = ImGui::SliderFloat("density", &density, 0, 1, "density = %.3f");
    upd3 = ImGui::SliderFloat("weight", &weight, 0, 1, "weight = %.3f");
    upd4 = ImGui::SliderFloat("exposure", &exposure, 0, 1, "exposure = %.3f");
    // upd = ImGui::SliderInt("NumSample", &numSample, 0, 62, "Num Sample = %.0f");

    ImGui::End();
    if (upd || upd1 || upd2 || upd3 || upd4)
        updateGodRays();
}
