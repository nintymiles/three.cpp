#ifndef BOX3_H
#define BOX3_H

#include "vector3.h"

#include <climits>
#include <cfloat>
#include <memory>
#include <vector>

template<typename T> class BufferAttribute;
class Object3d;
class Sphere;
struct Plane;
class Triangle;
using std::vector;

class Box3 {
    private: 
		

    public:
		Vector3 min,max;
        Box3(Vector3 min = Vector3(DBL_MIN,DBL_MIN,DBL_MIN),Vector3 max = Vector3(DBL_MAX,DBL_MAX,DBL_MAX)):min(min),max(max){}
        
        Box3& set(Vector3 min,Vector3 max){
            min.copy(min);
            max.copy(max);

            return *this;
        }

		//使用数组的地方同时提供vector实现
        Box3& setFromVector(vector<double> vec) {
            //初始值为反方向数值极值
            double minX = DBL_MAX;
            double minY = DBL_MAX;
            double minZ = DBL_MAX;

            double maxX = DBL_MIN;
            double maxY = DBL_MIN;
            double maxZ = DBL_MIN;

            for ( size_t i = 0, l = vec.size(); i < l; i += 3 ) {

                const double x = vec[ i ];
                const double y = vec[ i + 1 ];
                const double z = vec[ i + 2 ];

                if ( x < minX ) minX = x;
                if ( y < minY ) minY = y;
                if ( z < minZ ) minZ = z;

                if ( x > maxX ) maxX = x;
                if ( y > maxY ) maxY = y;
                if ( z > maxZ ) maxZ = z;

            }

            min.set( minX, minY, minZ );
            max.set( maxX, maxY, maxZ );

            return *this;
        }

		//数组的使用可能引起麻烦
		Box3& setFromArray(double array[],int arrLen) {
			double minX = DBL_MAX;
			double minY = DBL_MAX;
			double minZ = DBL_MAX;
			double maxX = DBL_MIN;
			double maxY = DBL_MIN;
			double maxZ = DBL_MIN;

			for ( int i = 0, l = arrLen; i < l; i += 3 ) {

				const double x = array[ i ];
				const double y = array[ i + 1 ];
				const double z = array[ i + 2 ];

				if ( x < minX ) minX = x;
				if ( y < minY ) minY = y;
				if ( z < minZ ) minZ = z;

				if ( x > maxX ) maxX = x;
				if ( y > maxY ) maxY = y;
				if ( z > maxZ ) maxZ = z;

			}

			min.set( minX, minY, minZ );
			max.set( maxX, maxY, maxZ );

			return *this;
		}

		template <typename T>
		Box3& setFromBufferAttribute( BufferAttribute<T>& attribute ); 

        Box3& setFromPoints(std::vector<Vector3>& points) {
            makeEmpty();

            for ( int i = 0, il = points.size(); i < il; i ++ ) {
                expandByPoint( points[ i ] );
            }

            return *this;
        }

		Box3& setFromPoints(Vector3 points[]) {
            makeEmpty();

            for ( int i = 0, il = 6; i < il; i ++ ) {
                expandByPoint( points[ i ] );
            }

            return *this;
        }

		Box3& setFromCenterAndSize(Vector3& center, Vector3& size);

	// setFromObject( object, precise = false ) {

	// 	this.makeEmpty();

	// 	return this.expandByObject( object, precise );

	// }

	// clone() {

	// 	return new this.constructor().copy( this );

	// }

		Box3& copy(Box3& box) {
			min.copy( box.min );
			max.copy( box.max );

			return *this;
		}

        Box3& makeEmpty() {
            min.x = min.y = min.z = DBL_MAX;
            max.x = max.y = max.z = DBL_MIN;

            return *this;
        }

		bool isEmpty() {
			// this is a more robust check for empty than ( volume <= 0 ) because volume can get positive with two negative axes
			return ( max.x < min.x ) || ( max.y < min.y ) || ( max.z < min.z );

		}

		// getCenter( target ) {
		// 	return this.isEmpty() ? target.set( 0, 0, 0 ) : target.addVectors( this.min, this.max ).multiplyScalar( 0.5 );
		// }
		Vector3& getCenter(Vector3& target) {
			//Vector3 zeroVec(0,0,0);
			return isEmpty() ? target.set(0,0,0) : target.addVectors(min,max).multiplyScalar(0.5);
		}

		Vector3& getSize(Vector3& target) {
			return isEmpty() ? target.set(0, 0, 0) : target.subVectors(max, min);
		}

		Box3& expandByPoint(Vector3& point) {
			min.min(point);
			max.max(point);

			return *this;
		}

		Box3& expandByVector(Vector3& vector) {
			min.sub( vector );
			max.add( vector );

			return *this;
		}

		Box3& expandByScalar(int scalar) {
			min.addScalar( - scalar );
			max.addScalar( scalar );

			return *this;
		}

        bool intersectsTriangle( Triangle& triangle );

		//Box3& expandByObject(Object3d& object, bool precise = false );

		bool containsPoint(Vector3& point) {
			return point.x < min.x || point.x > max.x ||
				point.y < min.y || point.y > max.y ||
				point.z < min.z || point.z > max.z ? false : true;
		}

		bool containsBox(Box3& box) {
			return min.x <= box.min.x && box.max.x <= max.x &&
				min.y <= box.min.y && box.max.y <= max.y &&
				min.z <= box.min.z && box.max.z <= max.z;

		}

		Vector3& getParameter(Vector3& point,Vector3& target) {
			// This can potentially have a divide by zero if the box
			// has a size dimension of 0.
			return target.set(
				( point.x - min.x ) / ( max.x - min.x ),
				( point.y - min.y ) / ( max.y - min.y ),
				( point.z - min.z ) / ( max.z - min.z )
			);
		}

		bool intersectsBox(Box3& box) {
			// using 6 splitting planes to rule out intersections.
			return box.max.x < min.x || box.min.x > max.x ||
				box.max.y < min.y || box.min.y > max.y ||
				box.max.z < min.z || box.min.z > max.z ? false : true;
		}

        Sphere& getBoundingSphere( Sphere& target );

        bool intersectsSphere(Sphere& sphere);

	    bool intersectsPlane( const Plane& plane );

		Vector3& clampPoint(Vector3& point,Vector3& target) {
			return target.copy(point).clamp(min,max);
		}

		//使用静态存储变量做计算的中间媒介，可能存在并发问题
		double distanceToPoint(Vector3& point);



		Box3& intersect(Box3& box) {

			min.max( box.min );
			max.min( box.max );

			// ensure that if there is no overlap, the result is fully empty, not slightly empty with non-inf/+inf values that will cause subsequence intersects to erroneously return valid values.
			if ( isEmpty() ) makeEmpty();

			return *this;
		}

		Box3& unionBox(Box3& box) {
			min.min( box.min );
			max.max( box.max );

			return *this;
		}

		Box3& applyMatrix4(Matrix4& matrix);

		Box3& translate(Vector3& offset) {
			min.add( offset );
			max.add( offset );

			return *this;
		}

		bool equals(Box3& box) {

			return box.min.equals( min ) && box.max.equals( max );

		}

		static bool satForAxes(double axes[],int length,Vector3& v0,Vector3& v1,Vector3& v2,Vector3& extents);

};

#endif /* BOX3_H */
