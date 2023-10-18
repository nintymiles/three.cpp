//
// Created by SeanR on 2023/10/18.
//

#ifndef THREE_CPP_MESH_STANDARD_MATERIAL_H
#define THREE_CPP_MESH_STANDARD_MATERIAL_H

#include "material.h"

class MeshStandardMaterial : public Material{
public:
    using sptr = std::shared_ptr<MeshStandardMaterial>;

    MeshStandardMaterial() :Material(){
        definesString.insert({"STANDARD", std::string()});
        type = "MeshStandardMaterial";
        name = "MeshStandardMaterial";

        shaderId = "physical";

        color.setHex(0xffffff);// = new Color(0xffffff); // diffuse
        roughness = 1.0f;
        metalness = 0.0f;

        //map = null;

        //lightMap = null;
        lightMapIntensity = 1.0f;

        //aoMap = null;
        aoMapIntensity = 1.0f;

        emissive.setHex(0x000000);
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

        //roughnessMap = null;

        //metalnessMap = null;

        //alphaMap = null;

        //envMap = null;
        envMapIntensity = 1.0f;

        refractionRatio = 0.98f;

        wireframe = false;
        wireframeLinewidth = 1;
        wireframeLinecap = "round";
        wireframeLinejoin = "round";

        skinning = false;
        morphTargets = false;
        morphNormals = false;

        vertexTangents = false;
    }

    MeshStandardMaterial(const MeshStandardMaterial& source) : Material(source) {
        definesString.insert({ "STANDARD",std::string() });

        color.copy(source.color);
        roughness = source.roughness;
        metalness = source.metalness;

        map = source.map;

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

        roughnessMap = source.roughnessMap;

        metalnessMap = source.metalnessMap;

        alphaMap = source.alphaMap;

        envMap = source.envMap;
        envMapIntensity = source.envMapIntensity;

        refractionRatio = source.refractionRatio;

        wireframe = source.wireframe;
        wireframeLinewidth = source.wireframeLinewidth;
        wireframeLinecap = source.wireframeLinecap;
        wireframeLinejoin = source.wireframeLinejoin;

        skinning = source.skinning;
        morphTargets = source.morphTargets;
        morphNormals = source.morphNormals;

        vertexTangents = source.vertexTangents;
    }

    static sptr create() {
        return std::make_shared<MeshStandardMaterial>();
    }

    virtual ~MeshStandardMaterial() = default;

    virtual MeshStandardMaterial* clone(){
        return new MeshStandardMaterial(*this);
    }

    virtual MeshStandardMaterial& copy(const MeshStandardMaterial& source){
        Material::copy(source);

        definesString.insert({ "STANDARD",std::string() });

        color.copy(source.color);
        roughness = source.roughness;
        metalness = source.metalness;

        map = source.map;

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

        roughnessMap = source.roughnessMap;

        metalnessMap = source.metalnessMap;

        alphaMap = source.alphaMap;

        envMap = source.envMap;
        envMapIntensity = source.envMapIntensity;

        refractionRatio = source.refractionRatio;

        wireframe = source.wireframe;
        wireframeLinewidth = source.wireframeLinewidth;
        wireframeLinecap = source.wireframeLinecap;
        wireframeLinejoin = source.wireframeLinejoin;

        skinning = source.skinning;
        morphTargets = source.morphTargets;
        morphNormals = source.morphNormals;

        vertexTangents = source.vertexTangents;

        return *this;
    }
};

#endif //THREE_CPP_MESH_STANDARD_MATERIAL_H
