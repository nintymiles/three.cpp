//
// Created by seanren on 11/20/2022.
//

#ifndef THREE_CPP_LIGHT_H
#define THREE_CPP_LIGHT_H

#include "object_3d.h"
#include "color.h"

#include <string>

class Light:public Object3D{
    public:
        bool isLight = true;
        std::string type = "Light";
        Color color;
        double intensity = 1.0;

        Light(Color color,double intensity = 1.0):color(color),intensity(intensity){}

        Light(const Light &light):color(light.color),intensity(light.intensity){}

};

//
//class Light extends Object3D {
//
//constructor( color, intensity = 1 ) {
//
//super();
//
//this.isLight = true;
//
//this.type = 'Light';
//
//this.color = new Color( color );
//this.intensity = intensity;
//
//}

//copy( source, recursive ) {
//
//super.copy( source, recursive );
//
//this.color.copy( source.color );
//this.intensity = source.intensity;
//
//return this;
//
//}
//
//toJSON( meta ) {
//
//        const data = super.toJSON( meta );
//
//        data.object.color = this.color.getHex();
//        data.object.intensity = this.intensity;
//
//        if ( this.groundColor !== undefined ) data.object.groundColor = this.groundColor.getHex();
//
//        if ( this.distance !== undefined ) data.object.distance = this.distance;
//        if ( this.angle !== undefined ) data.object.angle = this.angle;
//        if ( this.decay !== undefined ) data.object.decay = this.decay;
//        if ( this.penumbra !== undefined ) data.object.penumbra = this.penumbra;
//
//        if ( this.shadow !== undefined ) data.object.shadow = this.shadow.toJSON();
//
//        return data;
//
//}
//
//}



#endif //THREE_CPP_LIGHT_H
