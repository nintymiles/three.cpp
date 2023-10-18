//
// Created by SeanR on 2023/10/18.
//

#ifndef THREE_CPP_SPRITE_H
#define THREE_CPP_SPRITE_H

#include "object_3d.h"
class Sprite : public Object3D {
public:
    Vector2 center;
    using ptr = std::shared_ptr<Sprite>;
    Sprite(){}
};

#endif //THREE_CPP_SPRITE_H
