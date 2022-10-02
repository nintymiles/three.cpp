//
// Created by SeanR on 2022/9/28.
//

#ifndef SRC_MATH_PLANE_H_
#define SRC_MATH_PLANE_H_

#include "matrix3.h"
#include "vector3.h"
#include "sphere.h"

struct Plane
{
    //类内初始化不能使用括号方式，因为这样和函数声明冲突
    Vector3 normal;
    double constant;

    Plane(Vector3 normal = Vector3(1, 0, 0), double constant = 0) : normal(normal), constant(constant){};

    //        copy( plane ) {
    //
    //            this.normal.copy( plane.normal );
    //            this.constant = plane.constant;
    //
    //            return this;
    //
    //        }
    Plane(const Plane &p) : normal(p.normal), constant(p.constant){};

    Plane &set(Vector3 &n, double c)
    {
        normal.copy(n);
        constant = c;

        return *this;
    }

    Plane &setComponents(double x, double y, double z, double w)
    {
        normal.set(x, y, z);
        constant = w;

        return *this;
    }

    Plane &setFromNormalAndCoplanarPoint(Vector3 &normal, Vector3 &point)
    {
        normal.copy(normal);
        constant = -point.dot(normal);

        return *this;
    }

    Plane &setFromCoplanarPoints(Vector3 &a, Vector3 &b, Vector3 &c);

    Plane &normalize()
    {
        // Note: will lead to a divide by zero if the plane is invalid.
        const double inverseNormalLength = 1.0 / normal.length();
        normal.multiplyScalar(inverseNormalLength);
        constant *= inverseNormalLength;

        return *this;
    }

    Plane &negate()
    {
        constant *= -1;
        normal.negate();

        return *this;
    }

    double distanceToPoint(Vector3 &point)
    {
        return normal.dot(point) + constant;
    }

    double distanceToSphere(Sphere &sphere)
    {
        return distanceToPoint(sphere.center) - sphere.radius;
    }

    Vector3 &projectPoint(Vector3 &point, Vector3 &target)
    {
        return target.copy(normal).multiplyScalar(-distanceToPoint(point)).add(point);
    }

    Plane &translate(Vector3 &offset)
    {
        constant -= offset.dot(normal);

        return *this;
    }

    Vector3 &coplanarPoint(Vector3 &target)
    {
        return target.copy(normal).multiplyScalar(-constant);
    }

    Plane &applyMatrix4(Matrix4 &matrix);

    Plane &applyMatrix4(Matrix4 &matrix, Matrix3 &normalMatrix);

    //    Plane& applyMatrix4( Matrix4& matrix, Matrix3& optionalNormalMatrix ) {
    //        const normalMatrix = optionalNormalMatrix || _normalMatrix.getNormalMatrix( matrix );
    //
    //        const referencePoint = this.coplanarPoint( _vector1 ).applyMatrix4( matrix );
    //
    //        const normal = this.normal.applyMatrix3( normalMatrix ).normalize();
    //
    //        this.constant = - referencePoint.dot( normal );
    //
    //        return this;
    //    }

    bool equals( const Plane& plane ) {
        return plane.normal.equals( normal ) && ( plane.constant == constant );
    }
    // bool operator==(Plane &p)
    // {
    //     return normal.equals(p.normal) && constant == p.constant;
    // }

    // bool operator!=(Plane &p)
    // {
    //     // return !(this->operator==(p));
    //     return !(*this) == p;
    // }

    //        intersectLine( line, target ) {
    //
    //            const direction = line.delta( _vector1 );
    //
    //            const denominator = this.normal.dot( direction );
    //
    //            if ( denominator === 0 ) {
    //
    //                // line is coplanar, return origin
    //                if ( this.distanceToPoint( line.start ) === 0 ) {
    //
    //                    return target.copy( line.start );
    //
    //                }
    //
    //                // Unsure if this is the correct method to handle this case.
    //                return null;
    //
    //            }
    //
    //            const t = - ( line.start.dot( this.normal ) + this.constant ) / denominator;
    //
    //            if ( t < 0 || t > 1 ) {
    //
    //                return null;
    //
    //            }
    //
    //            return target.copy( direction ).multiplyScalar( t ).add( line.start );
    //
    //        }
    //
    //        intersectsLine( line ) {
    //
    //            // Note: this tests if a line intersects the plane, not whether it (or its end-points) are coplanar with it.
    //
    //            const startSign = this.distanceToPoint( line.start );
    //            const endSign = this.distanceToPoint( line.end );
    //
    //            return ( startSign < 0 && endSign > 0 ) || ( endSign < 0 && startSign > 0 );
    //
    //        }
    //
    //        intersectsBox( box ) {
    //
    //            return box.intersectsPlane( this );
    //
    //        }
    //
    //        intersectsSphere( sphere ) {
    //
    //            return sphere.intersectsPlane( this );
    //
    //        }

    //        clone() {
    //
    //            return new this.constructor().copy( this );
    //
    //        }
};

inline bool operator==(const Plane &lhs, const Plane &rhs){
    return lhs.normal.equals(rhs.normal) && lhs.constant == rhs.constant;
}
inline bool operator!=(const Plane &lhs, const Plane &rhs){
    return !(lhs == rhs);
}

#endif /* SRC_MATH_PLANE_H_ */
