//
// Created by SeanR on 2023/10/18.
//

#ifndef THREE_CPP_MESH_DISTANCE_MATERIAL_H
#define THREE_CPP_MESH_DISTANCE_MATERIAL_H

#include "material.h"

class MeshDistanceMaterial : public Material{
public:
    MeshDistanceMaterial() : Material(){
        type = "MeshDistanceMaterial";
        name = "MeshDistanceMaterial";
        shaderId = "distanceRGBA";
        nearDistance = 1;
        farDistance = 1000;

        skinning = false;

        morphTargets = false;

        //map = null;

        //alphaMap = null;

        //displacementMap = null;

        displacementScale = 1;

        displacementBias = 0;

        fog = false;
    }

    MeshDistanceMaterial(const MeshDistanceMaterial& source) : Material(source) {
        shaderId = "distanceRGBA";
        referencePosition.copy(source.referencePosition);
        nearDistance = source.nearDistance;
        farDistance = source.farDistance;

        skinning = source.skinning;
        morphTargets = source.morphTargets;

        map = source.map;

        alphaMap = source.alphaMap;

        displacementMap = source.displacementMap;
        displacementScale = source.displacementScale;
        displacementBias = source.displacementBias;
    }

    virtual ~MeshDistanceMaterial() = default;

    MeshDistanceMaterial* clone(){
        return new MeshDistanceMaterial(*this);
    }

    MeshDistanceMaterial& copy(const MeshDistanceMaterial& source){
        Material::copy(source);
        referencePosition.copy(source.referencePosition);
        nearDistance = source.nearDistance;
        farDistance = source.farDistance;

        skinning = source.skinning;
        morphTargets = source.morphTargets;

        map = source.map;

        alphaMap = source.alphaMap;

        displacementMap = source.displacementMap;
        displacementScale = source.displacementScale;
        displacementBias = source.displacementBias;

        return *this;
    }
};

#endif //THREE_CPP_MESH_DISTANCE_MATERIAL_H
