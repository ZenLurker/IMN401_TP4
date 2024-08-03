
#include "RotationMaterial.h"
#include "Node.h"
#include <glm/gtc/type_ptr.hpp>

RotationMaterial::RotationMaterial(std::string name) : MaterialGL(name) {
    time_since_start = 0.f;
    rotation_speed = 0.0005f;
}

RotationMaterial::~RotationMaterial() {}

void RotationMaterial::render(Node *o) {
}

void RotationMaterial::animate(Node *o, const float elapsedTime) {
    GLfloat deltaTime = clock() - time_since_start;
    time_since_start = clock();
    o->frame()->rotate(glm::vec3(0, 1, 0), rotation_speed * deltaTime);
}
