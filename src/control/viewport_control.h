//
// Created by SeanR on 11/23/2023.
//

#ifndef THREE_CPP_VIEWPORT_CONTROL_H
#define THREE_CPP_VIEWPORT_CONTROL_H

#include "vector4.h"

#include "common_types.h"

class ViewPortControl{
public:
    virtual bool update() = 0;

    virtual void sizeChanged(const Vector4 &screen) = 0;

    virtual void reset() = 0;

    virtual void keydown(threecpp::byte keyCode) = 0;

    virtual void keyup() = 0;

    virtual void mouseDown(unsigned button, float x, float y) = 0;

    virtual void mouseMove(float x, float y) = 0;

    virtual void mouseUp() = 0;

    virtual void mouseWheel(float delta) = 0;

//    virtual void touchstart();
//
//    virtual void touchmove();
//
//    virtual void touchend();
};

#endif //THREE_CPP_VIEWPORT_CONTROL_H
