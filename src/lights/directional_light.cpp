//
// Created by SeanR on 2023/10/17.
//

#include "directional_light.h"

DirectionalLight::DirectionalLight() : Light() {

    type = "DirectionalLight";
    position.copy(object3d::DefaultUp);
    target = Object3D::create();
    lightType = LightType::DirectionalLight;
    this->shadow = DirectionalLightShadow::create();
}

DirectionalLight::DirectionalLight(const Color& color, float intensity) :Light(color, intensity) {
    type = "DirectionalLight";
    position.copy(object3d::DefaultUp);
    target = Object3D::create();
    lightType = LightType::DirectionalLight;
    this->shadow = DirectionalLightShadow::create();
}

DirectionalLight::DirectionalLight(const DirectionalLight& source) : Light(source) {
    type = source.type;
    position.copy(source.position);
    target = Object3D::create();

    target->copy(*source.target);

    lightType = LightType::DirectionalLight;
    this->shadow = DirectionalLightShadow::create();
}

DirectionalLight& DirectionalLight::copy(const DirectionalLight& source){
    Light::copy(source);

    return *this;
}
