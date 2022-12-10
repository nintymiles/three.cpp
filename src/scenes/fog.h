//
// Created by SeanR on 2022/12/8.
//

#ifndef THREE_CPP_SRC_SCENES_FOG_H
#define THREE_CPP_SRC_SCENES_FOG_H

#include "color.h"

class Fog {
public:
    std::string name = "Fog";
    Color color;
    double near,far;
    Fog(Color color,double near = 1,double far = 1000):color(color),near(near),far(far){}
    Fog(const Fog& fog):color(fog.color),near(fog.near),far(fog.far){}

};


//import { Color } from '../math/Color.js';
//
//class Fog {
//
//    constructor( color, near = 1, far = 1000 ) {
//
//        this.name = '';
//
//        this.color = new Color( color );
//
//        this.near = near;
//        this.far = far;
//
//    }
//
//    clone() {
//
//        return new Fog( this.color, this.near, this.far );
//
//    }
//
//    toJSON( /* meta */ ) {
//
//        return {
//            type: 'Fog',
//            color: this.color.getHex(),
//            near: this.near,
//            far: this.far
//        };
//
//    }
//
//}



#endif //THREE_CPP_SRC_SCENES_FOG_H
