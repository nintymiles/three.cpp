//
// Created by Willie on 2022/10/5.
//

#include "frustum.h"
#include "box3.h"

Vector3 _frustum_vector;

namespace frustum {
Sphere _sphere;
Vector3 _vector;
}
using namespace frustum;

bool Frustum::intersectsBox( Box3& box ) {
    //const planes = this.planes;
    Plane *planes = this->planes;

    for ( int i = 0; i < 6; i ++ ) {
        Plane& plane = planes[ i ];

        // corner at max distance
        _frustum_vector.x = plane.normal.x > 0 ? box.max.x : box.min.x;
        _frustum_vector.y = plane.normal.y > 0 ? box.max.y : box.min.y;
        _frustum_vector.z = plane.normal.z > 0 ? box.max.z : box.min.z;

        if ( plane.distanceToPoint(_frustum_vector ) < 0 ) {
            return false;
        }
    }

    return true;
}

bool Frustum::intersectsObject(Object3D& object){
    auto geometry = object.geometry;

    if (geometry->boundingSphere.isEmpty() == true)
        geometry->computeBoundingSphere();

    _sphere.copy(geometry->boundingSphere).applyMatrix4(object.matrixWorld);

    return this->intersectsSphere(_sphere);
}