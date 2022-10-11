#ifndef SRC_CAMERAS_CAMERA_H_
#define SRC_CAMERAS_CAMERA_H_

#include "matrix4.h"
#include "object_3d.h"

#include <string>
#include <memory>

using std::string;
class Camera:public Object3D {
    public:
        string type;
        std::shared_ptr<Matrix4> matrixWorldInverse;
        std::shared_ptr<Matrix4> projectionMatrix;
        std::shared_ptr<Matrix4> projectionMatrixInverse;


        //默认情况，父类默认构造器被默认调用，若调用其他构造器，则在类初始化列表中调用对应构造器。
        Camera():type("Camera"),
                matrixWorldInverse(std::make_shared<Matrix4>()),
                projectionMatrix(std::make_shared<Matrix4>()),
                projectionMatrixInverse(std::make_shared<Matrix4>()){}

        Camera(const Camera& camera):type(camera.type),
                matrixWorldInverse(camera.matrixWorldInverse),
                projectionMatrix(camera.projectionMatrix),
                projectionMatrixInverse(camera.projectionMatrixInverse){}

        //cpp中clone的实现使用copy constructor即可
        // clone() {
        //     return new this.constructor().copy( this );
        // }

         Camera& copy( Camera& source, bool recursive ) {
             Object3D::copy( source, recursive );

             matrixWorldInverse->copy( *source.matrixWorldInverse );

             projectionMatrix->copy(* source.projectionMatrix );
             projectionMatrixInverse->copy( *source.projectionMatrixInverse );

             return *this;
         }

        Vector3& getWorldDirection( Vector3& target ) {
            this->updateWorldMatrix( true, false );
            //const e = this.matrixWorld.elements;

            return target.set( - matrixWorld->elements[ 8 ], - matrixWorld->elements[ 9 ], - matrixWorld->elements[ 10 ] ).normalize();
        }

        Camera& updateMatrixWorld( bool force ) {
            //派生类对象中，父类被重载的函数直接用范围操作符调用
            Object3D::updateMatrixWorld( force );

            this->matrixWorldInverse->copy( *matrixWorld ).invert();
            return *this;
        }

        Camera& updateWorldMatrix( bool updateParents, bool updateChildren ) {
            Object3D::updateWorldMatrix( updateParents, updateChildren );
            this->matrixWorldInverse->copy( *matrixWorld ).invert();

            return *this;
        }

        virtual bool isPerspectiveCamera();

        virtual bool isOrthographicCamera();


};

struct ViewOffset{
    bool enabled;
    double fullWidth;
    double fullHeight;
    double offsetX;
    double offsetY;
    double width;
    double height;
};


#endif /* SRC_CAMERAS_CAMERA_H_ */
