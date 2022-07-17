#include "event_dispatcher.h"
#include "layers.h"
#include "../math/math_utils.h"
#include "../math/vector3.h"

#include <string>
#include <memory>
#include <functional>

static long _object3DId = 0;
const Event _addedEvent = {type:"added"};
const Event _removedEvent = {type:"removed"};
typedef std::function<Object3D&> Object3DCallback;

class Object3D:public EventDispatcher{
    private:
        //成员之间的数据传递应该尽量使用引用，对象间的数据传递可能才是共享指针的长处所在
        //std::shared_ptr<Quaternion> _q1 = std::make_shared<Quaternion>(new Quaternion());
        Quaternion _q1();

        //std::shared_ptr<Vector3>    _v1 = std::make_shared<Vector3>(new Vector3());
        Vector3 _v1();
        //std::shared_ptr<Matrix4>    _m1 = std::make_shared<Matrix4>(new Matrix4()); 
        Matrix4 _m1();
        
        //std::shared_ptr<Vector3>    _target = std::make_shared<Vector3>(new Vector3());
        Vector3 _target();

        //std::shared_ptr<Vector3>    _position = std::make_shared<Vector3>(new Vector3());
        Vector3 _position();
        //std::shared_ptr<Vector3>    _scale = std::make_shared<Vector3>(new Vector3());
        Vector3 _scale();
        //std::shared_ptr<Quaternion> _quaternion = std::make_shared<Quaternion>(new Quaternion());
        Quaternion _quaternion();


        //std::shared_ptr<Vector3>    _xAxis = std::make_shared<Vector3>(new Vector3(1,0,0));
        Vector3 _xAxis(1f,0f,0f);
        //std::shared_ptr<Vector3>    _yAxis = std::make_shared<Vector3>(new Vector3(0,1,0));
        Vector3 _yAxis(0f,1f,0f);
        //std::shared_ptr<Vector3>    _zAxis = std::make_shared<Vector3>(new Vector3(0,0,1));
        Vector3 _zAxis(0f,0f,1f);

    public:
        static std::shared_ptr<Vector3> default_up= std::make_shared<Vector3>(new Vector3(0,1,0));

        static const bool default_matrix_auto_update = true;

        int id = _object3DId++;
        std::string uuid = generate_uuid();
        std::string name = "";
        std::string type = "Object3D";
        std::shared_ptr<Object3D> parent = std::make_shared<Object3D>(nullptr);
        //3D对象本身的存储使用智能指针
        std::vector<std::shared_ptr<Object3D>> children;
        
        //每个对象的属性不应该以共享数据的方式存在
        //std::shared_ptr<Vector3> up = std::make_shared<Vector3>(new Vector3())->copy(*defualt_up);
        Vector3 up(*default_up.x,*default_up.y,*default_up.z);

        // std::shared_ptr<Vector3> position = std::make_shared<Vector3>(new Vector3());
        Vector3 position();
        //std::shared_ptr<Eulter> rotation = std::make_shared<Eulter>(new Euler());
        Euler rotation();
        //std::shared_ptr<Quaternion> quaternion = std::make_shared<Quaternion>(new Quaternion());
        Quaternion quaternion();
        //std::shared_ptr<Vector3> scale = std::make_shared<Vector3>(new Vector3(1,1,1));
        Vector3 scale(1,1,1);

        std::shared_ptr<Matrix4> modelViewMatrix = std::make_shared<Matrix4>(new Matrix4()); 
        std::shared_ptr<Matrix4> normalMatrix = std::make_shared<Matrix4>(new Matrix4());

        std::shared_ptr<Matrix4> matrix = std::make_shared<Matrix4>(new Matrix4());
        std::shared_ptr<Matrix4> matrixWorld = std::make_shared<Matrix4>(new Matrix4());

        bool matrixAutoUpdate = default_matrix_auto_update;
        bool matrixWorldNeedsUpdate = false;
        std::shared_ptr<Layers> layers = std::make_shared<Layers>(new Layers());
        bool visible = true;

        bool castShadow = false;
        bool receiveShadow = false;

        bool frustumCulled = false;
        int renderOrder = 0;



		void onRotationChange() {
			quaternion.setFromEuler( rotation, false );
		}

		void onQuaternionChange() {
			rotation->setFromQuaternion( *quaternion, nullptr, false );
		}

        //renderer, scene, camera, geometry, material, group
        virtual void onBeforeRender();

        //renderer, scene, camera, geometry, material, group
        virtual void onAfterRender();

        virtual std::string getClassType(){return type};

        Object3D& applyMatrix4( Matrix4& matrix ) {
            if(matrixAutoUpdate) this->updateMatrix();

            matrix->premultiply( matrix );
            matrix->decompose( *position,*quaternion,*scale );

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
    
    	Object3D& rotateOnAxis( Vector3& axis,double angle ) {

            // rotate object on axis in object space
            // axis is assumed to be normalized
            _q1->setFromAxisAngle( axis, angle );

            quaternion->multiply( *_q1 );

            return this;

        }

        Object3D& rotateOnWorldAxis( Vector3& axis, double angle ) {
            // rotate object on axis in world space
            // axis is assumed to be normalized
            // method assumes no rotated parent

            _q1->setFromAxisAngle( axis, angle );

            quaternion->premultiply( *_q1 );

            return *this;
        }

    	Object3D& rotateX( double angle ) {
            rotateOnAxis( &_xAxis, angle );

            return *this;
        }

        Object3D& rotateY( double angle ) {
            rotateOnAxis( _yAxis, angle );

            return *this;
        }

        Object3D& rotateZ( double angle ) {
            rotateOnAxis( _zAxis, angle );

            return *this;
        }

        Object3D& translateOnAxis( Vector3& axis, double distance ) {
            // translate object by distance along axis in object space
            // axis is assumed to be normalized
            _v1->copy( axis ).applyQuaternion( *quaternion );

            position->add( _v1->multiplyScalar( distance ) );

            return *this;
        }

        Object3D& translateX( double distance ) {
            translateOnAxis( _xAxis, distance );

            return *this;
        }

        Object3D& translateY( double distance ) {
            translateOnAxis( _yAxis, distance );

            return *this;
        }

        Object3D& translateZ( double distance ) {
            translateOnAxis( _zAxis, distance );

            return *this;
        }


        Object3D& localToWorld( Vector3& vector ) {
            vector.applyMatrix4( this.matrixWorld );

            return *this;
        }

        Object3D& worldToLocal( Vector3& vector ) {
            vector.applyMatrix4( _m1->copy( *matrixWorld ).invert() );

            return *this;
        }

        Object3D& lookAt( double x,double y,double z ) {
            // This method does not support objects having non-uniformly-scaled parent(s)
            
            _target->set( x, y, z );

            std::shared_ptr<Object3D> parent = this->parent;

            updateWorldMatrix( true, false );

            _position->setFromMatrixPosition( matrixWorld );

            //if object is an instance of camera or light
            if ( this->getClassType()=="Camera" || this->getClassType()=="Light" ) {
                _m1->lookAt( *_position, *_target, *up );
            } else {
                _m1->lookAt( *_target, *_position, *up );
            }

            quaternion->setFromRotationMatrix( *_m1 );

            if ( parent ) {

                _m1->extractRotation( parent->matrixWorld );
                _q1->setFromRotationMatrix( *_m1 );
                quaternion->premultiply( _q1->invert() );

            }

            return *this;
        }

        Object3D& add( Object3D& object ) {

            if ( arguments.length > 1 ) {

                for ( let i = 0; i < arguments.length; i ++ ) {

                    this.add( arguments[ i ] );

                }

                return this;

            }

            if ( object == this ) {
                std::cout << ( 'THREE.Object3D.add: object can\'t be added as a child of itself. ' ) << object << std::endl;
                
                return *this;
            }

            if ( object && object.isObject3D ) {

                if ( object.parent != null ) {

                    object.parent.remove( object );

                }

                object.parent = this;
                this.children.push( object );

                object.dispatchEvent( _addedEvent );

            } 
            // else {

            //     console.error( 'THREE.Object3D.add: object not an instance of THREE.Object3D.', object );

            // }

            return *this;
        }


        getWorldPosition( target ) {

            this.updateWorldMatrix( true, false );

            return target.setFromMatrixPosition( this.matrixWorld );

        }

        Object3D& getWorldQuaternion( target ) {
            updateWorldMatrix( true, false );

            matrixWorld->decompose( _position, target, _scale );

            return *this;
        }

        Object3D& getWorldScale( Vector3& target ) {
            updateWorldMatrix( true, false );

            matrixWorld->decompose( *_position, *_quaternion, target );

            return *this;
        }

        Object3D& getWorldDirection( Vector3& target ) {
            updateWorldMatrix( true, false );

            target->set( matrixWorld->elements[ 8 ], matrixWorld->elements[ 9 ], matrixWorld->elements[ 10 ] ).normalize();

            return *this;
        }

        /* raycaster, intersects */
        virtual Object3D& raycast( ) {}


        //提供对每一个子对象包含自身的回调式遍历，
        Object3D& traverse( Object3DCallback callback ) {
            callback( this );

            for ( int i = 0, int l = children.size(); i < l; i ++ ) {
                children[ i ]->traverse( callback );
            }

            return *this;
        }


        Object3D& traverseVisible( Object3DCallback callback ) {

            if ( visible == false ) return;

            callback( *this );

            for ( int i = 0, int l = children.size(); i < l; i ++ ) {

                children[ i ]->traverseVisible( callback );

            }

            return *this;
        }

        Object3D& traverseAncestors( Object3DCallback callback ) {
            if ( parent != nullptr ) {

                callback( parent );

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
            for ( int i = 0, int l = children.size(); i < l; i ++ ) {
                children[ i ]->updateMatrixWorld( force );
            }

        }

        Object3D& updateWorldMatrix( bool updateParents, bool updateChildren ) {

            if ( updateParents == true && parent != null ) {

                parent->updateWorldMatrix( true, false );

            }

            if ( matrixAutoUpdate ) updateMatrix();

            if ( this.parent == null ) {
                matrixWorld->copy( *matrix );
            } else {
                matrixWorld->multiplyMatrices( parent->matrixWorld, *matrix );
            }

            // update children
            if ( updateChildren == true ) {
                for ( int i = 0, int l = children.length; i < l; i ++ ) {
                    children[ i ]->updateWorldMatrix( false, true );
                }
            }

        }

};