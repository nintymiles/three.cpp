//
// Created by SeanR on 2023/10/17.
//

#include "hemisphere_light.h"
#include "common_types.h"

HemisphereLight::HemisphereLight() : Light() {
    type = "HemisphereLight";
    lightType = LightType::HemisphereLight;
    position.copy(object3d::DefaultUp);
    groundColor.set(threecpp::Colors::white);
}

HemisphereLight::HemisphereLight(const Color& skyColor, const Color& groundColor, float intensity) : Light(skyColor,intensity) {

    type = "HemisphereLight";
    lightType = LightType::HemisphereLight;
    position.copy(object3d::DefaultUp);
    this->groundColor = groundColor;
}
HemisphereLight& HemisphereLight::copy(const HemisphereLight& source)
{
    Light::copy(source);
    type = source.type;
    position.copy(source.position);
    groundColor.copy(source.groundColor);
    return *this;
}
