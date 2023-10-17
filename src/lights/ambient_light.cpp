//
// Created by SeanR on 2023/10/17.
//

#include "ambient_light.h"

AmbientLight::AmbientLight() : Light() {
    type = "AmbientLight";
    lightType = LightType::AmbientLight;
}

AmbientLight::AmbientLight(const Color& color, float intensity) :Light(color, intensity) {
    type = "AmbientLight";
    lightType = LightType::AmbientLight;
}

AmbientLight::AmbientLight(const AmbientLight& source) : Light(source) {
    lightType = LightType::AmbientLight;
}

AmbientLight& AmbientLight::copy(const AmbientLight& source){
    Light::copy(source);
    return *this;
}