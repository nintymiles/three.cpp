//
// Created by SeanR on 2022/12/7.
//

#ifndef THREE_CPP_SRC_RENDERS_GL_RENDERER_H
#define THREE_CPP_SRC_RENDERS_GL_RENDERER_H

#include "constants.h"
#include "vector4.h"
#include "frustum.h"
#include "matrix4.h"
#include "vector2.h"
#include "vector3.h"
#include "scenes/scene.h"
#include "gl_render_target.h"
#include "gl_state.h"
#include "cameras/camera.h"
//#include ""

struct GLRendererParameter{
    bool depth = true,stencil = true,antialias = false,
            premultipliedAlpha = true,preserveDrawingBuffer =  false, failIfMajorPerformanceCaveat = false;

    bool alpha = false;
    std::string powerPreference = "default";
};

class GLRenderer {
public:
    // clearing options
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
    float toneMappingExposure = 1.0;


//    let currentRenderList = null;
//    let currentRenderState = null;
    std::vector<GLState> currentRenderState;


    // render() can be called from within a callback triggered by another render.
    // We track this so that the nested render call gets its list and state isolated from the parent render call.
//    const renderListStack = [];
//    const renderStateStack = [];

    // debug configuration container
    struct RenderDebug{
        bool checkShaderErrors = true;
    } debug;


    GLRenderer(const GLRendererParameter& parameters){
        _depth = parameters.depth ;
        _stencil = parameters.stencil ;
        _antialias = parameters.antialias ;
        _premultipliedAlpha = parameters.premultipliedAlpha ;
        _preserveDrawingBuffer = parameters.preserveDrawingBuffer ;
        //_powerPreference = parameters.powerPreference ;
        _failIfMajorPerformanceCaveat = parameters.failIfMajorPerformanceCaveat ;

//        let _alpha;
//        if ( parameters.context !== undefined ) {
//
//            _alpha = _context.getContextAttributes().alpha;
//
//        } else {
//
//            _alpha = parameters.alpha ;
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

    //rendering
    void render(Scene & scene,Camera& camera){

            if ( camera.isCamera != true ) {
                std::cout << "THREECPP.GLRenderer.render: camera is not an instance of THREE.Camera." << std::endl;
                return;
            }

            if ( _isContextLost == true ) return;

            // update scene graph
            if ( scene.autoUpdate == true ) scene.updateMatrixWorld(true);

            // update camera matrices and frustum
            if ( camera.parent == nullptr ) camera.updateMatrixWorld(true);

//            if ( xr.enabled == true && xr.isPresenting == true ) {
//                if ( xr.cameraAutoUpdate == true ) xr.updateCamera( camera );
//
//                camera = xr.getCamera(); // use XR camera for rendering
//            }


//            if ( scene.isScene == true ) scene.onBeforeRender( _this, scene, camera, _currentRenderTarget );

//            currentRenderState = renderStates.get( scene, renderStateStack.length );
//            currentRenderState.init();
//
//            renderStateStack.push( currentRenderState );
//
//            _projScreenMatrix.multiplyMatrices( camera.projectionMatrix, camera.matrixWorldInverse );
//            _frustum.setFromProjectionMatrix( _projScreenMatrix );
//
//            _localClippingEnabled = this.localClippingEnabled;
//            _clippingEnabled = clipping.init( this.clippingPlanes, _localClippingEnabled, camera );
//
//            currentRenderList = renderLists.get( scene, renderListStack.length );
//            currentRenderList.init();
//
//            renderListStack.push( currentRenderList );
//
//            projectObject( scene, camera, 0, _this.sortObjects );
//
//            currentRenderList.finish();
//
//            if ( _this.sortObjects === true ) {
//
//                currentRenderList.sort( _opaqueSort, _transparentSort );
//
//            }
//
//            //
//
//            if ( _clippingEnabled === true ) clipping.beginShadows();
//
//            const shadowsArray = currentRenderState.state.shadowsArray;
//
//            shadowMap.render( shadowsArray, scene, camera );
//
//            if ( _clippingEnabled === true ) clipping.endShadows();
//
//            //
//
//            if ( this.info.autoReset === true ) this.info.reset();
//
//            //
//
//            background.render( currentRenderList, scene );
//
//            // render scene
//
//            currentRenderState.setupLights( _this.physicallyCorrectLights );
//
//            if ( camera.isArrayCamera ) {
//
//                const cameras = camera.cameras;
//
//                for ( let i = 0, l = cameras.length; i < l; i ++ ) {
//
//                    const camera2 = cameras[ i ];
//
//                    renderScene( currentRenderList, scene, camera2, camera2.viewport );
//
//                }
//
//            } else {
//
//                renderScene( currentRenderList, scene, camera );
//
//            }
//
//            //
//
//            if ( _currentRenderTarget !== null ) {
//
//                // resolve multisample renderbuffers to a single-sample texture if necessary
//
//                textures.updateMultisampleRenderTarget( _currentRenderTarget );
//
//                // Generate mipmap if we're using any kind of mipmap filtering
//
//                textures.updateRenderTargetMipmap( _currentRenderTarget );
//
//            }
//
//            //
//
//            if ( scene.isScene === true ) scene.onAfterRender( _this, scene, camera );
//
//            // _gl.finish();
//
//            bindingStates.resetDefaultState();
//            _currentMaterialId = - 1;
//            _currentCamera = null;
//
//            renderStateStack.pop();
//
//            if ( renderStateStack.length > 0 ) {
//
//                currentRenderState = renderStateStack[ renderStateStack.length - 1 ];
//
//            } else {
//
//                currentRenderState = null;
//
//            }
//
//            renderListStack.pop();
//
//            if ( renderListStack.length > 0 ) {
//
//                currentRenderList = renderListStack[ renderListStack.length - 1 ];
//
//            } else {
//
//                currentRenderList = null;
//
//            }
    }
    GLRenderer& renderBufferDirect( const Camera& camera, const Scene& scene, geometry, Material& material, Object3D& object, group ) {

        //if ( scene == nullptr ) scene = _emptyScene; // renderBufferDirect second parameter used to be fog (could be null)

        const frontFaceCW = ( object.isMesh && object.matrixWorld->determinant() < 0 );

        const program = setProgram( camera, scene, geometry, material, object );

        state.setMaterial( material, frontFaceCW );

        //

        let index = geometry.index;
        let rangeFactor = 1;

        if ( material.wireframe === true ) {

            index = geometries.getWireframeAttribute( geometry );
            rangeFactor = 2;

        }

        //

        const drawRange = geometry.drawRange;
        const position = geometry.attributes.position;

        let drawStart = drawRange.start * rangeFactor;
        let drawEnd = ( drawRange.start + drawRange.count ) * rangeFactor;

        if ( group !== null ) {

            drawStart = Math.max( drawStart, group.start * rangeFactor );
            drawEnd = Math.min( drawEnd, ( group.start + group.count ) * rangeFactor );

        }

        if ( index !== null ) {

            drawStart = Math.max( drawStart, 0 );
            drawEnd = Math.min( drawEnd, index.count );

        } else if ( position !== undefined && position !== null ) {

            drawStart = Math.max( drawStart, 0 );
            drawEnd = Math.min( drawEnd, position.count );

        }

        const drawCount = drawEnd - drawStart;

        if ( drawCount < 0 || drawCount === Infinity ) return;

        //

        bindingStates.setup( object, material, program, geometry, index );

        let attribute;
        let renderer = bufferRenderer;

        if ( index !== null ) {

            attribute = attributes.get( index );

            renderer = indexedBufferRenderer;
            renderer.setIndex( attribute );

        }

        //

        if ( object.isMesh ) {

            if ( material.wireframe === true ) {

                state.setLineWidth( material.wireframeLinewidth * getTargetPixelRatio() );
                renderer.setMode( _gl.LINES );

            } else {

                renderer.setMode( _gl.TRIANGLES );

            }

        } else if ( object.isLine ) {

            let lineWidth = material.linewidth;

            if ( lineWidth === undefined ) lineWidth = 1; // Not using Line*Material

            state.setLineWidth( lineWidth * getTargetPixelRatio() );

            if ( object.isLineSegments ) {

                renderer.setMode( _gl.LINES );

            } else if ( object.isLineLoop ) {

                renderer.setMode( _gl.LINE_LOOP );

            } else {

                renderer.setMode( _gl.LINE_STRIP );

            }

        } else if ( object.isPoints ) {

            renderer.setMode( _gl.POINTS );

        } else if ( object.isSprite ) {

            renderer.setMode( _gl.TRIANGLES );

        }

        if ( object.isInstancedMesh ) {

            renderer.renderInstances( drawStart, drawCount, object.count );

        } else if ( geometry.isInstancedBufferGeometry ) {

            const maxInstanceCount = geometry._maxInstanceCount !== undefined ? geometry._maxInstanceCount : Infinity;
            const instanceCount = Math.min( geometry.instanceCount, maxInstanceCount );

            renderer.renderInstances( drawStart, drawCount, instanceCount );

        } else {

            renderer.render( drawStart, drawCount );

        }

    };

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


#endif //THREE_CPP_SRC_RENDERS_GL_RENDERER_H
