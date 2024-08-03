#pragma once

#include "EffectGL.h"

class Flou : public EffectGL {
public:
    Flou(std::string name);

    ~Flou();

    virtual void apply(FrameBufferObject *src, FrameBufferObject *target);

    virtual void animate(const float elapsedTime);

    virtual void displayInterface();

    void updateFlou();

protected:
    GLProgram *vp;
    GLProgram *fph;
    GLProgram *fpv;
    GLint l_blur_h;
    GLint l_blur_v;

    FrameBufferObject *tmp;
    GLint blurAmount;
};
