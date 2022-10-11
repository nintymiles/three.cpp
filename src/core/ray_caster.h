#ifndef SRC_CORE_RAY_CASTER_H_
#define SRC_CORE_RAY_CASTER_H_
#include "number.h"
#include "layers.h"
#include "vector3.h"
#include "ray.h"
#include "cameras/camera.h"
#include "cameras/orthographic_camera.h"
#include "cameras/perspective_camera.h"

#include <cmath>

class Raycaster {
	public:
		double near,far;
        Ray ray;
        Camera* camera;

		Raycaster(Vector3& origin,Vector3& direction,double near = 0,double far = Number::MAX):ray(Ray(origin,direction)),camera(
            nullptr),near(near),far(far){}

	// constructor( origin, direction, near = 0, far = Infinity ) {

	// 	this.ray = new Ray( origin, direction );
	// 	// direction is assumed to be normalized (for accurate distance calculations)

	// 	this.near = near;
	// 	this.far = far;
	// 	this.camera = null;
	// 	this.layers = new Layers();

	// 	this.params = {
	// 		Mesh: {},
	// 		Line: { threshold: 1 },
	// 		LOD: {},
	// 		Points: { threshold: 1 },
	// 		Sprite: {}
	// 	};

	// }

        Raycaster& set( Vector3& origin, Vector3& direction ) {
            // direction is assumed to be normalized (for accurate distance calculations)
            ray.set( origin, direction );

            return *this;
        }

        Raycaster& setFromCamera( Vector3& coords, Camera& camera ) {

            if ( camera.isPerspectiveCamera() ) {
                //引用也可以执行运行时绑定
                OrthographicCamera& oc = dynamic_cast<OrthographicCamera&>(camera);

                ray.origin.setFromMatrixPosition( *oc.matrixWorld );
                ray.direction.set( coords.x, coords.y, 0.5 ).unproject( camera ).sub( ray.origin ).normalize();
                this->camera = &camera;

            } else if ( camera.isOrthographicCamera() ) {
                PerspectiveCamera& pc = dynamic_cast<PerspectiveCamera&>(camera);

                ray.origin.set( coords.x, coords.y, ( pc.near + pc.far ) / ( pc.near - pc.far ) ).unproject( camera ); // set origin in plane of camera
                ray.direction.set( 0, 0, - 1 ).transformDirection( *camera.matrixWorld );
                this->camera = &camera;

            } else {
                //console.error( 'THREE.Raycaster: Unsupported camera type: ' + camera.type );
            }

        }

//	intersectObject( object, recursive = true, intersects = [] ) {
//
//		intersectObject( object, this, intersects, recursive );
//
//		intersects.sort( ascSort );
//
//		return intersects;
//
//	}
//
//	intersectObjects( objects, recursive = true, intersects = [] ) {
//
//		for ( let i = 0, l = objects.length; i < l; i ++ ) {
//
//			intersectObject( objects[ i ], this, intersects, recursive );
//
//		}
//
//		intersects.sort( ascSort );
//
//		return intersects;
//
//	}
//
//}
//
//function ascSort( a, b ) {
//
//	return a.distance - b.distance;
//
//}
//
//function intersectObject( object, raycaster, intersects, recursive ) {
//
//	if ( object.layers.test( raycaster.layers ) ) {
//
//		object.raycast( raycaster, intersects );
//
//	}
//
//	if ( recursive === true ) {
//
//		const children = object.children;
//
//		for ( let i = 0, l = children.length; i < l; i ++ ) {
//
//			intersectObject( children[ i ], raycaster, intersects, true );
//
//		}
//
//	}

};

#endif /* SRC_CORE_RAY_CASTER_H_ */
