//
// Created by SeanR on 2024/1/14.
//

#include "ray_caster.h"

float ascSort( std::shared_ptr<IntersectionData> a,std::shared_ptr<IntersectionData> b ) {

    return a->distance - b->distance;
}

void intersectObject( Object3D::sptr object, Raycaster& raycaster, std::vector<std::shared_ptr<IntersectionData>>& intersects, bool recursive ) {

    if ( object->layers.test( *raycaster.layers ) ) {
        object->raycast( raycaster, intersects );
    }

    if (recursive == true) {

        auto children = object->children;

        for (size_t i = 0, l = children.size(); i < l; i++) {

            intersectObject(children[i], raycaster, intersects, true);
        }

    }
}