//
// Created by seanren on 11/20/2022.
//

#ifndef THREE_CPP_POINT_LIGHT_H
#define THREE_CPP_POINT_LIGHT_H

#include "light.h"
#include "point_light_shadow.h"
#include "color.h"
#include <string>

class PointLight : public Light {
public:
    std::string type = "PointLight";
    bool isPointLight = true;

    double distance = distance;
    int decay = 1; // for physically correct lights, should be 2.

    PointLightShadow shadow;

    PointLight( Color &color, double intensity, double distance = 0, double decay = 1 ):
        Light(color,intensity),decay(decay),distance(distance){}

    double power(){
        // compute the light's luminous power (in lumens) from its intensity (in candela)
        // for an isotropic light source, luminous power (lm) = 4 π luminous intensity (cd)
        return intensity * 4 * Number::PI;
    }

    PointLight& power(double power){
        // set the light's intensity (in candela) from the desired luminous power (in lumens)
        this->intensity = power / ( 4 * Number::PI );
        return *this;
    }

//    dispose() {
//
//        this.shadow.dispose();
//
//    }
//
//    copy( source ) {
//
//        super.copy( source );
//
//        this.distance = source.distance;
//        this.decay = source.decay;
//
//        this.shadow = source.shadow.clone();
//
//        return this;
//
//    }

};


#endif //THREE_CPP_POINT_LIGHT_H
