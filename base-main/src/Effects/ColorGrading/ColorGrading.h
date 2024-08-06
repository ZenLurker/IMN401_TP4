#pragma once

#include "EffectGL.h"

class ColorGrading : public EffectGL {
public:
    ColorGrading(std::string name);


    ~ColorGrading();

    virtual void apply(FrameBufferObject *src, FrameBufferObject *target);

    virtual void animate(const float elapsedTime);

    virtual void displayInterface();

    void updateColorGrading();

    void addLut(Texture2D *my_lut);

protected:
    GLProgram *vp;
    GLProgram *fph;
    GLProgram *fpv;
    GLint l_blur_h;
    GLint l_blur_v;

    FrameBufferObject *tmp;
    GLint blurAmount;
    Texture2D *lut;
};
