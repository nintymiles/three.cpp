//
// Created by SeanR on 2022/12/7.
//

#ifndef THREE_CPP_SRC_RENDERS_GL_RENDER_H
#define THREE_CPP_SRC_RENDERS_GL_RENDER_H

#include "constants.h"
#include "vector4.h"
#include "frustum.h"
#include "matrix4.h"
#include "vector2.h"
#include "vector3.h"
#include "scenes/scene.h"
#include "gl_render_target.h"
//#include ""

class GLRender {
public:
    // clearing
    bool autoClear = true;
    bool autoClearColor = true;
    bool autoClearDepth = true;
    bool autoClearStencil = true;

    // scene graph
    bool sortObjects = true;

    // user-defined clipping
    //this.clippingPlanes = [];
    bool localClippingEnabled = false;

    // physically based shading
    int outputEncoding = LinearEncoding;

    // physical lights
    bool physicallyCorrectLights = false;

    // tone mapping
    bool toneMapping = NoToneMapping;
    int toneMappingExposure = 1.0;




    GLRender(){
//        _depth = parameters.depth !== undefined ? parameters.depth : true,
//        _stencil = parameters.stencil !== undefined ? parameters.stencil : true,
//        _antialias = parameters.antialias !== undefined ? parameters.antialias : false,
//        _premultipliedAlpha = parameters.premultipliedAlpha !== undefined ? parameters.premultipliedAlpha : true,
//        _preserveDrawingBuffer = parameters.preserveDrawingBuffer !== undefined ? parameters.preserveDrawingBuffer : false,
//        _powerPreference = parameters.powerPreference !== undefined ? parameters.powerPreference : 'default',
//        _failIfMajorPerformanceCaveat = parameters.failIfMajorPerformanceCaveat !== undefined ? parameters.failIfMajorPerformanceCaveat : false;

//        let _alpha;
//
//        if ( parameters.context !== undefined ) {
//
//            _alpha = _context.getContextAttributes().alpha;
//
//        } else {
//
//            _alpha = parameters.alpha !== undefined ? parameters.alpha : false;
//
//        }
//
//        let currentRenderList = null;
//        let currentRenderState = null;
//
//        // render() can be called from within a callback triggered by another render.
//        // We track this so that the nested render call gets its list and state isolated from the parent render call.
//
//        const renderListStack = [];
//        const renderStateStack = [];
//
//        // public properties
//        // WebGL Render的容器元素一定是canvas
//        this.domElement = _canvas;
//
//        // Debug configuration container
//        this.debug = {
//
//            /**
//             * Enables error checking and reporting when shader programs are being compiled
//             * @type {boolean}
//             */
//            checkShaderErrors: true
//        };



    }

private:
    bool _depth,_stencil,_antialias,_premultipliedAlpha,_preserveDrawingBuffer,_powerPreference,_failIfMajorPerformanceCaveat;

    // internal properties
    //const _this = this;
    bool _isContextLost = false;

    // internal state cache
    int _currentActiveCubeFace = 0;
    int _currentActiveMipmapLevel = 0;
    //let _currentRenderTarget = null;
    int _currentMaterialId = - 1;

//    let _currentCamera = null;

    Vector4d _currentViewport;
    Vector4d _currentScissor;
//    let _currentScissorTest = null;

//    let _width = _canvas.width;
//    let _height = _canvas.height;

    int _pixelRatio = 1;
//    let _opaqueSort = null;
//    let _transparentSort = null;

//    Vector4d _viewport( 0, 0, _width, _height );
//    Vector4d _scissor( 0, 0, _width, _height );
    bool _scissorTest = false;

    // frustum
    Frustum _frustum;

    // clipping
    bool _clippingEnabled = false;
    bool _localClippingEnabled = false;

    // transmission
    //let _transmissionRenderTarget = null;

    // camera matrices cache
    Matrix4 _projScreenMatrix;

    Vector2 _vector2;
    Vector3 _vector3;

    GLRenderTarget* _currentRenderTarget;

    //struct 包含有成员函数后，不适用于指定初始化，通常需要构造函数
    Scene _emptyScene{nullptr,nullptr,nullptr,true };

    int getTargetPixelRatio() {
        return _currentRenderTarget == nullptr ? _pixelRatio : 1;
    }

};


#endif //THREE_CPP_SRC_RENDERS_GL_RENDER_H
