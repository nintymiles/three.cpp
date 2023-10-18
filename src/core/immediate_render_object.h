//
// Created by SeanR on 2023/10/18.
//

#ifndef THREE_CPP_IMMEDIATE_RENDER_OBJECT_H
#define THREE_CPP_IMMEDIATE_RENDER_OBJECT_H

#include "object_3d.h"

class ImmediateRenderObject : public Object3D {
public:
    ImmediateRenderObject() : Object3D() {}
    ImmediateRenderObject(const ImmediateRenderObject& source) : Object3D(source) {}
    void render() {}
};

#endif //THREE_CPP_IMMEDIATE_RENDER_OBJECT_H
