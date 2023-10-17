//
// Created by SeanR on 2023/10/17.
//
#include "rect_area_light.h"

RectAreaLight::RectAreaLight() : Light() {
    type = "RectAreaLight";
    lightType = LightType::RectAreaLight;
    width = 10;
    height = 10;
}

RectAreaLight::RectAreaLight(const Color& color, float intensity, float width, float height) : Light(color, intensity) {
    type = "RectAreaLight";
    lightType = LightType::RectAreaLight;
    this->width = !std::isnan(width) ? width : 10;
    this->height = !std::isnan(height) ? height : 10;
}

RectAreaLight& RectAreaLight::copy(const RectAreaLight& source)
{
    Light::copy(source);
    this->width = source.width;
    this->height = source.height;

    return *this;
}
