//
// Created by SeanR on 2022/6/12.
//

#ifndef SRC_MATH_RAY_H_
#define SRC_MATH_RAY_H_

#include "vector3.h"
#include "sphere.h"
#include <memory>

class Ray: public std::enable_shared_from_this<Ray>{
public:
    using sptr = std::shared_ptr<Ray>;

    Vector3 origin;
    Vector3 direction;

    Ray(Vector3 origin = Vector3(),Vector3 direction = Vector3(0,0,-1)):origin(origin),direction(direction){}

    Ray& set(Vector3& origin,Vector3& direction){
        this->origin.copy(origin);
        this->direction.copy(direction);

        return *this;
    }

    //copy constructor
    Ray(const Ray& ray):origin(ray.origin),direction(ray.direction){}

    Ray& copy(const Ray& ray){
         this->origin.copy(ray.origin);
         this->direction.copy(ray.direction);

         return *this;
    }

    Vector3& at(double t, Vector3& target){
        return target.copy( this->direction ).multiplyScalar( t ).add( this->origin );
    }

    Ray& lookAt(Vector3& v) {
        this->direction.copy(v).sub( this->origin ).normalize();

        return *this;
    }

    Ray& recast(double t);

    Vector3& closestPointToPoint( Vector3& point, Vector3& target ) {
        target.subVectors( point, origin );
        const double directionDistance = target.dot( direction );

        if ( directionDistance < 0 ) {
            return target.copy( origin );
        }

        return target.copy( direction ).multiplyScalar( directionDistance ).add( origin );
    }

    double distanceToPoint(Vector3& point) {
        return sqrt( distanceSqToPoint( point ) );
    }

    double distanceSqToPoint(Vector3& point);

    double distanceSqToSegment(Vector3& v0,Vector3& v1,std::shared_ptr<Vector3> optionalPointOnRay,std::shared_ptr<Vector3> optionalPointOnSegment);

    bool intersectSphere( Sphere& sphere, Vector3& target );

    bool intersectsSphere( Sphere& sphere ) {
        return distanceSqToPoint( sphere.center ) <= ( sphere.radius * sphere.radius );
    }

    float distanceToPlane( Plane& plane );

    bool intersectPlane( Plane& plane, Vector3& target );

    bool intersectsPlane( Plane& plane );

    bool intersectBox( Box3& box, Vector3& target );

    bool intersectsBox( Box3& box );

    bool intersectTriangle( Vector3& a, Vector3& b, Vector3& c, bool backfaceCulling, Vector3& target );

    Ray& applyMatrix4( Matrix4& matrix4 ) {
        origin.applyMatrix4( matrix4 );
        direction.transformDirection( matrix4 );

        return *this;
    }

    bool equals( Ray& ray ) {
        return ray.origin.equals( origin ) && ray.direction.equals( direction );
    }

    Ray clone() {
        return *this;
    }

};



#endif /* SRC_MATH_RAY_H_ */
