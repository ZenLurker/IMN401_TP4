

#ifndef _DisplacementMaterial_
#define _DisplacementMaterial_

#include "MaterialGL.h"
#include "Texture2D.h"

class DisplacementMaterial : public MaterialGL {
public:
    // Attributs

    // Constructeur-Destructeur

    /**
        Constructeur de la classe � partir du nom du mat�riau
        @param name : nom du mat�riau
    */
    DisplacementMaterial(std::string name = "");

    /**
        Destructeur de la classe
    */
    ~DisplacementMaterial();

    // M�thodes

    /**
        M�thode virtuelle qui est appel�e pour faire le rendu d'un objet en utilisant ce mat�riau
        @param o : Node/Objet pour lequel on veut effectuer le rendu
    */
    virtual void render(Node *o);

    /**
        M�thode virtuelle qui est appel�e pour modifier une valeur d'un param�tre n�cessaire pour le rendu
        @param o : Node/Objet concern� par le rendu
        @param elapsedTime : temps
    */
    virtual void animate(Node *o, const float elapsedTime);

    void updatePhong();

    virtual void displayInterface();

    void setHeightMap(Texture2D *heightTexture);

    void setDiffuseTexture(Texture2D *textureDiffuse);

    void setPhong(glm::vec3 albedo, glm::vec4 coefficient);

protected:
    GLProgram *vp;
    GLProgram *fp;

    PhongParams param;

    GLint l_View, l_Proj, l_Model, l_PosLum, l_PosCam, l_Phong, l_Albedo, l_displacementScale;

    Texture2D *texture_diffuse;
    Texture2D *height_texture;
};

#endif