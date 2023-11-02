#ifndef SRC_CAMERAS_CAMERA_H_
#define SRC_CAMERAS_CAMERA_H_

#include "matrix4.h"
#include "vector3.h"
#include "vector4.h"
#include "object_3d.h"

#include <string>
#include <memory>

class Camera : public Object3D {
public:
    struct CameraView {
        bool enabled;
        float fullWidth;
        float fullHeight;
        float offsetX;
        float offsetY;
        float width;
        float height;
    };
    using sptr = std::shared_ptr<Camera>;

    float bottom;

    float left;

    float top;

    float zoom=1;

    float cameraRight;

    /**
    * Camera frustum vertical field of view, from bottom to top of view, in degrees.
    */
    float fov = 45.0f;
    /**
    * Camera frustum aspect ratio, window width divided by window height.
    */
    float aspect = 1.0f;
    /**
    * Camera frustum far plane.
    */
    float _far = 2000.0f;
    /**
    * Camera frustum near plane.
    */
    float _near = 0.1f;

    Vector4 viewport;

    Camera();

    Camera(const Camera& camera);

    static sptr create() {
        return std::make_shared<Camera>();
    }
    /**
     * This is the inverse of matrixWorld. MatrixWorld contains the Matrix which has the world transform of the Camera.
     */
    Matrix4 matrixWorldInverse;
    /**
     * This is the matrix which contains the projection.
     */
    Matrix4 projectionMatrix;
    /**
     * This is the inverse of projectionMatrix.
     */
    Matrix4 projectionMatrixInverse;

    bool isArrayCamera = false;

    bool isPerspective = false;

    bool isOrthographic = false;

    virtual Vector3& getWorldDirection(Vector3& target) override;

    virtual void updateMatrixWorld(bool force = false) override;

    virtual void updateWorldMatrix(bool updateParents = false, bool updateChildren = false) override;

    virtual void setViewOffset(float fullWidth, float fullHeight, float x, float y, float width, float height) {}

    virtual void clearViewOffset() {}

    virtual Camera& copy(Camera& source, bool recursive=true);

    virtual Camera* clone();

    virtual void updateProjectionMatrix();
};

//using std::string;
//class Camera:public Object3D {
//    public:
//        using sptr = std::make_shared<Camera>;
//
//        bool isCamera = true;
//        string type;
//        std::shared_ptr<Matrix4> matrixWorldInverse;
//        std::shared_ptr<Matrix4> projectionMatrix;
//        std::shared_ptr<Matrix4> projectionMatrixInverse;
//
//
//        //默认情况，父类默认构造器被默认调用，若调用其他构造器，则在类初始化列表中调用对应构造器。
//        Camera():type("Camera"),
//                matrixWorldInverse(std::make_shared<Matrix4>()),
//                projectionMatrix(std::make_shared<Matrix4>()),
//                projectionMatrixInverse(std::make_shared<Matrix4>()){}
//
//        Camera(const Camera& camera):type(camera.type),
//                matrixWorldInverse(camera.matrixWorldInverse),
//                projectionMatrix(camera.projectionMatrix),
//                projectionMatrixInverse(camera.projectionMatrixInverse){}
//
//        //cpp中clone的实现使用copy constructor即可
//        // clone() {
//        //     return new this.constructor().copy( this );
//        // }
//
//         Camera& copy( Camera& source, bool recursive ) {
//             Object3D::copy( source, recursive );
//
//             matrixWorldInverse->copy( *source.matrixWorldInverse );
//
//             projectionMatrix->copy(* source.projectionMatrix );
//             projectionMatrixInverse->copy( *source.projectionMatrixInverse );
//
//             return *this;
//         }
//
//        Vector3& getWorldDirection( Vector3& target ) {
//            this->updateWorldMatrix( true, false );
//            //const e = this.matrixWorld.elements;
//
//            return target.set( - matrixWorld->elements[ 8 ], - matrixWorld->elements[ 9 ], - matrixWorld->elements[ 10 ] ).normalize();
//        }
//
//        Camera& updateMatrixWorld( bool force ) {
//            //派生类对象中，父类被重载的函数直接用范围操作符调用
//            Object3D::updateMatrixWorld( force );
//
//            this->matrixWorldInverse->copy( *matrixWorld ).invert();
//            return *this;
//        }
//
//        Camera& updateWorldMatrix( bool updateParents, bool updateChildren ) {
//            Object3D::updateWorldMatrix( updateParents, updateChildren );
//            this->matrixWorldInverse->copy( *matrixWorld ).invert();
//
//            return *this;
//        }
//
//        virtual bool isPerspectiveCamera();
//
//        virtual bool isOrthographicCamera();
//
//
//};
//
//struct ViewOffset{
//    bool enabled;
//    double fullWidth;
//    double fullHeight;
//    double offsetX;
//    double offsetY;
//    double width;
//    double height;
//};
//
//using CameraSptr = std::shared_ptr<Camera>;

#endif /* SRC_CAMERAS_CAMERA_H_ */
