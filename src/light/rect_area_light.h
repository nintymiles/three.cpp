//
// Created by SeanR on 2023/9/2.
//

#ifndef THREE_CPP_RECT_AREA_LIGHT_H
#define THREE_CPP_RECT_AREA_LIGHT_H

#include "light.h"

class RectAreaLight:public Light{
public:
    RectAreaLight(ColorSptr color,double intensity,double width = 10.0,double height = 10.0): Light(color,intensity),width(width),height(height){}

    const double power() const{
        // compute the light's luminous power (in lumens) from its intensity (in nits)
        return this->_intensity * this->width * this->height * Number::PI;
    }

    RectAreaLight& power( double power ) {
        // set the light's intensity (in nits) from the desired luminous power (in lumens)
        this->_intensity = power / ( this->width * this->height * Number::PI );
    }

private:
    bool isRectAreaLight = true;
    std::string type = "RectAreaLight";
    double width,height;

};

#endif //THREE_CPP_RECT_AREA_LIGHT_H
