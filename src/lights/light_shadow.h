//
// Created by seanren on 11/20/2022.
//

#ifndef THREE_CPP_LIGHT_SHADOW_H
#define THREE_CPP_LIGHT_SHADOW_H

#include "matrix4.h"
#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "frustum.h"
#include "cameras/camera.h"

#include "gl_render_target.h"

#include <map>
#include <memory>


class Light;
class Camera;

enum class LightShadowType : unsigned {
    LightShadow=0,
    DirectionalLightShadow=1,
    PointLightShadow=2,
    SpotLightShadow=3
};

namespace shadow {
    static size_t _shadowId = 0;
}
using namespace shadow;

class LightShadow {

public:
    using sptr = std::shared_ptr<LightShadow>;

    size_t id;

    bool autoUpdate = true;

    bool needsUpdate = false;

    LightShadowType lightShadowType;

    Frustum frustum;

    Vector2 frameExtents = Vector2(1, 1);

    unsigned viewportCount = 1;

    std::vector<Vector4> viewports = { {Vector4(0,0,1,1)} };

    Camera::sptr camera;

    float shadowBias = 0.0f;

    float shadowNormalBias = 0;

    float shadowRadius = 1.0f;

    Vector2 shadowMapSize = Vector2(512,512);

    GLRenderTarget::sptr shadowMap = nullptr;

    GLRenderTarget::sptr shadowMapPass = nullptr;

    Matrix4 shadowMatrix;

    float shadowCameraNear;

    float shadowCameraFar;


    LightShadow(): id(_shadowId++),lightShadowType(LightShadowType::LightShadow){}

    LightShadow(const Camera::sptr& camera) :id(_shadowId++), camera(camera), lightShadowType(LightShadowType::LightShadow) {}

    LightShadow(const LightShadow& source);

    static sptr create() {
        return std::make_shared<LightShadow>();
    }

    static sptr create(const Camera::sptr& camera) {
        return std::make_shared<LightShadow>(camera);
    }

    virtual LightShadow& copy(const LightShadow& source);

    virtual LightShadow* clone();

    virtual void updateMatrices(const Light& light, int viewportIndex=-1);

    Vector2& getFrameExtents() {
        return frameExtents;
    }

    unsigned getViewportCount() {
        return viewportCount;
    }

    Vector4& getViewport(int viewportIndex) {
        return viewports[viewportIndex];
    }

    Frustum& getFrustum() {
        return frustum;
    }
};



////const _projScreenMatrix = /*@__PURE__*/ new Matrix4();
////const _lightPositionWorld = /*@__PURE__*/ new Vector3();
////const _lookTarget = /*@__PURE__*/ new Vector3();
//Vector3 _lightShadow_lightPositionWorld;
//Vector3 _lightShadow_lookTarget;
//Matrix4 _lightShadow_projScreenMatrix;
//
//class LightShadow{
//public:
//    std::shared_ptr<Camera> camera;
//
//    double bias = 0;
//    double normalBias = 0;
//    double radius = 1;
//    int blurSamples = 8;
//
//    Vector2 mapSize{512,512};
//
//    Matrix4 matrix;
//    std::map<std::string,double> map;
//    bool mapPass;
//
//    bool needsUpdate = false;
//    bool autoUpdate = false;
//
//    Frustum _frustum;
//    Vector2 _frameExtents{1,1};
//
//    int _viewportCount = 1;
//    Vector4d _viewports[1] = {Vector4d{0,0,1,1}};
//
//    //if you intend to use dynamic child class,of course pointer type is appropriate
//    LightShadow(std::shared_ptr<Camera> camera):camera(camera){}
//
//    virtual bool isDirectionalLightShadow(){return  false;}
//
//    int getViewportCount() {
//        return _viewportCount;
//    }
//
//    Frustum getFrustum() {
//        return _frustum;
//    }
//
//    LightShadow& updateMatrices( Light &light ) {
//
//        const Camera& shadowCamera = *(this->camera);
//        const Matrix4& shadowMatrix = this->matrix;
//
//        _lightShadow_lightPositionWorld.setFromMatrixPosition( *light.matrixWorld );
//        shadowCamera.position->copy( _lightShadow_lightPositionWorld );
//
//        //if light is DirectionalLight
////        _lightShadow_lookTarget.setFromMatrixPosition( light.target.matrixWorld );
////        shadowCamera.lookAt( _lightShadow_lookTarget );
////        shadowCamera.updateMatrixWorld();
//
//        _lightShadow_projScreenMatrix.multiplyMatrices( *shadowCamera.projectionMatrix, *shadowCamera.matrixWorldInverse );
//        _frustum.setFromProjectionMatrix( _lightShadow_projScreenMatrix );
//
//        _lightShadow_projScreenMatrix.set(
//        0.5, 0.0, 0.0, 0.5,
//        0.0, 0.5, 0.0, 0.5,
//        0.0, 0.0, 0.5, 0.5,
//        0.0, 0.0, 0.0, 1.0
//        );
//
//        _lightShadow_projScreenMatrix.multiply( *shadowCamera.projectionMatrix );
//        _lightShadow_projScreenMatrix.multiply( *shadowCamera.matrixWorldInverse );
//
//    }
//
//     Vector4d& getViewport( int viewportIndex ) {
//        return _viewports[ viewportIndex ];
//    }
//
//    Vector2& getFrameExtents() {
//        return _frameExtents;
//    }
//
//    //    dispose() {
////
////        if ( this.map ) {
////
////            this.map.dispose();
////
////        }
////
////        if ( this.mapPass ) {
////
////            this.mapPass.dispose();
////
////        }
////
////    }
//
//};
//
//using LightShadowSptr = std::shared_ptr<LightShadow>;
//
////class LightShadow {
////
////    constructor( camera ) {
////
////            this.camera = camera;
////
////            this.bias = 0;
////            this.normalBias = 0;
////            this.radius = 1;
////            this.blurSamples = 8;
////
////            this.mapSize = new Vector2( 512, 512 );
////
////            this.map = null;
////            this.mapPass = null;
////            this.matrix = new Matrix4();
////
////            this.autoUpdate = true;
////            this.needsUpdate = false;
////
////            this._frustum = new Frustum();
////            this._frameExtents = new Vector2( 1, 1 );
////
////            this._viewportCount = 1;
////
////            this._viewports = [
////
////            new Vector4( 0, 0, 1, 1 )
////
////            ];
////
////    }
////
////    getViewportCount() {
////
////        return this._viewportCount;
////
////    }
////
////    getFrustum() {
////
////        return this._frustum;
////
////    }
////
////    updateMatrices( light ) {
////
////            const shadowCamera = this.camera;
////            const shadowMatrix = this.matrix;
////
////            _lightPositionWorld.setFromMatrixPosition( light.matrixWorld );
////            shadowCamera.position.copy( _lightPositionWorld );
////
////            _lookTarget.setFromMatrixPosition( light.target.matrixWorld );
////            shadowCamera.lookAt( _lookTarget );
////            shadowCamera.updateMatrixWorld();
////
////            _projScreenMatrix.multiplyMatrices( shadowCamera.projectionMatrix, shadowCamera.matrixWorldInverse );
////            this._frustum.setFromProjectionMatrix( _projScreenMatrix );
////
////            shadowMatrix.set(
////            0.5, 0.0, 0.0, 0.5,
////            0.0, 0.5, 0.0, 0.5,
////            0.0, 0.0, 0.5, 0.5,
////            0.0, 0.0, 0.0, 1.0
////            );
////
////            shadowMatrix.multiply( shadowCamera.projectionMatrix );
////            shadowMatrix.multiply( shadowCamera.matrixWorldInverse );
////
////    }
////
////    getViewport( viewportIndex ) {
////
////            return this._viewports[ viewportIndex ];
////
////    }
////
////    getFrameExtents() {
////
////        return this._frameExtents;
////
////    }
////
////    dispose() {
////
////        if ( this.map ) {
////
////            this.map.dispose();
////
////        }
////
////        if ( this.mapPass ) {
////
////            this.mapPass.dispose();
////
////        }
////
////    }
////
////    copy( source ) {
////
////            this.camera = source.camera.clone();
////
////            this.bias = source.bias;
////            this.radius = source.radius;
////
////            this.mapSize.copy( source.mapSize );
////
////            return this;
////
////    }
////
////    clone() {
////
////        return new this.constructor().copy( this );
////
////    }
////
////    toJSON() {
////
////        const object = {};
////
////        if ( this.bias !== 0 ) object.bias = this.bias;
////        if ( this.normalBias !== 0 ) object.normalBias = this.normalBias;
////        if ( this.radius !== 1 ) object.radius = this.radius;
////        if ( this.mapSize.x !== 512 || this.mapSize.y !== 512 ) object.mapSize = this.mapSize.toArray();
////
////        object.camera = this.camera.toJSON( false ).object;
////        delete object.camera.matrix;
////
////        return object;
////
////    }
////
////}



#endif //THREE_CPP_LIGHT_SHADOW_H
