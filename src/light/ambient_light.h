//
// Created by seanren on 02/05/2023.
//

#ifndef THREE_CPP_AMBIENT_LIGHT_H
#define THREE_CPP_AMBIENT_LIGHT_H

#include "light.h"
#include "color.h"
#include <string>

class AmbientLight : public Light {
public:
    AmbientLight(Color &color, double intensity):
        Light(color,intensity){}


    LightType type() const override{
        return _type;
    }

    bool isAmbientLight() const override{
        return _isAmbientLight;
    }


private:
    LightType _type = LightType::AmbientLight;
    bool _isAmbientLight = true;

};


#endif //THREE_CPP_AMBIENT_LIGHT_H
