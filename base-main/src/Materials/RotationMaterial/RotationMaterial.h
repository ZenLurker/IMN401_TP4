

#ifndef _RotationMaterial_
#define _RotationMaterial_

#include "MaterialGL.h"

class RotationMaterial : public MaterialGL {
public:
    RotationMaterial(std::string name = "");

    ~RotationMaterial();

    virtual void render(Node *o);

    virtual void animate(Node *o, const float elapsedTime);

    virtual void displayInterface(){};

    GLfloat time_since_start, rotation_speed;
};

#endif