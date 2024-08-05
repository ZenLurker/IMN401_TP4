

#ifndef _MATERIALGL_
#define _MATERIALGL_

#include "GLProgramPipeline.h"
#include "Scene.h"
class Node;

struct PhongParams {
    glm::vec4 coeff;  // les parametres de Phong : coeff.x : ka, coeff.y = kd, coeff.z : ks, coeff.w : s
    glm::vec3 albedo; // La couleur diffuse
};

class MaterialGL {
public:
    GLProgramPipeline *m_ProgramPipeline;

    MaterialGL(std::string name = "");

    ~MaterialGL();

    virtual void render(Node *o);

    virtual void animate(Node *o, const float elapsedTime);

    std::string getName() { return m_Name; };

    virtual void displayInterface(){};

protected:
    std::string m_Name;

    Scene *scene;
};

#endif