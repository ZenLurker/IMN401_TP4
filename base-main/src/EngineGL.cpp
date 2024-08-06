
#include "EngineGL.h"
#include "Scene.h"

#include "Materials/DisplacementMappingMaterial/DisplacementMaterial.h"
#include "Materials/TextureMaterial/TextureMaterial.h"

#include "Materials/BaseMaterial/BaseMaterial.h"
#include "Texture2D.h"

#include <glfw3native.h>

bool EngineGL::init() {
    LOG_INFO << "Initializing Scene" << std::endl;

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, m_Width, m_Height);
    setClearColor(glm::vec4(.5, .5, .5, 1));

    myFBO = new FrameBufferObject("", 2048, 2048);
    godRays = scene->getEffect<GodRays>("GodRays");
    display = scene->getEffect<Display>("Display");
    colorGrading = scene->getEffect<ColorGrading>("ColorGrading");

    Texture2D *texture_LUT = new Texture2D(ObjPath + "Textures/MultiLut_atlas1.png ");
    colorGrading = scene->getEffect<ColorGrading>("ColorGrading");

    colorGrading->addLut(texture_LUT);

    glm::vec3 moonAlbedo = {1, 1, 1};
    glm::vec3 albedo = {0.8, 0.8, 0.8};
    glm::vec4 coefficients = {0.1, 1.0, 0.5, 60.0}; // kd = 1.0, ks = 0.5, ka = 0.1, s = 60.0

    // Moon object with displacement mapping
    Node *moon = scene->getNode("Moon");
    moon->frame()->translate(glm::vec3(0, 15, 0));
    moon->frame()->scale(glm::vec3(0.35f));
    moon->setModel(scene->m_Models.get<ModelGL>(ObjPath + "HighPolySphere.obj"));
    Texture2D *textureMoon = new Texture2D(ObjPath + "Textures/moon_heightMap.jpg");
    Texture2D *textureMoonHeightMap = new Texture2D(ObjPath + "Textures/moon_heightMap.jpg");

    DisplacementMaterial *matMoon = new DisplacementMaterial("Light");
    matMoon->setDiffuseTexture(textureMoon);
    matMoon->setHeightMap(textureMoonHeightMap);
    matMoon->setPhong(moonAlbedo, coefficients);
    moon->setMaterial(matMoon);

    // light node L
    Node *L = scene->getNode("L");
    L->frame()->translate(glm::vec3(0, 10, 0));
    L->setModel(scene->m_Models.get<ModelGL>(ObjPath + "Sphere.obj"));

    BaseMaterial *matLight = new BaseMaterial("Light");
    L->setMaterial(matLight);

    // Objets et texture de la boite
    Node *box = scene->getNode("Box");
    box->frame()->scale(glm::vec3(4.0f));
    box->setModel(scene->m_Models.get<ModelGL>(ObjPath + "Box.obj"));
    Texture2D *textureBox = new Texture2D(ObjPath + "Textures/Box_diff.jpg");
    Texture2D *textureBoxN = new Texture2D(ObjPath + "Textures/Box_nrm.jpg");

    //*************************** TP4 : Exemple d intégration avec le matériau TextureMaterial

    TextureMaterial *matBox = new TextureMaterial("matBox");
    matBox->setDiffuseTexture(textureBox);
    matBox->setNormalMap(textureBoxN);
    matBox->setPhong(albedo, coefficients);
    box->setMaterial(matBox);

    /********************************** */
    // (0,0.775,0) is the center of the pillar
    // Pillar
    Node *pillar = scene->getNode("Pillar");
    pillar->frame()->scale(glm::vec3(4.0f));
    pillar->setModel(scene->m_Models.get<ModelGL>(ObjPath + "Pillar.obj"));
    Texture2D *texturePillar = new Texture2D(ObjPath + "Textures/Pillar_diff.jpg");
    Texture2D *texturePillarN = new Texture2D(ObjPath + "Textures/Pillar_nrm.jpg");

    /*************************** TP4 : Exemple d intégration avec le matériau TextureMaterial*/
    TextureMaterial *matPillar = new TextureMaterial("matPillar");
    matPillar->setDiffuseTexture(texturePillar);
    matPillar->setNormalMap(texturePillarN);
    matPillar->setPhong(albedo, coefficients);
    pillar->setMaterial(matPillar);
    /********************************** */

    scene->getSceneNode()->adopt(L);
    scene->getSceneNode()->adopt(box);
    scene->getSceneNode()->adopt(pillar);
    scene->getSceneNode()->adopt(moon);

    setupEngine();
    return (true);
}

void EngineGL::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    myFBO->enable();

    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (unsigned int i = 0; i < allNodes->nodes.size(); i++)
        allNodes->nodes[i]->render();

    myFBO->disable();

    display->apply(myFBO, NULL);
    colorGrading->apply(myFBO, NULL);
    godRays->apply(myFBO, NULL);
}

void EngineGL::animate(const float elapsedTime) {
    // Animate each node
    for (unsigned int i = 0; i < allNodes->nodes.size(); i++) {
        allNodes->nodes[i]->animate(elapsedTime);
    }

    // Update Camera Buffer
    scene->camera()->updateBuffer();
}

void EngineGL::onWindowResize(int w, int h) {
    m_Width = w;
    m_Height = h;
    glViewport(0, 0, w, h);
    float ratio = (float)w / (float)h;

    scene->resizeViewport(w, h);
    scene->camera()->setPerspectiveProjection(glm::radians(45.0f), ratio, 1.0f, 2000.0f);
}

void EngineGL::setClearColor(glm::vec4 color) {
    glClearColor(color.x, color.y, color.z, color.w);
}

void EngineGL::displayInterface() {
    if (myFBO) {
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("FBOs")) {
                ImGui::MenuItem(myFBO->getName().c_str(), NULL, &(myFBO->show_interface));
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        myFBO->displayInterface();
    }
}

// Message callbck error for getting OpenGL problems
// All credits to https://github.com/fendevel/Guide-to-Modern-OpenGL-Functions/blob/master/README.md#gltexture
void message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const *message, void const *user_param) {
    auto const src_str = [source]() {
        switch (source) {
        case GL_DEBUG_SOURCE_API: return "API";
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
        case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
        case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
        case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
        case GL_DEBUG_SOURCE_OTHER: return "OTHER";
        default: return "";
        }
    }();

    auto const type_str = [type]() {
        switch (type) {
        case GL_DEBUG_TYPE_ERROR: return "ERROR";
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
        case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
        case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
        case GL_DEBUG_TYPE_MARKER: return "MARKER";
        case GL_DEBUG_TYPE_OTHER: return "OTHER";
        default: return "";
        }
    }();

    auto const severity_str = [severity]() {
        switch (severity) {
        case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
        case GL_DEBUG_SEVERITY_LOW: return "LOW";
        case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
        case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
        default: return "";
        }
    }();
    LOG_INFO << src_str << ", " << type_str << ", " << severity_str << ", " << id << ": " << message << '\n';
}

EngineGL::EngineGL(int width, int height) {
    m_Width = width;
    m_Height = height;

    myFBO = NULL;

    scene = Scene::getInstance();
    scene->resizeViewport(m_Width, m_Height);
}

EngineGL::~EngineGL() {}

void EngineGL::setupEngine() {
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
    glDebugMessageCallback(message_callback, nullptr);

    this->allNodes = new NodeCollector();
    allNodes->collect(scene->getRoot());

    LOG_INFO << "initialisation complete" << std::endl;
}