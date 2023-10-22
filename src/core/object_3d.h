#ifndef OBJECT_3D_H
#define OBJECT_3D_H

#include "layers.h"
#include "math_utils.h"
#include "vector3.h"
#include "euler.h"
#include "quaternion.h"
#include "sole.h"

#include "buffer_attribute.h"
#include "geometry.h"
#include "material.h"

#include <string>
#include <memory>
#include <functional>
#include <algorithm>

class GLRenderer;
class Scene;
class Group;
class Camera;

class GLRenderTarget;

namespace object3d {
static size_t _objectIdCount = 0;
static Vector3 DefaultUp = Vector3(0,1,0);
static bool DefaultMatrixAutoUpdate = true;
}

using namespace object3d;

/**
* Base class for scene graph objects
*/
class Object3D {
public:
    using sptr = std::shared_ptr<Object3D>;

    /**
     * Unique number of this object instance.
     */
    const size_t id;

    /**
     *
     */
    sole::uuid uuid = sole::uuid1();

    /**
     * Optional name of the object (doesn't need to be unique).
     */
    std::string name="";

    std::string type="Object3D";

    /**
     * Object's parent in the scene graph.
     */
    Object3D* parent = nullptr;

    /**
     * Array with object's children.
     */
    std::vector<Object3D::sptr> children;

    /**
     * Up direction.
     */
    Vector3 up = Vector3(0,1,0);

    /**
     * Object's local position.
     */
    Vector3 position;

    /**
     * Object's local rotation (Euler angles), in radians.
     */
    Euler rotation;

    /**
     * Global rotation.
     */
    Quaternion quaternion;

    /**
     * Object's local scale.
     */
    Vector3 scale = Vector3(1,1,1);

    Matrix4 modelViewMatrix;

    Matrix3 normalMatrix;

    /**
     * Local transform.
     */
    Matrix4 matrix;

    /**
     * The global transform of the object. If the Object3d has no parent, then it's identical to the local transform.
     */
    Matrix4 matrixWorld;

    /**
     * When this is set, it calculates the matrix of position, (rotation or quaternion) and scale every frame and also recalculates the matrixWorld property.
     */
    bool matrixAutoUpdate = DefaultMatrixAutoUpdate;

    /**
     * When this is set, it calculates the matrixWorld in that frame and resets this property to false.
     */
    bool matrixWorldNeedsUpdate = true;

    Layers layers;
    /**
     * Object gets rendered if true.
     */
    bool visible = true;

    /**
     * Gets rendered into shadow map.
     */
    bool castShadow = false;

    /**
     * Material gets baked in shadow receiving.
     */
    bool receiveShadow = false;

    /**
     * When this is set, it checks every frame if the object is in the frustum of the camera. Otherwise the object gets drawn every frame even if it isn't visible.
     */
    bool frustumCulled = true;

    /**
     * Overrides the default rendering order of scene graph objects, from lowest to highest renderOrder. Opaque and transparent objects remain sorted independently though. When this property is set for an instance of Group, all descendants objects will be sorted and rendered together.
     */
    unsigned renderOrder = 0;



    ///**
    // * An object that can be used to store custom data about the Object3d. It should not hold references to functions as these will not be cloned.
    // */


    // /**
    //  * Custom depth material to be used when rendering to the depth map. Can only be used in context of meshes.
    //  * When shadow-casting with a DirectionalLight or SpotLight, if you are (a) modifying vertex positions in
    //  * the vertex shader, (b) using a displacement map, (c) using an alpha map with alphaTest, or (d) using a
    //  * transparent texture with alphaTest, you must specify a customDepthMaterial for proper shadows.
    //  */
    Material::sptr  customDepthMaterial;

    //  /**
    //   * Same as customDepthMaterial, but used with PointLight.
    //   */
    Material::sptr customDistanceMaterial;

    //   /**
    //	* Used to check whether this or derived classes are Object3Ds. Default is true.
    //	* You should not change this, as it is used internally for optimisation.
    //	*/
    //	readonly isObject3D : true;

    /**
     * Calls before rendering object
     */
    //function<void(GLRenderer& renderer, Scene& scene, Camera& camera, std::any& object, Group* group)> onBeforeRender;
    threecpp::Signal<void(GLRenderer& renderer, const std::shared_ptr<Scene>& scene, const std::shared_ptr<Camera>& camera, const std::shared_ptr<Object3D>& object,const std::shared_ptr<BufferGeometry>& geometry,const std::shared_ptr<Material>& material,const std::shared_ptr<GLRenderTarget>& renderTarget,threecpp::DrawRange* group)> onBeforeRender;


    /**
     * Calls after rendering object
     */
    //function<void(GLRenderer& renderer, Scene& scene, Camera& camera, std::any& object, Group* group)> onAfterRender;
    threecpp::Signal<void(GLRenderer& renderer, const std::shared_ptr<Scene>& scene, const std::shared_ptr<Camera>& camera, const std::shared_ptr<Object3D>& object,const std::shared_ptr<BufferGeometry>& geometry,const std::shared_ptr<Material>& material,const std::shared_ptr<GLRenderTarget>& renderTarget, threecpp::DrawRange* group)> onAfterRender;


    std::vector<float> morphTargetInfluences;

    std::unordered_map<std::string, unsigned> morphTargetDictionary;



    Geometry::sptr geometry;

    Material::sptr material;

    std::vector<Material::sptr> materials;

    BufferAttribute<float>::sptr instanceColor;

protected:

    void onRotationChange(const Euler& rotation);

    void onQuaternionChange(const Quaternion& quaternion);



public:
    Object3D();

    Object3D(const Geometry::sptr& geometry, const Material::sptr& material);

    Object3D(const Geometry::sptr& geometry, const std::initializer_list<Material::sptr>& materials);

    Object3D(const Geometry::sptr& geometry, const std::vector<Material::sptr>& materials);

    Object3D(const Object3D& object);

    Object3D& operator = (const Object3D& source) {
        return this->copy(source);
    }
    virtual ~Object3D() = default;


    static sptr create() {
        return std::make_shared<Object3D>();
    }
    static sptr create(const Geometry::sptr& geometry, const Material::sptr& material) {
        return std::make_shared<Object3D>(geometry, material);
    }

    static sptr create(const Geometry::sptr& geometry, const std::initializer_list<Material::sptr>& materials) {
        return std::make_shared<Object3D>(geometry, materials);
    }
    static sptr create(const Object3D& object) {
        return std::make_shared<Object3D>(object);
    }

    /**
     * This updates the position, rotation and scale with the matrix.
     */
    void applyMatrix4(Matrix4& matrix);

    Object3D& applyQuaternion(Quaternion& quaternion);

    /**
     *
     */
    void setRotationFromAxisAngle(Vector3& axis, float angle);

    /**
     *
     */
    void setRotationFromEuler(Euler& euler);

    /**
     *
     */
    void setRotationFromMatrix(Matrix4& m);

    /**
     *
     */
    void setRotationFromQuaternion(Quaternion& q);

    /**
     * Rotate an object along an axis in object space. The axis is assumed to be normalized.
     * @param axis	A normalized vector in object space.
     * @param angle	The angle in radians.
     */
    Object3D& rotateOnAxis(const Vector3& axis, float angle);

    /**
     * Rotate an object along an axis in world space. The axis is assumed to be normalized. Method Assumes no rotated parent.
     * @param axis	A normalized vector in object space.
     * @param angle	The angle in radians.
     */
    Object3D& rotateOnWorldAxis(const Vector3& axis, float angle);

    /**
     *
     * @param angle
     */
    Object3D& rotateX(float angle);

    /**
     *
     * @param angle
     */
    Object3D& rotateY(float angle);

    /**
     *
     * @param angle
     */
    Object3D& rotateZ(float angle);

    /**
     * @param axis	A normalized vector in object space.
     * @param distance	The distance to translate.
     */
    Object3D& translateOnAxis(const Vector3& axis, float distance);

    /**
     * Translates object along x axis by distance.
     * @param distance Distance.
     */
    Object3D& translateX(float distance);

    /**
     * Translates object along y axis by distance.
     * @param distance Distance.
     */
    Object3D& translateY(float distance);

    /**
     * Translates object along z axis by distance.
     * @param distance Distance.
     */
    Object3D& translateZ(float distance);

    /**
     * Updates the vector from local space to world space.
     * @param vector A local vector.
     */
    Vector3 localToWorld(Vector3& vector);

    /**
     * Updates the vector from world space to local space.
     * @param vector A world vector.
     */
    Vector3 worldToLocal(Vector3& vector);

    /**
     * Rotates object to face point in space.
     * @param vector A world vector to look at.
     */
    void lookAt(const Vector3& vector);

    void lookAt(float x, float y, float z);

    /**
     * Adds object as child of this object.
     */
    Object3D& add(const Object3D::sptr & object);

    /**
     * Removes object as child of this object.
     */
    Object3D& remove(const Object3D::sptr & object);

    /**
    * Removes all children objects
    */
    Object3D& removeAll();
    /**
     * Adds object as a child of this, while maintaining the object's world transform.
     */
    Object3D& attach(const Object3D::sptr & object);

    /**
     * Searches through the object's children and returns the first with a matching id.
     * @param id	Unique number of the object instance
     */
    //Object3D& getObjectById(unsigned id);

    /**
     * Searches through the object's children and returns the first with a matching name.
     * @param name	String to match to the children's Object3d.name property.
     */
    //Object3D& getObjectByName(std::string name);

    //Object3D& getObjectByProperty(std::string name, std::string);

    Vector3& getWorldPosition(Vector3& target);

    Quaternion& getWorldQuaternion(Quaternion& target);


    Vector3& getWorldScale(Vector3&& target);

    virtual Vector3& getWorldDirection(Vector3& target);

    //Object3D& raycast(raycaster: Raycaster, intersects : Intersection[]) ;

    void traverse(std::function<void(Object3D&)> callback);

    void traverseVisible(std::function<void(Object3D&)> callback);

    void traverseAncestors(std::function<void(Object3D&)> callback);

    /**
     * Updates local transform.
     */
    void updateMatrix();

    /**
     * Updates global transform of the object and its children.
     */
    virtual void updateMatrixWorld(bool force = false);

    virtual void updateWorldMatrix(bool updateParents = false, bool updateChildren = false);

    /*toJSON(meta ? : {
    geometries: any;
    materials: any;
    textures: any;
    images: any;
    }) : any;*/

    virtual Object3D* clone(bool recursive = true);

    /**
     *
     * @param object
     * @param recursive
     */
    virtual Object3D& copy(const Object3D& source,bool recursive = true);

    bool isCamera = false;

    bool isLight = false;

    bool isMesh = false;

    bool isPoints = false;

    bool isLine = false;

    bool isLightProbe = false;

    bool isInstancedMesh = false;

};

//static long _object3DId = 0;
//
//class Object3D;
//typedef std::function<void(Object3D&)> Object3DCallback;
//
//class Quaternion;
//
//class Object3D:public EventDispatcher{
//public:
//    virtual bool isMesh();
//
//public:
//        Event _addedEvent = {"added"};
//        Event _removedEvent = {"removed"};
//        static std::shared_ptr<Vector3> default_up;
//
//        static const bool default_matrix_auto_update = true;
//
//        int id = _object3DId++;
//        std::string uuid = MathUtils::generate_uuid();
//        std::string name = "";
//        std::string type = "Object3D";
//        bool isObject3D = true;
//
//        std::shared_ptr<Object3D> parent;
//        //3D对象本身的存储使用智能指针
//        std::vector<std::shared_ptr<Object3D>> children;
//
//        //这种声明方式导致内存泄漏,不支持copy构造函数？依赖了default_up静态变量？
//        //std::shared_ptr<Vector3> up = std::make_shared<Vector3>(*default_up);
//        std::shared_ptr<Vector3> up = std::make_shared<Vector3>(0,1,0);
//        //Vector3 up(default_up->x,default_up->y,default_up->z);
//
//        std::shared_ptr<Vector3> position = std::make_shared<Vector3>();
//        //Vector3 position();
//        std::shared_ptr<Euler> rotation = std::make_shared<Euler>();
//        //Euler rotation();
//        std::shared_ptr<Quaternion> quaternion = std::make_shared<Quaternion>();
//        //Quaternion quaternion();
//        std::shared_ptr<Vector3> scale = std::make_shared<Vector3>(1,1,1);
//        //Vector3 scale(1,1,1);
//
//        std::shared_ptr<Matrix4> modelViewMatrix = std::make_shared<Matrix4>();
//        std::shared_ptr<Matrix4> normalMatrix = std::make_shared<Matrix4>();
//
//        std::shared_ptr<Matrix4> matrix = std::make_shared<Matrix4>();
//        std::shared_ptr<Matrix4> matrixWorld = std::make_shared<Matrix4>();
//
//        bool matrixAutoUpdate = default_matrix_auto_update;
//        bool matrixWorldNeedsUpdate = false;
//        std::shared_ptr<Layers> layers = std::make_shared<Layers>();
//        bool visible = true;
//
//        bool castShadow = false;
//        bool receiveShadow = false;
//
//        bool frustumCulled = false;
//        int renderOrder = 0;
//
//
//
//		void onRotationChange() {
//			quaternion->setFromEuler( *rotation, false );
//		}
//
//		void onQuaternionChange() {
//            euler_order order = XYZ;
//			rotation->setFromQuaternion( *quaternion, order, false );
//		}
//
//        //renderer, scene, camera, geometry, material, group
//        virtual void onBeforeRender(){};
//
//        //renderer, scene, camera, geometry, material, group
//        virtual void onAfterRender(){};
//
//        virtual std::string getClassType(){return type;}
//
//        Object3D& applyMatrix4( Matrix4& matrix ) {
//            if(matrixAutoUpdate)
//                updateMatrix();
//
//            this->matrix->premultiply( matrix );
//            this->matrix->decompose( *position,*quaternion,*scale );
//
//            return *this;
//        }
//
//        Object3D& applyQuaternion( Quaternion& q ) {
//		    quaternion->premultiply( q );
//
//		    return *this;
//        }
//
//        Object3D& setRotationFromAxisAngle( Vector3& axis,double angle ) {
//            // assumes axis is normalized
//            quaternion->setFromAxisAngle( axis, angle );
//
//            return *this;
//        }
//
//    	Object3D& setRotationFromEuler( Euler& euler ) {
//            quaternion->setFromEuler( euler, true );
//
//            return *this;
//        }
//
//        Object3D& setRotationFromMatrix( Matrix4& m ) {
//            // assumes the upper 3x3 of m is a pure rotation matrix (i.e, unscaled)
//            quaternion->setFromRotationMatrix( m );
//
//            return *this;
//        }
//
//        Object3D& setRotationFromQuaternion( Quaternion& q ) {
//            // assumes q is normalized
//            quaternion->copy( q );
//
//            return *this;
//        }
//
//    	Object3D& rotateOnAxis( Vector3& axis,double angle );
//
//        Object3D& rotateOnWorldAxis( Vector3& axis, double angle );
//
//    	Object3D& rotateX( double angle );
//
//        Object3D& rotateY( double angle );
//
//        Object3D& rotateZ( double angle );
//
//        Object3D& translateOnAxis( Vector3& axis, double distance );
//
//        Object3D& translateX( double distance );
//
//        Object3D& translateY( double distance );
//
//        Object3D& translateZ( double distance );
//
//
//        Object3D& localToWorld( Vector3& vector ) {
//            vector.applyMatrix4( *matrixWorld );
//
//            return *this;
//        }
//
//        Object3D& worldToLocal( Vector3& vector );
//
//        Object3D& lookAt( double x,double y,double z );
//
//        Object3D& add( std::shared_ptr<Object3D> objectPtr ) {
//            std::shared_ptr<Object3D> thisSharePtr = std::shared_ptr<Object3D>(this);
//            if ( objectPtr == thisSharePtr ) {
//                return *this;
//            }
//
//            if ( objectPtr->isObject3D ) {
//                if ( objectPtr->parent ) {
//                    objectPtr->parent->remove( objectPtr );
//                }
//
//                objectPtr->parent = thisSharePtr;
//                children.push_back( objectPtr );
//
//                objectPtr->dispatchEvent( _addedEvent );
//            }
//
//            return *this;
//        }
//
//        Object3D& remove( std::shared_ptr<Object3D> objectPtr ) {
//            auto result = std::find(children.cbegin(),children.cend(),objectPtr);
//
//            if(result != children.cend()){
//                objectPtr->parent = nullptr;
//                children.erase(result);
//                objectPtr->dispatchEvent(_removedEvent);
//            }
//
//            // const index = this.children.indexOf( object );
//
//            // if ( index !== - 1 ) {
//
//            //     object.parent = null;
//            //     this.children.splice( index, 1 );
//
//            //     objectPtr.dispatchEvent( _removedEvent );
//
//            // }
//
//            return *this;
//        }
//
//        Vector3 getWorldPosition() {
//            updateWorldMatrix( true, false );
//
//            return Vector3().setFromMatrixPosition( *matrixWorld );
//        }
//
//        Quaternion getWorldQuaternion();
//
//        Vector3 getWorldScale();
//
//        Vector3 getWorldDirection() {
//            updateWorldMatrix( true, false );
//
//            Vector3 target;
//            target.set( matrixWorld->elements[ 8 ], matrixWorld->elements[ 9 ], matrixWorld->elements[ 10 ] ).normalize();
//
//            return target;
//        }
//
//        /* raycaster, intersects */
//        virtual std::shared_ptr<Object3D> raycast( ){return 0;};
//
//
//        //提供对每一个子对象包含自身的回调式遍历，
//        Object3D& traverse( Object3DCallback callback ) {
//            callback( *this );
//
//            //for语句只支持单一类型多个变量定义，不支持多类型多变量定义
//            //错误：for ( int i = 0, int l = children.size(); i < l; i ++ )
//            for ( size_t i = 0, l = children.size(); i < l; i ++ ) {
//                children[ i ]->traverse( callback );
//            }
//
//            return *this;
//        }
//
//
//        Object3D& traverseVisible( Object3DCallback callback ) {
//
//            if ( visible == false ) return *this;
//
//            callback( *this );
//
//            for ( size_t i = 0, l = children.size(); i < l; i ++ ) {
//
//                children[ i ]->traverseVisible( callback );
//
//            }
//
//            return *this;
//        }
//
//        Object3D& traverseAncestors( Object3DCallback callback ) {
//            if ( parent != nullptr ) {
//
//                callback( *parent );
//
//                parent->traverseAncestors( callback );
//
//            }
//
//            return *this;
//        }
//
//        Object3D& updateMatrix() {
//            matrix->compose( *position, *quaternion, *scale );
//
//            matrixWorldNeedsUpdate = true;
//
//            return *this;
//        }
//
//        Object3D& updateMatrixWorld( bool force ) {
//
//            if ( matrixAutoUpdate ) updateMatrix();
//
//            if ( matrixWorldNeedsUpdate || force ) {
//
//                if ( parent == nullptr ) {
//                    matrixWorld->copy( *matrix );
//                } else {
//                    matrixWorld->multiplyMatrices( *(parent->matrixWorld), *matrix );
//                }
//
//                matrixWorldNeedsUpdate = false;
//
//                force = true;
//            }
//
//            // update children
//            for ( size_t i = 0, l = children.size(); i < l; i ++ ) {
//                children[ i ]->updateMatrixWorld( force );
//            }
//            return *this;
//        }
//
//        Object3D& updateWorldMatrix( bool updateParents, bool updateChildren ) {
//
//            if ( updateParents == true && parent ) {
//
//                parent->updateWorldMatrix( true, false );
//
//            }
//
//            if ( matrixAutoUpdate ) updateMatrix();
//
//            if ( parent ) {
//                matrixWorld->copy( *matrix );
//            } else {
//                matrixWorld->multiplyMatrices( *parent->matrixWorld, *matrix );
//            }
//
//            // update children
//            if ( updateChildren == true ) {
//                for ( size_t i = 0, l = children.size(); i < l; i ++ ) {
//                    children[ i ]->updateWorldMatrix( false, true );
//                }
//            }
//            return *this;
//        }
//
//        Object3D& copy( Object3D& source, bool recursive = true ) {
//            this->name = source.name;
//
//            this->up->copy( *source.up );
//            this->position->copy( *source.position );
////            this.rotation.order = source.rotation.order;
////            this.quaternion.copy( source.quaternion );
////            this.scale.copy( source.scale );
////
////            this.matrix.copy( source.matrix );
////            this.matrixWorld.copy( source.matrixWorld );
////
////            this.matrixAutoUpdate = source.matrixAutoUpdate;
////            this.matrixWorldNeedsUpdate = source.matrixWorldNeedsUpdate;
////
////            this.matrixWorldAutoUpdate = source.matrixWorldAutoUpdate;
////
////            this.layers.mask = source.layers.mask;
////            this.visible = source.visible;
////
////            this.castShadow = source.castShadow;
////            this.receiveShadow = source.receiveShadow;
////
////            this.frustumCulled = source.frustumCulled;
////            this.renderOrder = source.renderOrder;
////
////            this.userData = JSON.parse( JSON.stringify( source.userData ) );
////
//            if ( recursive == true ) {
//                for ( size_t i = 0; i < source.children.size(); i ++ ) {
//                    std::shared_ptr<Object3D> child = source.children[ i ];
//                    add(child);
//                }
//
//            }
//            return *this;
//        }
//
//
//};

using Object3DSptr = std::shared_ptr<Object3D>;

#endif /* OBJECT_3D_H */
