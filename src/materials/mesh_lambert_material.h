//
// Created by SeanR on 2023/10/18.
//

#ifndef THREE_CPP_MESH_LAMBERT_MATERIAL_H
#define THREE_CPP_MESH_LAMBERT_MATERIAL_H

#include "material.h"

class MeshLambertMaterial : public Material{
public:
    MeshLambertMaterial() :Material(){
        type = "MeshLambertMaterial";
        name = "MeshLambertMaterial";

        shaderId = "lambert";

        color.setHex(0xffffff);

        map = nullptr;

        lightMap = nullptr;
        lightMapIntensity = 1.0f;

        aoMap = nullptr;
        aoMapIntensity = 1.0f;

        emissive.setHex(0x000000);
        emissiveIntensity = 1.0f;
        emissiveMap = nullptr;

        specularMap = nullptr;

        alphaMap = nullptr;

        envMap = nullptr;
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

    MeshLambertMaterial(const Color& color) : MeshLambertMaterial() {
        this->color =color;
    }

    MeshLambertMaterial(const MeshLambertMaterial& source) : Material(source) {
        shaderId = "lambert";
        color.copy(source.color);

        lightMap = source.lightMap;
        lightMapIntensity = source.lightMapIntensity;

        aoMap = source.aoMap;
        aoMapIntensity = source.aoMapIntensity;

        emissive.copy(source.emissive);
        emissiveMap = source.emissiveMap;
        emissiveIntensity = source.emissiveIntensity;

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

    virtual ~MeshLambertMaterial() = default;

    static sptr create() {
        return std::make_shared<MeshLambertMaterial>();
    }

    static sptr create(Color color) {
        return std::make_shared<MeshLambertMaterial>(color);
    }

    MeshLambertMaterial* clone() {
        return new MeshLambertMaterial(*this);
    }

    MeshLambertMaterial& copy(const MeshLambertMaterial& source){
        Material::copy(source);
        color.copy(source.color);


        lightMapIntensity = source.lightMapIntensity;

        aoMapIntensity = source.aoMapIntensity;

        emissive.copy(source.emissive);

        emissiveIntensity = source.emissiveIntensity;



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

#endif //THREE_CPP_MESH_LAMBERT_MATERIAL_H
