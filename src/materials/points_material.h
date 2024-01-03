//
// Created by SeanR on 2023/11/3.
//

#ifndef THREE_CPP_POINTS_MATERIAL_H
#define THREE_CPP_POINTS_MATERIAL_H

#include "material.h"

class PointsMaterial : public Material{
public:
    using sptr = std::shared_ptr<PointsMaterial>;

    PointsMaterial() :Material() {
        type = "PointsMaterial";
        name = "PointsMaterial";
        shaderId = "points";
        color.set(0xffffff);

        size = 1;
        sizeAttenuation = true;

        morphTargets = false;
    }

    PointsMaterial(const PointsMaterial& source) : Material(source) {
        color.copy(source.color);

        map = source.map;

        alphaMap = source.alphaMap;

        size = source.size;
        sizeAttenuation = source.sizeAttenuation;

        morphTargets = source.morphTargets;
    }

    virtual ~PointsMaterial() = default;

    virtual PointsMaterial* clone() {
        return new PointsMaterial(*this);
    }

    virtual PointsMaterial& copy(const PointsMaterial& source){
        Material::copy(source);

        color.copy(source.color);

        map = source.map;

        alphaMap = source.alphaMap;

        size = source.size;
        sizeAttenuation = source.sizeAttenuation;

        morphTargets = source.morphTargets;

        return *this;

    }

    static sptr create(){
        return std::make_shared<PointsMaterial>();
    }
};

#endif //THREE_CPP_POINTS_MATERIAL_H
