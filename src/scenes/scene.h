//
// Created by SeanR on 2022/10/19.
//

#ifndef THREE_CPP_SRC_SCENES_SCENE_H
#define THREE_CPP_SRC_SCENES_SCENE_H

#include "object_3d.h"

#include "color.h"
#include "fog.h"
#include "materials/material.h"

struct Scene: public Object3D{
    bool isScene = true;
    std::string type = "Scene";
    bool autoUpdate = true; // checked by the renderer

//    this.background = null;
    std::shared_ptr<Color> background;
//    this.environment = null;
//    this.fog = null;
    std::shared_ptr<Fog> fog;

//    this.overrideMaterial = null;
    std::shared_ptr<Material> overrideMaterial;

    Scene(std::shared_ptr<Color> background,std::shared_ptr<Fog> fog,std::shared_ptr<Material> overrideMaterial,bool autoUpdate)
                                :background(background),fog(fog),overrideMaterial(overrideMaterial),autoUpdate(autoUpdate){}


};

//copy( source, recursive ) {
//
//super.copy( source, recursive );
//
//if ( source.background !== null ) this.background = source.background.clone();
//if ( source.environment !== null ) this.environment = source.environment.clone();
//if ( source.fog !== null ) this.fog = source.fog.clone();
//
//if ( source.overrideMaterial !== null ) this.overrideMaterial = source.overrideMaterial.clone();
//
//this.autoUpdate = source.autoUpdate;
//this.matrixAutoUpdate = source.matrixAutoUpdate;
//
//return this;
//
//}
//
//toJSON( meta ) {
//
//    const data = super.toJSON( meta );
//
//    if ( this.fog !== null ) data.object.fog = this.fog.toJSON();
//
//    return data;
//
//}
//
//}



#endif //THREE_CPP_SRC_SCENES_SCENE_H
