#ifndef SRC_CAMERAS_CAMERA_H_
#define SRC_CAMERAS_CAMERA_H_

#include "matrix4.h"
#include "object_3d.h"

#include <string>

using std::string;
class Camera:public Object3D {
    public:
        string type;
        Matrix4 matrixWorldInverse;
        Matrix4 projectionMatrix;
        Matrix4 projectionMatrixInverse;

        //默认情况，父类默认构造器被默认调用，若调用其他构造器，则在类初始化列表中调用对应构造器。
        Camera():type("Camera"),
                matrixWorldInverse(Matrix4()),
                projectionMatrix(Matrix4()),
                projectionMatrixInverse(Matrix4()){}

        Camera(const Camera& camera):type(camera.type),
                matrixWorldInverse(camera.matrixWorldInverse),
                projectionMatrix(camera.projectionMatrix),
                projectionMatrixInverse(camera.projectionMatrixInverse){}

        //cpp中clone的实现使用copy constructor即可
        // clone() {
        //     return new this.constructor().copy( this );
        // }

        // copy( source, recursive ) {

        //     super.copy( source, recursive );

        //     this.matrixWorldInverse.copy( source.matrixWorldInverse );

        //     this.projectionMatrix.copy( source.projectionMatrix );
        //     this.projectionMatrixInverse.copy( source.projectionMatrixInverse );

        //     return this;

        // }

        Vector3& getWorldDirection( Vector3& target ) {
            this->updateWorldMatrix( true, false );
            //const e = this.matrixWorld.elements;

            return target.set( - matrixWorld->elements[ 8 ], - matrixWorld->elements[ 9 ], - matrixWorld->elements[ 10 ] ).normalize();
        }

        Camera& updateMatrixWorld( bool force ) {
            //派生类对象中，父类被重载的函数直接用范围操作符调用
            Object3D::updateMatrixWorld( force );

            this->matrixWorldInverse.copy( *matrixWorld ).invert();
            return *this;
        }

        Camera& updateWorldMatrix( bool updateParents, bool updateChildren ) {
            Object3D::updateWorldMatrix( updateParents, updateChildren );
            this->matrixWorldInverse.copy( *matrixWorld ).invert();

            return *this;
        }



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
