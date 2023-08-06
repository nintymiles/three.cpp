//
// Created by seanren on 8/6/2023.
//

#ifndef THREE_CPP_HEMISPHERE_LIGHT_H
#define THREE_CPP_HEMISPHERE_LIGHT_H

#include "light.h"
#include "color.h"

class HemisphereLight:public Light{
public:
    HemisphereLight(ColorSptr skyColor,ColorSptr groundColor,double intensity): Light(skyColor,intensity){
        position->copy(*Object3D::default_up);
        updateMatrix();
        _groundColor->copy(*groundColor);
    }

    bool isHemisphereLight(){return true;}

    LightType type(){return threecpp::LightType::HemisphereLight;}

    HemisphereLight& copy(const HemisphereLight& source,bool recursive){
        _groundColor->copy(*source.groundColor());
    }

    const ColorSptr groundColor() const {return _groundColor;}


private:
    ColorSptr _groundColor;

};


#endif //THREE_CPP_HEMISPHERE_LIGHT_H
