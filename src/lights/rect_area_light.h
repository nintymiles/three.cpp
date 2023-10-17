//
// Created by SeanR on 2023/9/2.
//

#ifndef THREE_CPP_RECT_AREA_LIGHT_H
#define THREE_CPP_RECT_AREA_LIGHT_H

#include "light.h"

class RectAreaLight : public Light {
public:
    using sptr = std::shared_ptr<RectAreaLight>;

    RectAreaLight();

    RectAreaLight(const Color& color, float intensity = 1, float width = std::numeric_limits<float>::quiet_NaN(), float height = std::numeric_limits<float>::quiet_NaN());

    static sptr create(const Color& color, float intensity = 1, float width = std::numeric_limits<float>::quiet_NaN(), float height = std::numeric_limits<float>::quiet_NaN()) {
        return std::make_shared<RectAreaLight>(color, intensity, width, height);
    }
    virtual RectAreaLight* clone() override {
        return new RectAreaLight(*this);
    }
    virtual RectAreaLight& copy(const RectAreaLight& source);

};

#endif //THREE_CPP_RECT_AREA_LIGHT_H
