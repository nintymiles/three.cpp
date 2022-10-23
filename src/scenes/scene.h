//
// Created by SeanR on 2022/10/19.
//

#ifndef THREE_CPP_SRC_SCENES_SCENE_H
#define THREE_CPP_SRC_SCENES_SCENE_H

#include "object_3d.h"

class Scene: public Object3D{
public:
    bool isScene = true;
    std::string type = "Scene";
    bool autoUpdate = true; // checked by the renderer

//    this.background = null;
//    this.environment = null;
//    this.fog = null;
//
//    this.overrideMaterial = null;

    Scene() = default;


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
