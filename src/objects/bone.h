//
// Created by SeanR on 2022/10/13.
//

#ifndef THREE_CPP_SRC_OBJECTS_BONE_H
#define THREE_CPP_SRC_OBJECTS_BONE_H

#include "object_3d.h"

class Bone : public Object3D {
public:
    std::string type = "Bone";
    bool isBone = true;

    Bone() = default;

};

#endif //THREE_CPP_SRC_OBJECTS_BONE_H
