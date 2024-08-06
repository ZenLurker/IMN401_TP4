#pragma once

#include "EffectGL.h"

class GrayScale : public EffectGL {
public:
    GrayScale(std::string name);

    ~GrayScale();

    virtual void apply(FrameBufferObject *src, FrameBufferObject *target);

    virtual void animate(const float elapsedTime);

    virtual void displayInterface();

    void updateGrayScale();

protected:
    GLProgram *vp;
    GLProgram *fp;
};
