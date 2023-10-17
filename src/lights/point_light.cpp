//
// Created by SeanR on 2023/10/17.
//
#include "point_light.h"

PointLight::PointLight() : Light() {
    type = "PointLight";
    lightType = LightType::PointLight;
    distance = 0;
    decay = 1;
    this->shadow = PointLightShadow::create();
}

PointLight::PointLight(const Color& color, float intensity, float distance, float decay) : Light(color, intensity) {
    type = "PointLight";
    lightType = LightType::PointLight;
    this->distance = !std::isnan(distance) ? distance : 0;
    this->decay = !std::isnan(decay) ? decay : 1;
    this->shadow = PointLightShadow::create();
}

PointLight& PointLight::copy(const PointLight& source){
    Light::copy(source);
    this->distance = source.distance;
    this->decay = source.decay;



    if (source.shadow != nullptr) {
        shadow.reset();
        shadow->copy(*source.shadow);
    }

    return *this;
}