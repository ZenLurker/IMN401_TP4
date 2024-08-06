#include "ColorGrading.h"
#include <glm/gtc/type_ptr.hpp>

ColorGrading::ColorGrading(std::string name) : EffectGL(name) {
    vp = new GLProgram(EffectPath + "common/Effect-VS.glsl", GL_VERTEX_SHADER);
    fph = new GLProgram(EffectPath + "ColorGrading/ColorGrading-FS.glsl", GL_FRAGMENT_SHADER);

    m_ProgramPipeline->useProgramStage(vp, GL_VERTEX_SHADER_BIT);
    m_ProgramPipeline->useProgramStage(fph, GL_FRAGMENT_SHADER_BIT);
    m_ProgramPipeline->link();
    m_ProgramPipeline->printInfoLog();
}

ColorGrading::~ColorGrading() {}

void ColorGrading::apply(FrameBufferObject *src, FrameBufferObject *target) {
    // Bind the source texture
    glBindTextureUnit(0, src->getColorTexture()->getId());

    // Bind the LUT texture
    glBindTextureUnit(1, lut->getId());

    if (target)
        target->enable();

    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set the uniforms
    glProgramUniform1f(fph->getId(), glGetUniformLocation(fph->getId(), "fLUT_TileSizeXY"), 32.0); // Example value
    glProgramUniform1f(fph->getId(), glGetUniformLocation(fph->getId(), "fLUT_TileAmount"), 32.0); // Example value
    glProgramUniform1f(fph->getId(), glGetUniformLocation(fph->getId(), "fLUT_LutAmount"), 18.0);  // Example value
    glProgramUniform1i(fph->getId(), glGetUniformLocation(fph->getId(), "fLUT_LutSelector"), 1);   // Example value
    glProgramUniform1f(fph->getId(), glGetUniformLocation(fph->getId(), "fLUT_AmountChroma"), 1);  // Example value
    glProgramUniform1f(fph->getId(), glGetUniformLocation(fph->getId(), "fLUT_AmountLuma"), 1);    // Example value

    m_ProgramPipeline->bind();
    drawQuad();
    m_ProgramPipeline->release();

    if (target)
        target->disable();
}

void ColorGrading::animate(const float elapsedTime) {}

void ColorGrading::displayInterface() {}

void ColorGrading::addLut(Texture2D *my_lut) {

    lut = my_lut;
}
