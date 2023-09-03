//
// Created by seanren on 02/05/2023.
//

#ifndef THREE_CPP_LIGHT_PROBE_H
#define THREE_CPP_LIGHT_PROBE_H

#include "light.h"
#include "color.h"
#include "spherical_harmonics3.h"
#include <string>

class LightProbe : public Light {
public:
    LightProbe(ColorSptr color, double intensity):
        Light(color,intensity){}


    LightType type() const override{
        return _type;
    }

    SphericalHarmonics3Sptr sh(){
        return _sh;
    }

    bool isLightProbe() const override{
        return _isLightProbe;
    }

    virtual bool isHemisphereLightProbe() const;


private:
    LightType _type = LightType::LightProbe;
    bool _isLightProbe = true;
    SphericalHarmonics3Sptr _sh;


};

using LightProbeSptr = std::shared_ptr<LightProbe>;


#endif //THREE_CPP_LIGHT_PROBE_H
