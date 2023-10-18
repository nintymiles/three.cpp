//
// Created by SeanR on 2023/10/18.
//

#ifndef THREE_CPP_SPRITE_MATERIAL_H
#define THREE_CPP_SPRITE_MATERIAL_H
#include "material.h"

class SpriteMaterial : public Material {
public:
    SpriteMaterial() :Material() {
        type = "SpriteMaterial";
        name = "SpriteMaterial";
        shaderId = "sprite";

        color.setHex(0xffffff);// = new Color(0xffffff);

        //map = null;

        //alphaMap = null;

        rotation = 0;

        sizeAttenuation = true;

        transparent = true;
    }

    SpriteMaterial(const SpriteMaterial& source) :Material(source){
        color.copy(source.color);

        map = source.map;

        alphaMap = source.alphaMap;

        rotation = source.rotation;

        sizeAttenuation = source.sizeAttenuation;
    }

    virtual ~SpriteMaterial() = default;

    virtual SpriteMaterial* clone() {
        return new SpriteMaterial(*this);
    }

    virtual SpriteMaterial& copy(const SpriteMaterial& source) {
        Material::copy(source);

        color.copy(source.color);

        map = source.map;

        alphaMap = source.alphaMap;

        rotation = source.rotation;

        sizeAttenuation = source.sizeAttenuation;

        return *this;
    }
};

#endif //THREE_CPP_SPRITE_MATERIAL_H
