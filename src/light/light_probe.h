//
// Created by seanren on 02/05/2023.
//

#ifndef THREE_CPP_LIGHT_PROBE_H
#define THREE_CPP_LIGHT_PROBE_H

#include "light.h"
#include "color.h"
#include <string>

class LightProbe : public Light {
public:
    LightProbe(Color &color, double intensity):
        Light(color,intensity){}


    LightType type() const override{
        return _type;
    }

    bool isLightProbe() const override{
        return _isLightProbe;
    }


private:
    LightType _type = LightType::LightProbe;
    bool _isLightProbe = true;

};


#endif //THREE_CPP_LIGHT_PROBE_H
