#include "Flou.h"
#include <glm/gtc/type_ptr.hpp>

Flou::Flou(std::string name) : EffectGL(name) {
    vp = new GLProgram(EffectPath + "common/Effect-VS.glsl", GL_VERTEX_SHADER);
    fph = new GLProgram(EffectPath + "Flou/FlouHorizontal-FS.glsl", GL_FRAGMENT_SHADER);
    fpv = new GLProgram(EffectPath + "Flou/FlouVertical-FS.glsl", GL_FRAGMENT_SHADER);
    tmp = new FrameBufferObject();

    m_ProgramPipeline->useProgramStage(vp, GL_VERTEX_SHADER_BIT);
    m_ProgramPipeline->link();
    m_ProgramPipeline->printInfoLog();

    l_blur_h = glGetUniformLocation(fph->getId(), "blurAmount");
    l_blur_v = glGetUniformLocation(fpv->getId(), "blurAmount");

    blurAmount = 0;
    updateFlou();
}

Flou::~Flou() {}

void Flou::apply(FrameBufferObject *src, FrameBufferObject *target) {
    // Passe 1: Flou horizontal
    glBindTextureUnit(0, src->getColorTexture()->getId());
    tmp->enable();

    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_ProgramPipeline->useProgramStage(fph, GL_FRAGMENT_SHADER_BIT);
    m_ProgramPipeline->bind();
    drawQuad();
    m_ProgramPipeline->release();
    tmp->disable();

    // Passe 2: Flou vertical
    glBindTextureUnit(0, tmp->getColorTexture()->getId());

    if (target)
        target->enable();

    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_ProgramPipeline->useProgramStage(fpv, GL_FRAGMENT_SHADER_BIT);
    m_ProgramPipeline->bind();
    drawQuad();
    m_ProgramPipeline->release();

    if (target)
        target->disable();
}

void Flou::animate(const float elapsedTime) {}

void Flou::updateFlou() {
    glProgramUniform1i(fph->getId(), l_blur_h, blurAmount);
    glProgramUniform1i(fpv->getId(), l_blur_v, blurAmount);
}

void Flou::displayInterface() {
    bool upd = false;
    upd = ImGui::SliderInt("Blur Amount", &blurAmount, 0, 10, "Blur amount = %.0f");

    if (upd)
        updateFlou();
}
