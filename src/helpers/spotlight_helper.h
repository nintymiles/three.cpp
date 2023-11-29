//
// Created by SeanR on 11/24/2023.
//

#ifndef THREE_CPP_SPOTLIGHTHELPER_H
#define THREE_CPP_SPOTLIGHTHELPER_H

#include "object_3d.h"

#include "light.h"
#include "color.h"
#include "matrix4.h"
#include "line.h"

class SpotlightHelper: public Object3D{
    Light::sptr light;
    Color color;
    LineSegments::sptr cone;

public:
    using sptr = std::shared_ptr<SpotlightHelper>;

    SpotlightHelper(Light::sptr light,Color color = Color{});

    static sptr create(Light::sptr light,Color color = Color{}){
        return std::make_shared<SpotlightHelper>(light,color);
    }

    SpotlightHelper& update();
};




#endif //THREE_CPP_SPOTLIGHTHELPER_H
