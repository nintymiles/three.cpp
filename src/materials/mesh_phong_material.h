//
// Created by SeanR on 2023/10/18.
//

#ifndef THREE_CPP_MESH_PHONG_MATERIAL_H
#define THREE_CPP_MESH_PHONG_MATERIAL_H

#include "material.h"

class MeshPhongMaterial :public Material{
public:
    MeshPhongMaterial() : Material(){
        type = "MeshPhongMaterial";
        name = "MeshPhongMaterial";
        shaderId = "phong";
        color.setHex(0xffffff);// = new Color(0xffffff); // diffuse
        specular.setHex(0x111111);// = new Color(0x111111);
        shininess = 30;

        //map = null;

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

        //envMap = null;
        combine = Combine::MultiplyOperation;
        reflectivity = 1;
        refractionRatio = 0.98f;

        wireframe = false;
        wireframeLinewidth = 1;
        wireframeLinecap = "round";
        wireframeLinejoin = "round";

        skinning = false;
        morphTargets = false;
        morphNormals = false;

    }

    MeshPhongMaterial(Color color) : MeshPhongMaterial() {
        this->color.copy(color);
    }

    MeshPhongMaterial(const MeshPhongMaterial& source) : Material(source) {
        type = "MeshPhongMaterial";
        name = "MeshPhongMaterial";
        shaderId = "phong";
        color.copy(source.color);

        specular.copy(source.specular);
        shininess = source.shininess;

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

        specularMap = source.specularMap;

        alphaMap = source.alphaMap;

        envMap = source.envMap;
        combine = source.combine;
        reflectivity = source.reflectivity;
        refractionRatio = source.refractionRatio;

        wireframe = source.wireframe;
        wireframeLinewidth = source.wireframeLinewidth;
        wireframeLinecap = source.wireframeLinecap;
        wireframeLinejoin = source.wireframeLinejoin;

        skinning = source.skinning;
        morphTargets = source.morphTargets;
        morphNormals = source.morphNormals;
    }

    virtual ~MeshPhongMaterial() = default;

    static sptr create() {
        return std::make_shared<MeshPhongMaterial>();
    }

    static sptr create(Color color) {
        return std::make_shared<MeshPhongMaterial>(color);
    }

    MeshPhongMaterial* clone(){
        return new MeshPhongMaterial(*this);
    }

    std::shared_ptr<Material> cloneShared(){
        return std::make_shared<MeshPhongMaterial>(*this);
    }

    MeshPhongMaterial& copy(const MeshPhongMaterial& source){
        Material::copy(source);

        color.copy(source.color);

        specular.copy(source.specular);
        shininess = source.shininess;

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

        specularMap = source.specularMap;

        alphaMap = source.alphaMap;

        envMap = source.envMap;
        combine = source.combine;
        reflectivity = source.reflectivity;
        refractionRatio = source.refractionRatio;

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

#endif //THREE_CPP_MESH_PHONG_MATERIAL_H
