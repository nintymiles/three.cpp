#ifndef OBJECT_3D_H
#define OBJECT_3D_H
#include "event_dispatcher.h"
#include "layers.h"
#include "math_utils.h"
#include "vector3.h"

#include <string>
#include <memory>
#include <functional>
#include <algorithm>

static long _object3DId = 0;
Event _addedEvent = {"added"};
Event _removedEvent = {"removed"};
class Object3D;
typedef std::function<void(Object3D&)> Object3DCallback;

class Quaternion;

class Object3D:public EventDispatcher{
    private:

    public:
        static std::shared_ptr<Vector3> default_up;

        static const bool default_matrix_auto_update = true;

        int id = _object3DId++;
        std::string uuid = MathUtils::generate_uuid();
        std::string name = "";
        std::string type = "Object3D";
        bool isObject3D = true;

        std::shared_ptr<Object3D> parent;
        //3D对象本身的存储使用智能指针
        std::vector<std::shared_ptr<Object3D>> children;
        
        std::shared_ptr<Vector3> up = std::make_shared<Vector3>(Vector3().copy(*default_up));
        //Vector3 up(default_up->x,default_up->y,default_up->z);

        std::shared_ptr<Vector3> position = std::make_shared<Vector3>();
        //Vector3 position();
        std::shared_ptr<Euler> rotation = std::make_shared<Euler>();
        //Euler rotation();
        std::shared_ptr<Quaternion> quaternion = std::make_shared<Quaternion>();
        //Quaternion quaternion();
        std::shared_ptr<Vector3> scale = std::make_shared<Vector3>(1,1,1);
        //Vector3 scale(1,1,1);

        std::shared_ptr<Matrix4> modelViewMatrix = std::make_shared<Matrix4>(); 
        std::shared_ptr<Matrix4> normalMatrix = std::make_shared<Matrix4>();

        std::shared_ptr<Matrix4> matrix = std::make_shared<Matrix4>();
        std::shared_ptr<Matrix4> matrixWorld = std::make_shared<Matrix4>();

        bool matrixAutoUpdate = default_matrix_auto_update;
        bool matrixWorldNeedsUpdate = false;
        std::shared_ptr<Layers> layers = std::make_shared<Layers>();
        bool visible = true;

        bool castShadow = false;
        bool receiveShadow = false;

        bool frustumCulled = false;
        int renderOrder = 0;



		void onRotationChange() {
			quaternion->setFromEuler( *rotation, false );
		}

		void onQuaternionChange() {
            euler_order order = XYZ;
			rotation->setFromQuaternion( *quaternion, order, false );
		}

        //renderer, scene, camera, geometry, material, group
        virtual void onBeforeRender(){};

        //renderer, scene, camera, geometry, material, group
        virtual void onAfterRender(){};

        virtual std::string getClassType(){return type;}

        Object3D& applyMatrix4( Matrix4& matrix ) {
            if(matrixAutoUpdate) 
                updateMatrix();

            this->matrix->premultiply( matrix );
            this->matrix->decompose( *position,*quaternion,*scale );

            return *this;
        }

        Object3D& applyQuaternion( Quaternion& q ) {
		    quaternion->premultiply( q );

		    return *this;
        }

        Object3D& setRotationFromAxisAngle( Vector3& axis,double angle ) {
            // assumes axis is normalized
            quaternion->setFromAxisAngle( axis, angle );

            return *this;
        }

    	Object3D& setRotationFromEuler( Euler& euler ) {
            quaternion->setFromEuler( euler, true );

            return *this;
        }

        Object3D& setRotationFromMatrix( Matrix4& m ) {
            // assumes the upper 3x3 of m is a pure rotation matrix (i.e, unscaled)
            quaternion->setFromRotationMatrix( m );

            return *this;
        }

        Object3D& setRotationFromQuaternion( Quaternion& q ) {
            // assumes q is normalized
            quaternion->copy( q );

            return *this;
        }
    
    	Object3D& rotateOnAxis( Vector3& axis,double angle );

        Object3D& rotateOnWorldAxis( Vector3& axis, double angle );

    	Object3D& rotateX( double angle );

        Object3D& rotateY( double angle );

        Object3D& rotateZ( double angle );

        Object3D& translateOnAxis( Vector3& axis, double distance );

        Object3D& translateX( double distance );

        Object3D& translateY( double distance );

        Object3D& translateZ( double distance );


        Object3D& localToWorld( Vector3& vector ) {
            vector.applyMatrix4( *matrixWorld );

            return *this;
        }

        Object3D& worldToLocal( Vector3& vector );

        Object3D& lookAt( double x,double y,double z );

        Object3D& add( std::shared_ptr<Object3D> objectPtr ) {
            std::shared_ptr<Object3D> thisSharePtr = std::shared_ptr<Object3D>(this);
            if ( objectPtr == thisSharePtr ) {
                return *this;
            }

            if ( objectPtr->isObject3D ) {
                if ( objectPtr->parent ) {
                    objectPtr->parent->remove( objectPtr );
                }

                objectPtr->parent = thisSharePtr;
                children.push_back( objectPtr );

                objectPtr->dispatchEvent( _addedEvent );
            } 

            return *this;
        }

        Object3D& remove( std::shared_ptr<Object3D> objectPtr ) {
            auto result = std::find(children.cbegin(),children.cend(),objectPtr);

            if(result != children.cend()){
                objectPtr->parent = nullptr;
                children.erase(result);
                objectPtr->dispatchEvent(_removedEvent);
            }

            // const index = this.children.indexOf( object );

            // if ( index !== - 1 ) {

            //     object.parent = null;
            //     this.children.splice( index, 1 );

            //     objectPtr.dispatchEvent( _removedEvent );

            // }

            return *this;
        }        

        Vector3 getWorldPosition() {
            updateWorldMatrix( true, false );

            return Vector3().setFromMatrixPosition( *matrixWorld );
        }

        Quaternion getWorldQuaternion();

        Vector3 getWorldScale();

        Vector3 getWorldDirection() {
            updateWorldMatrix( true, false );

            Vector3 target;
            target.set( matrixWorld->elements[ 8 ], matrixWorld->elements[ 9 ], matrixWorld->elements[ 10 ] ).normalize();

            return target;
        }

        /* raycaster, intersects */
        virtual std::shared_ptr<Object3D> raycast( ){return 0;};


        //提供对每一个子对象包含自身的回调式遍历，
        Object3D& traverse( Object3DCallback callback ) {
            callback( *this );

            //for语句只支持单一类型多个变量定义，不支持多类型多变量定义
            //错误：for ( int i = 0, int l = children.size(); i < l; i ++ )
            for ( size_t i = 0, l = children.size(); i < l; i ++ ) {
                children[ i ]->traverse( callback );
            }

            return *this;
        }


        Object3D& traverseVisible( Object3DCallback callback ) {

            if ( visible == false ) return *this;

            callback( *this );

            for ( size_t i = 0, l = children.size(); i < l; i ++ ) {

                children[ i ]->traverseVisible( callback );

            }

            return *this;
        }

        Object3D& traverseAncestors( Object3DCallback callback ) {
            if ( parent != nullptr ) {

                callback( *parent );

                parent->traverseAncestors( callback );

            }

            return *this;
        }

        Object3D& updateMatrix() {
            matrix->compose( *position, *quaternion, *scale );

            matrixWorldNeedsUpdate = true;

            return *this;
        }

        Object3D& updateMatrixWorld( bool force ) {

            if ( matrixAutoUpdate ) updateMatrix();

            if ( matrixWorldNeedsUpdate || force ) {

                if ( parent == nullptr ) {
                    matrixWorld->copy( *matrix );
                } else {
                    matrixWorld->multiplyMatrices( *(parent->matrixWorld), *matrix );
                }

                matrixWorldNeedsUpdate = false;

                force = true;
            }

            // update children
            for ( size_t i = 0, l = children.size(); i < l; i ++ ) {
                children[ i ]->updateMatrixWorld( force );
            }
            return *this;
        }

        Object3D& updateWorldMatrix( bool updateParents, bool updateChildren ) {

            if ( updateParents == true && parent ) {

                parent->updateWorldMatrix( true, false );

            }

            if ( matrixAutoUpdate ) updateMatrix();

            if ( parent ) {
                matrixWorld->copy( *matrix );
            } else {
                matrixWorld->multiplyMatrices( *parent->matrixWorld, *matrix );
            }

            // update children
            if ( updateChildren == true ) {
                for ( size_t i = 0, l = children.size(); i < l; i ++ ) {
                    children[ i ]->updateWorldMatrix( false, true );
                }
            }
            return *this;
        }

        Object3D& copy( Object3D& source, bool recursive = true ) {
            this->name = source.name;

            this->up->copy( *source.up );
            this->position->copy( *source.position );
//            this.rotation.order = source.rotation.order;
//            this.quaternion.copy( source.quaternion );
//            this.scale.copy( source.scale );
//
//            this.matrix.copy( source.matrix );
//            this.matrixWorld.copy( source.matrixWorld );
//
//            this.matrixAutoUpdate = source.matrixAutoUpdate;
//            this.matrixWorldNeedsUpdate = source.matrixWorldNeedsUpdate;
//
//            this.matrixWorldAutoUpdate = source.matrixWorldAutoUpdate;
//
//            this.layers.mask = source.layers.mask;
//            this.visible = source.visible;
//
//            this.castShadow = source.castShadow;
//            this.receiveShadow = source.receiveShadow;
//
//            this.frustumCulled = source.frustumCulled;
//            this.renderOrder = source.renderOrder;
//
//            this.userData = JSON.parse( JSON.stringify( source.userData ) );
//
            if ( recursive == true ) {
                for ( size_t i = 0; i < source.children.size(); i ++ ) {
                    std::shared_ptr<Object3D> child = source.children[ i ];
                    add(child);
                }

            }
            return *this;
        }
        

};
#endif /* OBJECT_3D_H */
