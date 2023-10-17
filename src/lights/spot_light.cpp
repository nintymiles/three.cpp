//
// Created by SeanR on 2023/10/17.
//
#include "spot_light.h"

SpotLight::SpotLight() : Light() {
    type = "SpotLight";
    lightType = LightType::SpotLight;
    this->position.copy(object3d::DefaultUp);
    this->updateMatrix();
    this->target = std::make_shared<Object3D>();
    this->shadow = SpotLightShadow::create();

}

SpotLight::SpotLight(const Color& color, float intensity, float distance, float angle, float penumbra, float decay) : Light(color,intensity) {
    type = "SpotLight";
    lightType = LightType::SpotLight;
    this->position.copy(object3d::DefaultUp);
    this->updateMatrix();
    this->target = std::make_shared<Object3D>();
    this->shadow = SpotLightShadow::create();
    this->distance = !std::isnan(distance) ? distance : 0;
    this->angle = !std::isnan(angle) ? angle : (float)(M_PI / 3);
    this->penumbra = !std::isnan(penumbra) ? penumbra : 0;
    this->decay = !std::isnan(decay) ? decay : 1;

}

SpotLight& SpotLight::copy(const SpotLight& source) {
    Light::copy(source);


    this->shadow.reset();
    if (source.shadow != nullptr) {
        this->shadow = SpotLightShadow::create();
        this->shadow->copy(*source.shadow);
    }

    this->target.reset();
    if (source.target != nullptr)
        this->target = Object3D::ptr(source.target->clone());

    return *this;
}
