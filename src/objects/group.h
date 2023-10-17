//
// Created by SeanR on 2022/10/13.
//

#ifndef THREE_CPP_SRC_OBJECTS_GROUP_H
#define THREE_CPP_SRC_OBJECTS_GROUP_H

#include "object_3d.h"

class Group : public Object3D {
public:
    std::vector<std::string> materialLibraries;
    using sptr = std::shared_ptr<Group>;

    Group() : Object3D(){
        this->type = "Group";
    }

    static sptr create() {
        return std::make_shared<Group>();
    }
};

//class Group : public Object3D {
//public:
//    std::string type = "Group";
//    bool isGroup = true;
//
//    Group() = default;
//
//};

#endif //THREE_CPP_SRC_OBJECTS_GROUP_H
