//
// Created by seanren on 11/20/2022.
//

#ifndef THREE_CPP_LIGHT_H
#define THREE_CPP_LIGHT_H

#include "object_3d.h"
#include "color.h"
#include "common_types.h"
#include "point_light_shadow.h"

#include <string>

class Light:public Object3D{
    public:
        using LightType = threecpp::LightType;

        Light(Color color,double intensity = 1.0):_color(color),_intensity(intensity){}

        Light(const Light &light):_color(light.color()),_intensity(light.intensity()){}

        Color color() const{return _color;}

        bool isLight() const{return _isLight;}

        void intensity(double intensity){_intensity=intensity;}

        virtual LightType type() const {return _type;}

        virtual double intensity() const {return _intensity;}

        virtual double distance() const {}

        virtual std::shared_ptr<LightShadow> shadow();

        virtual bool isAmbientLight() const {};

        virtual bool isDirectionalLight() const {};

        virtual bool isLightProbe() const {};

    protected:
        bool _isLight = true;
        LightType _type = LightType::Light;
        double _intensity = 1.0;
        Color _color;


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
