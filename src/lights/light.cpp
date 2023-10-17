//
// Created by SeanR on 2023/10/17.
//
#include "light.h"

Light::Light() : Object3D() {
    isLight = true;
    receiveShadow = false;
    type = "Light";
    lightType = LightType::Light;
}

Light::Light(const Color& color,float intensity) :Object3D(), color(color), intensity(intensity) {
    isLight = true;
    receiveShadow = false;
    type = "Light";
    lightType = LightType::Light;
}

Light::Light(const Light& source) : Light() {
    type = source.type;
    receiveShadow = source.receiveShadow;

    color.copy(source.color);

    groundColor.copy(source.groundColor);

    intensity = source.intensity;


    angle = source.angle;

    exponent = source.exponent;

    decay = source.decay;

    penumbra = source.penumbra;

    //LightShadow::ptr shadow = nullptr;

    //Object3D::ptr target;

    sh.copy(source.sh);

    width = source.width;

    height = source.height;

    direction.copy(source.direction);

    distance = source.distance;

    coneCos = source.coneCos;


    halfWidth.copy(source.halfWidth);

    halfHeight;

}

Light& Light::copy(const Light& source) {
    Object3D::copy(source);

    type = source.type;
    receiveShadow = source.receiveShadow;

    lightType = source.lightType;

    color.copy(source.color);

    groundColor.copy(source.groundColor);

    intensity = source.intensity;


    angle = source.angle;

    exponent = source.exponent;

    decay = source.decay;

    penumbra = source.penumbra;

    //LightShadow::ptr shadow = nullptr;

    //Object3D::ptr target;

    sh.copy(source.sh);

    width = source.width;

    height = source.height;

    direction.copy(source.direction);

    distance = source.distance;

    coneCos = source.coneCos;

    halfWidth.copy(source.halfWidth);

    halfHeight;

    return *this;
}

