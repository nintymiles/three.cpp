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

struct IntersectionData{
    float distance;
    Vector3 point;
    Object3D::sptr object;
    Vector2 uv;
    Vector2 uv2;
    struct FaceData{
        size_t a,b,c;
        Vector3 normal;
        size_t materialIndex;
    };
    FaceData face;
    size_t faceIndex;
};

float ascSort( std::shared_ptr<IntersectionData> a,std::shared_ptr<IntersectionData> b );

class Raycaster;
void intersectObject( Object3D::sptr object, Raycaster& raycaster, std::vector<std::shared_ptr<IntersectionData>>& intersects, bool recursive );

class Raycaster {
	public:
		double near,far;
        Ray ray;
        Camera::sptr camera;
        Layers::sptr layers;

		Raycaster(Vector3& origin,Vector3& direction,double near = 0,double far = math_number::INF): ray(Ray(origin, direction)), camera(
            nullptr), near(near), far(far),layers(std::make_shared<Layers>()){}

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

        Raycaster& setFromCamera( Vector2& coords, Camera::sptr camera ) {

            if ( camera->isOrthographic ) {
                //引用也可以执行运行时绑定
                OrthographicCamera::sptr oc = std::dynamic_pointer_cast<OrthographicCamera>(camera);

                ray.origin.set( coords.x, coords.y, ( oc->_near + oc->_far ) / ( oc->_near - oc->_far ) ).unproject( *camera ); // set origin in plane of camera
                ray.direction.set( 0, 0, - 1 ).transformDirection( camera->matrixWorld );
                this->camera = camera;

            } else if ( camera->isPerspective ) {
                PerspectiveCamera::sptr pc = std::dynamic_pointer_cast<PerspectiveCamera>(camera);

                ray.origin.setFromMatrixPosition( pc->matrixWorld );
                //投射在film plane某个位置上的光线可以有无数条，这些射线和经由物体上投射在对应点的光线重合。所以这个射线的z值选择并不关键
                //且这条射线只是用来确定相交点
                ray.direction.set( coords.x, coords.y, 0.5 ).unproject( *camera ).sub( ray.origin ).normalize();
                this->camera = camera;

            } else {
                std::cerr << "Raycaster: Unsupported camera type: " + camera->type << std::endl;
            }

            return *this;

        }

        std::vector<std::shared_ptr<IntersectionData>> intersectObject( Object3D::sptr object, bool recursive = true ) {

            std::vector<std::shared_ptr<IntersectionData>> intersects{};

            ::intersectObject( object, *this, intersects, recursive );

            std::sort(intersects.begin(),intersects.end(), ascSort);

            return intersects;

        }

        std::vector<std::shared_ptr<IntersectionData>> intersectObjects( std::vector<Object3D::sptr>& objects, bool recursive = true ) {

            std::vector<std::shared_ptr<IntersectionData>> intersects{};
            for ( size_t i = 0, l = objects.size(); i < l; i ++ ) {

                ::intersectObject( objects[ i ], *this, intersects, recursive );
            }

            //intersects.sort( ascSort );
            std::sort(intersects.begin(),intersects.end(), ascSort);

            return intersects;

        }



};


#endif /* SRC_CORE_RAY_CASTER_H_ */
