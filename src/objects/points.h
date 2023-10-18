//
// Created by SeanR on 2023/10/18.
//

#ifndef THREE_CPP_POINTS_H
#define THREE_CPP_POINTS_H

#include "object_3d.h"

class Points :public Object3D{

public:
    using sptr = std::shared_ptr<Points>;
    
    Points() :Object3D(){
        this->type = "Points";
        this->isPoints = true;
    }
    
    Points(const Geometry::sptr& geometry, const std::initializer_list<Material::sptr>& materials) : Object3D(geometry, materials) {}

    Points(const Geometry::sptr& geometry, const std::vector<Material::sptr>& materials) : Object3D(geometry, materials) {}

    Points(const Geometry::sptr& geometry, const Material::sptr& material) : Object3D(geometry, material){
        this->type = "Points";
        this->isPoints = true;
    }

    static sptr create() {
        return std::make_shared<Points>();
    }

    static sptr create(const Geometry::sptr& geometry, const Material::sptr& material){
        return std::make_shared<Points>(geometry, material);
    }

    static sptr create(const Geometry::sptr& geometry, const std::vector<Material::sptr>& material){
        return std::make_shared<Points>(geometry, material);
    }

    static sptr create(const Geometry::sptr& geometry, const std::initializer_list<Material::sptr>& material){
        return std::make_shared<Points>(geometry, material);
    }

};

#endif //THREE_CPP_POINTS_H
