//
// Created by SeanR on 2023/9/2.
//

#ifndef THREE_CPP_HEMISPHERE_LIGHT_PROBE_H
#define THREE_CPP_HEMISPHERE_LIGHT_PROBE_H

#include "light_probe.h"
#include "color.h"

class HemisphereLightProbe:public LightProbe{

    HemisphereLightProbe(ColorSptr skyColor,ColorSptr groundColor,double intensity = 1): LightProbe(skyColor,intensity){
       Vector3 sky(skyColor->r,skyColor->g,skyColor->b);
       Vector3 ground(groundColor->r,groundColor->g,groundColor->b);

       double c0 = std::sqrt(Number::PI);
       double c1 = c0 * std::sqrt(.75);

       LightProbe::sh()->coefficients[0]->copy(sky).add(ground).multiplyScalar(c0);
       LightProbe::sh()->coefficients[1]->copy(sky).add(ground).multiplyScalar(c1);
    }

    bool isHemisphereLightProbe() const override{
        return true;
    }
};

#endif //THREE_CPP_HEMISPHERE_LIGHT_PROBE_H
