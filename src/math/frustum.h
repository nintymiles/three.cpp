//
// Created by SeanRen on 2022/10/5.
//

#ifndef THREE_CPP_FRUSTUM_H
#define THREE_CPP_FRUSTUM_H

#include "object_3d.h"
#include "vector3.h"
#include "sphere.h"
#include "plane.h"
#include "matrix4.h"
#include "sprite.h"

namespace frustum {
    Sphere _sphere;
    Vector3 _vector;
}
using namespace frustum;

class Frustum {
public:
    Plane planes[6];

    //可在构造函数初始化列表中初始化数组
    Frustum(Plane p0 = Plane(),Plane p1 = Plane(),Plane p2 = Plane(),Plane p3 = Plane(),Plane p4 = Plane(),Plane p5 = Plane()):planes{p0,p1,p2,p3,p4,p5}{};

    Frustum(const Frustum& f):planes{f.planes[0],f.planes[1],f.planes[2],f.planes[3],f.planes[4],f.planes[5]}{}

    Frustum& set( Plane p0, Plane p1, Plane p2, Plane p3, Plane p4, Plane p5 ) {
        planes[ 0 ].copy( p0 );
        planes[ 1 ].copy( p1 );
        planes[ 2 ].copy( p2 );
        planes[ 3 ].copy( p3 );
        planes[ 4 ].copy( p4 );
        planes[ 5 ].copy( p5 );

        return *this;
    }

    Frustum& copy( const Frustum& frustum ) {
        for ( int i = 0; i < 6; i ++ ) {
            planes[ i ].copy( frustum.planes[ i ] );
        }

        return *this;
    }

    Frustum& setFromProjectionMatrix( Matrix4& m ) {
        double *me = m.elements;
        const double me0 = me[ 0 ], me1 = me[ 1 ], me2 = me[ 2 ], me3 = me[ 3 ];
        const double me4 = me[ 4 ], me5 = me[ 5 ], me6 = me[ 6 ], me7 = me[ 7 ];
        const double me8 = me[ 8 ], me9 = me[ 9 ], me10 = me[ 10 ], me11 = me[ 11 ];
        const double me12 = me[ 12 ], me13 = me[ 13 ], me14 = me[ 14 ], me15 = me[ 15 ];

        planes[ 0 ].setComponents( me3 - me0, me7 - me4, me11 - me8, me15 - me12 ).normalize();
        planes[ 1 ].setComponents( me3 + me0, me7 + me4, me11 + me8, me15 + me12 ).normalize();
        planes[ 2 ].setComponents( me3 + me1, me7 + me5, me11 + me9, me15 + me13 ).normalize();
        planes[ 3 ].setComponents( me3 - me1, me7 - me5, me11 - me9, me15 - me13 ).normalize();
        planes[ 4 ].setComponents( me3 - me2, me7 - me6, me11 - me10, me15 - me14 ).normalize();
        planes[ 5 ].setComponents( me3 + me2, me7 + me6, me11 + me10, me15 + me14 ).normalize();

        return *this;
    }

    bool intersectsObject(Object3D& object){
        auto geometry = object.geometry;

        if (geometry->boundingSphere.isEmpty() == true)
            geometry->computeBoundingSphere();

        _sphere.copy(geometry->boundingSphere).applyMatrix4(object.matrixWorld);

        return this->intersectsSphere(_sphere);
    }

    bool intersectsSprite(const Sprite& sprite){
        return false;
    }

//    intersectsSprite( sprite ) {
//
//            _sphere.center.set( 0, 0, 0 );
//            _sphere.radius = 0.7071067811865476;
//            _sphere.applyMatrix4( sprite.matrixWorld );
//
//            return this.intersectsSphere( _sphere );
//
//    }

    bool intersectsSphere( Sphere& sphere ) {
        //可以使用指针赋值，实现数组的别名（引用）
        //Plane* planes = this->planes;
        //数组的引用
        Plane (&planes)[6] = this->planes;
        //gsl::span<Plane> planes = gsl::span<Plane>(this->planes);
        Vector3& center = sphere.center;
        const double negRadius = - sphere.radius;

        for ( int i = 0; i < 6; i ++ ) {
            const double distance = planes[ i ].distanceToPoint( center );

            if ( distance < negRadius ) {
                return false;
            }
        }

        return true;
    }

    bool intersectsBox( Box3& box );

    bool containsPoint( Vector3& point ) {
        Plane* planes = this->planes;

        for ( int i = 0; i < 6; i ++ ) {
            if ( planes[ i ].distanceToPoint( point ) < 0 ) {
                return false;
            }
        }

        return true;
    }


};



#endif //THREE_CPP_FRUSTUM_H
