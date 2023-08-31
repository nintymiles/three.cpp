//
// Created by SeanRen on 8/31/2023.
//

#ifndef THREE_CPP_AMBIENT_LIGHT_PROBE_H
#define THREE_CPP_AMBIENT_LIGHT_PROBE_H

#include "light_probe.h"
#include "color.h"

class AmbientLightProbe:public LightProbe{
public:
    AmbientLightProbe(ColorSptr color,double intensity = 1): LightProbe(color,intensity) {
        // without extra factor of PI in the shader, would be 2 / Math.sqrt( Math.PI );
        LightProbe::sh()->coefficients[0]->set(color->r,color->g,color->b).multiplyScalar(2*std::sqrt(Number::PI));
    }

    bool isAmbientLightProbe() const{
        return _isAmbientLightProbe;
    }
private:
    bool _isAmbientLightProbe = true;
};

#endif //THREE_CPP_AMBIENT_LIGHT_PROBE_H
