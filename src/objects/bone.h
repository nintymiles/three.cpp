//
// Created by SeanR on 2022/10/13.
//

#ifndef THREE_CPP_SRC_OBJECTS_BONE_H
#define THREE_CPP_SRC_OBJECTS_BONE_H

#include "object_3d.h"

class Bone : public Object3D {
public:
    using sptr = std::shared_ptr<Bone>;

    Bone() : Object3D(){
        type = "Bone";
    }
    Bone(const Bone& source) : Object3D(source) {}

    static sptr create() {
        return std::make_shared <Bone>();
    }
};

#endif //THREE_CPP_SRC_OBJECTS_BONE_H
