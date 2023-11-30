//
// Created by SeanR on 11/30/2023.
//

#ifndef THREE_CPP_MESH_NORMAL_MATERIAL_H
#define THREE_CPP_MESH_NORMAL_MATERIAL_H

#include "material.h"

class MeshNormalMaterial :public Material{
public:
    MeshNormalMaterial() : Material(){
        type = "MeshNormalMaterial";
        name = "MeshNormalMaterial";
        shaderId = "normal";

        bumpMap = nullptr;
        bumpScale = 1;

        normalMap = nullptr;
        normalMapType = NormalMapTypes::TangentSpaceNormalMap;
        normalScale.set(1.0f, 1.0f);// = new Vector2(1, 1);

        displacementMap = nullptr;
        displacementScale = 1;
        displacementBias = 0;

        wireframe = false;
        wireframeLinewidth = 1;
//        wireframeLinecap = "round";
//        wireframeLinejoin = "round";

        flatShading = false;

    }

    MeshNormalMaterial(Color color) : MeshNormalMaterial() {
        this->color.copy(color);
    }

    MeshNormalMaterial(const MeshNormalMaterial& source) : Material(source) {
        type = source.type;//"MeshNormalMaterial";
        name = source.name;//"MeshNormalMaterial";
        shaderId = source.shaderId;//"normal";

        bumpMap = source.bumpMap;
        bumpScale = source.bumpScale;

        normalMap = source.normalMap;
        normalMapType = source.normalMapType;
        normalScale= source.normalScale;

        displacementMap = source.displacementMap;
        displacementScale = source.displacementScale;
        displacementBias = source.displacementBias;

        wireframe = source.wireframe;
        wireframeLinewidth = source.wireframeLinewidth;
//        wireframeLinecap = "round";
//        wireframeLinejoin = "round";

        flatShading = source.flatShading;
    }

    virtual ~MeshNormalMaterial() = default;

    static sptr create() {
        return std::make_shared<MeshNormalMaterial>();
    }

//    static sptr create(Color color) {
//        return std::make_shared<MeshNormalMaterial>(color);
//    }

    MeshNormalMaterial* clone(){
        return new MeshNormalMaterial(*this);
    }

    MeshNormalMaterial& copy(const MeshNormalMaterial& source){
        Material::copy(source);

        type = source.type;//"MeshNormalMaterial";
        name = source.name;//"MeshNormalMaterial";
        shaderId = source.shaderId;//"normal";

        bumpMap = source.bumpMap;
        bumpScale = source.bumpScale;

        normalMap = source.normalMap;
        normalMapType = source.normalMapType;
        normalScale= source.normalScale;

        displacementMap = source.displacementMap;
        displacementScale = source.displacementScale;
        displacementBias = source.displacementBias;

        wireframe = source.wireframe;
        wireframeLinewidth = source.wireframeLinewidth;
//        wireframeLinecap = "round";
//        wireframeLinejoin = "round";

        flatShading = source.flatShading;

        return *this;
    }

};

#endif //THREE_CPP_MESH_NORMAL_MATERIAL_H
