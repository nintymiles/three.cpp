//
// Created by SeanR on 2023/10/18.
//

#ifndef THREE_CPP_MESH_TOON_MATERIAL_H
#define THREE_CPP_MESH_TOON_MATERIAL_H

#include "material.h"

class MeshToonMaterial : public Material{
public:
    MeshToonMaterial() :Material(){
        definesString.insert({"TOON", std::string() });;
        type = "MeshToonMaterial";
        name = "MeshToonMaterial";

        shaderId = "toon";

        color.setHex(0xffffff);// = new Color(0xffffff);
        specular.setHex(0x111111);// = new Color(0x111111);
        shininess = 30;

        //map = null;
        //gradientMap = null;

        //lightMap = null;
        lightMapIntensity = 1.0f;

        //aoMap = null;
        aoMapIntensity = 1.0f;

        emissive.setHex(0x000000);// = new Color(0x000000);
        emissiveIntensity = 1.0f;
        //emissiveMap = null;

        //bumpMap = null;
        bumpScale = 1;

        //normalMap = null;
        normalMapType = NormalMapTypes::TangentSpaceNormalMap;
        normalScale.set(1.0f, 1.0f);// = new Vector2(1, 1);

        //displacementMap = null;
        displacementScale = 1;
        displacementBias = 0;

        //specularMap = null;

        //alphaMap = null;

        wireframe = false;
        wireframeLinewidth = 1;
        wireframeLinecap = "round";
        wireframeLinejoin = "round";

        skinning = false;
        morphTargets = false;
        morphNormals = false;

    }

    MeshToonMaterial(const MeshToonMaterial& source) : Material(source) {

        color.copy(source.color);
        specular.copy(source.specular);
        shininess = source.shininess;

        map = source.map;
        gradientMap = source.gradientMap;

        lightMap = source.lightMap;
        lightMapIntensity = source.lightMapIntensity;

        aoMap = source.aoMap;
        aoMapIntensity = source.aoMapIntensity;

        emissive.copy(source.emissive);
        emissiveMap = source.emissiveMap;
        emissiveIntensity = source.emissiveIntensity;

        bumpMap = source.bumpMap;
        bumpScale = source.bumpScale;

        normalMap = source.normalMap;
        normalMapType = source.normalMapType;
        normalScale.copy(source.normalScale);

        displacementMap = source.displacementMap;
        displacementScale = source.displacementScale;
        displacementBias = source.displacementBias;

        specularMap = source.specularMap;

        alphaMap = source.alphaMap;

        wireframe = source.wireframe;
        wireframeLinewidth = source.wireframeLinewidth;
        wireframeLinecap = source.wireframeLinecap;
        wireframeLinejoin = source.wireframeLinejoin;

        skinning = source.skinning;
        morphTargets = source.morphTargets;
        morphNormals = source.morphNormals;
    }
    virtual ~MeshToonMaterial() = default;

    virtual MeshToonMaterial* clone(){
        return new MeshToonMaterial(*this);
    }

    virtual MeshToonMaterial& copy(const MeshToonMaterial& source){
        Material::copy(source);

        color.copy(source.color);
        specular.copy(source.specular);
        shininess = source.shininess;

        map = source.map;
        gradientMap = source.gradientMap;

        lightMap = source.lightMap;
        lightMapIntensity = source.lightMapIntensity;

        aoMap = source.aoMap;
        aoMapIntensity = source.aoMapIntensity;

        emissive.copy(source.emissive);
        emissiveMap = source.emissiveMap;
        emissiveIntensity = source.emissiveIntensity;

        bumpMap = source.bumpMap;
        bumpScale = source.bumpScale;

        normalMap = source.normalMap;
        normalMapType = source.normalMapType;
        normalScale.copy(source.normalScale);

        displacementMap = source.displacementMap;
        displacementScale = source.displacementScale;
        displacementBias = source.displacementBias;

        specularMap = source.specularMap;

        alphaMap = source.alphaMap;

        wireframe = source.wireframe;
        wireframeLinewidth = source.wireframeLinewidth;
        wireframeLinecap = source.wireframeLinecap;
        wireframeLinejoin = source.wireframeLinejoin;

        skinning = source.skinning;
        morphTargets = source.morphTargets;
        morphNormals = source.morphNormals;

        return *this;
    }

};

#endif //THREE_CPP_MESH_TOON_MATERIAL_H
