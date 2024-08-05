#include "DisplacementMaterial.h"
#include "Node.h"
#include <glm/gtc/type_ptr.hpp>

// Lets reserve 5 texture units per section, the last one for the normal map
const GLint UNITS_PER_SECTION = 5;

DisplacementMaterial::DisplacementMaterial(std::string name) : MaterialGL(name) {

    vp = new GLProgram(MaterialPath + "TextureMaterial/TextureMaterial-VS.glsl", GL_VERTEX_SHADER);
    fp = new GLProgram(MaterialPath + "TextureMaterial/TextureMaterial-FS.glsl", GL_FRAGMENT_SHADER);

    m_ProgramPipeline->useProgramStage(vp, GL_VERTEX_SHADER_BIT);
    m_ProgramPipeline->useProgramStage(fp, GL_FRAGMENT_SHADER_BIT);

    l_View = glGetUniformLocation(vp->getId(), "View");
    l_Proj = glGetUniformLocation(vp->getId(), "Proj");
    l_Model = glGetUniformLocation(vp->getId(), "Model");
    l_PosLum = glGetUniformLocation(vp->getId(), "PosLum");
    l_PosCam = glGetUniformLocation(vp->getId(), "PosCam");
    l_Phong = glGetUniformLocation(fp->getId(), "Phong");
    l_Albedo = glGetUniformLocation(fp->getId(), "diffuseAlbedo");
    l_displacementScale = glGetUniformLocation(vp->getId(), "displacementScale");

    param.albedo = glm::vec3(0.8, 0.8, 0.8);
    param.coeff = glm::vec4(0.2, 0.8, 1.0, 100.0);

    updatePhong();
}

DisplacementMaterial::~DisplacementMaterial() {
}

void DisplacementMaterial::render(Node *o) {

    m_ProgramPipeline->bind();
    glBindTextureUnit(0, texture_diffuse->getId());
    glBindTextureUnit(1, texture_normals->getId());

    o->drawGeometry(GL_TRIANGLES);

    m_ProgramPipeline->release();
}

void DisplacementMaterial::animate(Node *o, const float elapsedTime) {

    // Recuperer la matrice view
    glm::mat4 view = Scene::getInstance()->camera()->getViewMatrix();
    // Recuperer la matrice proj
    glm::mat4 proj = Scene::getInstance()->camera()->getProjectionMatrix();
    // Recuperer la matrice model
    glm::mat4 model = o->frame()->getModelMatrix();

    glProgramUniformMatrix4fv(vp->getId(), l_View, 1, GL_FALSE, glm::value_ptr(view));
    glProgramUniformMatrix4fv(vp->getId(), l_Proj, 1, GL_FALSE, glm::value_ptr(proj));
    glProgramUniformMatrix4fv(vp->getId(), l_Model, 1, GL_FALSE, glm::value_ptr(model));

    // convertir le point (0,0,0) du repère "light" vers le repere de l'objet
    glProgramUniform3fv(vp->getId(), l_PosLum, 1, glm::value_ptr(Scene::getInstance()->getNode("L")->frame()->convertPtTo(glm::vec3(0.0, 0.0, 0.0), o->frame())));
    // convertir le point (0,0,0) du repère camera vers le repere de l'objet
    glProgramUniform3fv(vp->getId(), l_PosCam, 1, glm::value_ptr(Scene::getInstance()->camera()->frame()->convertPtTo(glm::vec3(0.0, 0.0, 0.0), o->frame())));
    glProgramUniform1f(vp->getId(), l_displacementScale, 1.f);
}

void DisplacementMaterial::updatePhong() {
    glProgramUniform4fv(fp->getId(), l_Phong, 1, glm::value_ptr(glm::vec4(param.coeff)));
    glProgramUniform3fv(fp->getId(), l_Albedo, 1, glm::value_ptr(param.albedo));
}

void DisplacementMaterial::displayInterface() {
    bool upd = false;
    upd = ImGui::SliderFloat("ambiant", &param.coeff.x, 0.0f, 1.0f, "ambiant = %.2f") || upd;
    upd = ImGui::SliderFloat("diffuse", &param.coeff.y, 0.0f, 1.0f, "diffuse = %.2f") || upd;
    upd = ImGui::SliderFloat("specular", &param.coeff.z, 0.0f, 2.0f, "specular = %.2f") || upd;
    upd = ImGui::SliderFloat("exposant", &param.coeff.w, 0.1f, 200.0f, "exposant = %f") || upd;

    upd = ImGui::ColorPicker3("Albedo", glm::value_ptr(param.albedo)) || upd;

    if (upd)
        updatePhong();
}

void DisplacementMaterial::setNormalMap(Texture2D *textureNormals) {
    texture_normals = textureNormals;
}

void DisplacementMaterial::setDiffuseTexture(Texture2D *textureDiffuse) {
    texture_diffuse = textureDiffuse;
}

void DisplacementMaterial::setPhong(glm::vec3 albedo, glm::vec4 coefficient) {
    param.albedo = albedo;
    param.coeff = coefficient;
}
