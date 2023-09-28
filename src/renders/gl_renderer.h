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
#include "gl_buffer_renderer.h"
#include "buffer_geometry.h"
#include "common_types.h"
#include "materials/mesh_basic_material.h"

#include <algorithm>
#include <typeinfo>

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

    void initGLContext(){
        state = std::make_shared<GLState>();
    }

    GLRenderer& renderBufferDirect( const Camera& camera, const Scene& scene, const BufferGeometry<int,double>& geometry, Material& material, Object3D& object, std::shared_ptr<threecpp::GeometryGroup> group ) {
        using GIndex = shared_ptr<BufferAttribute<int>>;
        //if ( scene == nullptr ) scene = _emptyScene; // renderBufferDirect second parameter used to be fog (could be null)
        const bool frontFaceCW = (object.isMesh() && object.matrixWorld->determinant() < 0);

        GLProgram program = setProgram(camera, scene, geometry, material, object);

        //state->setMaterial( material, frontFaceCW );

        //
        GIndex index = geometry.index;
        int rangeFactor = 1;

        //typeid为RTTI操作符
        if (typeid(*this) == typeid(MeshBasicMaterial)) {
            MeshBasicMaterial& meshMaterial = dynamic_cast<MeshBasicMaterial&>(material);
            if (material.wireframe == true) {
                //todo
                //index = geometries.getWireframeAttribute(geometry);
                rangeFactor = 2;
            }
        }

        //
        bindingStates.setup( object, material, program, geometry, index );

        std::shared_ptr<BufferAttribute<int>> attribute;
        std::shared_ptr<GLBufferRenderer> renderer = bufferRenderer;

        if ( index != null ) {

            attribute = attributes.get( index );

            renderer = indexedBufferRenderer;
            renderer.setIndex( attribute );

        }

        //
        threecpp::Range drawRange = geometry.drawRange;
        std::shared_ptr<BufferAttribute<double>> position = geometry.attributes.at("position");

        int drawStart = drawRange.start * rangeFactor;
        int drawEnd = ( drawRange.start + drawRange.count ) * rangeFactor;

        if ( group != nullptr ) {
            drawStart = std::max<int>( drawStart, group->start * rangeFactor );
            drawEnd = std::min<int>( drawEnd, ( group->start + group->count ) * rangeFactor );

        }

        if ( index != nullptr ) {
            drawStart = std::max<int>( drawStart, 0 );
            drawEnd = std::min<int>( drawEnd, index->count() );

        } else if (  position != nullptr ) {
            drawStart = std::max<int>( drawStart, 0 );
            drawEnd = std::min<int>( drawEnd, position->count() );

        }

        int drawCount = drawEnd - drawStart;

        if ( drawCount < 0 || drawCount == std::numeric_limits<int>::max() ) return *this;


        //
        if ( object.isMesh ) {

            if ( material.wireframe == true ) {
                state.setLineWidth( material.wireframeLinewidth * getTargetPixelRatio() );
                renderer.setMode( _gl.LINES );
            } else {
                renderer.setMode( _gl.TRIANGLES );
            }

        } else if ( object.isLine ) {
            let lineWidth = material.linewidth;

            if ( lineWidth == undefined ) lineWidth = 1; // Not using Line*Material

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
            const maxInstanceCount = geometry._maxInstanceCount != undefined ? geometry._maxInstanceCount : Infinity;
            const instanceCount = Math.min( geometry.instanceCount, maxInstanceCount );

            renderer.renderInstances( drawStart, drawCount, instanceCount );
        } else {
            renderer.render( drawStart, drawCount );
        }

    };

    function setProgram( camera, scene, geometry, material, object ) {

        if ( scene.isScene !== true ) scene = _emptyScene; // scene could be a Mesh, Line, Points, ...

        textures.resetTextureUnits();

        const fog = scene.fog;
        const environment = material.isMeshStandardMaterial ? scene.environment : null;
        const encoding = ( _currentRenderTarget === null ) ? _this.outputEncoding : ( _currentRenderTarget.isXRRenderTarget === true ? _currentRenderTarget.texture.encoding : LinearEncoding );
        const envMap = ( material.isMeshStandardMaterial ? cubeuvmaps : cubemaps ).get( material.envMap || environment );
        const vertexAlphas = material.vertexColors === true && !! geometry.attributes.color && geometry.attributes.color.itemSize === 4;
        const vertexTangents = !! material.normalMap && !! geometry.attributes.tangent;
        const morphTargets = !! geometry.morphAttributes.position;
        const morphNormals = !! geometry.morphAttributes.normal;
        const morphColors = !! geometry.morphAttributes.color;
        const toneMapping = material.toneMapped ? _this.toneMapping : NoToneMapping;

        const morphAttribute = geometry.morphAttributes.position || geometry.morphAttributes.normal || geometry.morphAttributes.color;
        const morphTargetsCount = ( morphAttribute !== undefined ) ? morphAttribute.length : 0;

        const materialProperties = properties.get( material );
        const lights = currentRenderState.state.lights;

        if ( _clippingEnabled === true ) {

            if ( _localClippingEnabled === true || camera !== _currentCamera ) {

                const useCache =
                        camera === _currentCamera &&
                                   material.id === _currentMaterialId;

                // we might want to call this function with some ClippingGroup
                // object instead of the material, once it becomes feasible
                // (#8465, #8379)
                clipping.setState( material, camera, useCache );

            }

        }

        //

        let needsProgramChange = false;

        if ( material.version === materialProperties.__version ) {

            if ( materialProperties.needsLights && ( materialProperties.lightsStateVersion !== lights.state.version ) ) {

                needsProgramChange = true;

            } else if ( materialProperties.outputEncoding !== encoding ) {

                needsProgramChange = true;

            } else if ( object.isInstancedMesh && materialProperties.instancing === false ) {

                needsProgramChange = true;

            } else if ( ! object.isInstancedMesh && materialProperties.instancing === true ) {

                needsProgramChange = true;

            } else if ( object.isSkinnedMesh && materialProperties.skinning === false ) {

                needsProgramChange = true;

            } else if ( ! object.isSkinnedMesh && materialProperties.skinning === true ) {

                needsProgramChange = true;

            } else if ( materialProperties.envMap !== envMap ) {

                needsProgramChange = true;

            } else if ( material.fog === true && materialProperties.fog !== fog ) {

                needsProgramChange = true;

            } else if ( materialProperties.numClippingPlanes !== undefined &&
                                                                 ( materialProperties.numClippingPlanes !== clipping.numPlanes ||
                                                                                                            materialProperties.numIntersection !== clipping.numIntersection ) ) {

                needsProgramChange = true;

            } else if ( materialProperties.vertexAlphas !== vertexAlphas ) {

                needsProgramChange = true;

            } else if ( materialProperties.vertexTangents !== vertexTangents ) {

                needsProgramChange = true;

            } else if ( materialProperties.morphTargets !== morphTargets ) {

                needsProgramChange = true;

            } else if ( materialProperties.morphNormals !== morphNormals ) {

                needsProgramChange = true;

            } else if ( materialProperties.morphColors !== morphColors ) {

                needsProgramChange = true;

            } else if ( materialProperties.toneMapping !== toneMapping ) {

                needsProgramChange = true;

            } else if ( capabilities.isWebGL2 === true && materialProperties.morphTargetsCount !== morphTargetsCount ) {

                needsProgramChange = true;

            }

        } else {

            needsProgramChange = true;
            materialProperties.__version = material.version;

        }

        //

        let program = materialProperties.currentProgram;

        if ( needsProgramChange === true ) {

            program = getProgram( material, scene, object );

        }

        let refreshProgram = false;
        let refreshMaterial = false;
        let refreshLights = false;

        const p_uniforms = program.getUniforms(),
                m_uniforms = materialProperties.uniforms;

        if ( state.useProgram( program.program ) ) {

            refreshProgram = true;
            refreshMaterial = true;
            refreshLights = true;

        }

        if ( material.id !== _currentMaterialId ) {

            _currentMaterialId = material.id;

            refreshMaterial = true;

        }

        if ( refreshProgram || _currentCamera !== camera ) {

            p_uniforms.setValue( _gl, 'projectionMatrix', camera.projectionMatrix );

            if ( capabilities.logarithmicDepthBuffer ) {

                p_uniforms.setValue( _gl, 'logDepthBufFC',
                                     2.0 / ( Math.log( camera.far + 1.0 ) / Math.LN2 ) );

            }

            if ( _currentCamera !== camera ) {

                _currentCamera = camera;

                // lighting uniforms depend on the camera so enforce an update
                // now, in case this material supports lights - or later, when
                // the next material that does gets activated:

                refreshMaterial = true;		// set to true on material change
                refreshLights = true;		// remains set until update done

            }

            // load material specific uniforms
            // (shader material also gets them for the sake of genericity)

            if ( material.isShaderMaterial ||
                 material.isMeshPhongMaterial ||
                 material.isMeshToonMaterial ||
                 material.isMeshStandardMaterial ||
                 material.envMap ) {

                const uCamPos = p_uniforms.map.cameraPosition;

                if ( uCamPos !== undefined ) {

                    uCamPos.setValue( _gl,
                                      _vector3.setFromMatrixPosition( camera.matrixWorld ) );

                }

            }

            if ( material.isMeshPhongMaterial ||
                 material.isMeshToonMaterial ||
                 material.isMeshLambertMaterial ||
                 material.isMeshBasicMaterial ||
                 material.isMeshStandardMaterial ||
                 material.isShaderMaterial ) {

                p_uniforms.setValue( _gl, 'isOrthographic', camera.isOrthographicCamera === true );

            }

            if ( material.isMeshPhongMaterial ||
                 material.isMeshToonMaterial ||
                 material.isMeshLambertMaterial ||
                 material.isMeshBasicMaterial ||
                 material.isMeshStandardMaterial ||
                 material.isShaderMaterial ||
                 material.isShadowMaterial ||
                 object.isSkinnedMesh ) {

                p_uniforms.setValue( _gl, 'viewMatrix', camera.matrixWorldInverse );

            }

        }

        // skinning and morph target uniforms must be set even if material didn't change
        // auto-setting of texture unit for bone and morph texture must go before other textures
        // otherwise textures used for skinning and morphing can take over texture units reserved for other material textures

        if ( object.isSkinnedMesh ) {

            p_uniforms.setOptional( _gl, object, 'bindMatrix' );
            p_uniforms.setOptional( _gl, object, 'bindMatrixInverse' );

            const skeleton = object.skeleton;

            if ( skeleton ) {

                if ( capabilities.floatVertexTextures ) {

                    if ( skeleton.boneTexture === null ) skeleton.computeBoneTexture();

                    p_uniforms.setValue( _gl, 'boneTexture', skeleton.boneTexture, textures );
                    p_uniforms.setValue( _gl, 'boneTextureSize', skeleton.boneTextureSize );

                } else {

                    console.warn( 'THREE.WebGLRenderer: SkinnedMesh can only be used with WebGL 2. With WebGL 1 OES_texture_float and vertex textures support is required.' );

                }

            }

        }

        const morphAttributes = geometry.morphAttributes;

        if ( morphAttributes.position !== undefined || morphAttributes.normal !== undefined || ( morphAttributes.color !== undefined && capabilities.isWebGL2 === true ) ) {

            morphtargets.update( object, geometry, material, program );

        }


        if ( refreshMaterial || materialProperties.receiveShadow !== object.receiveShadow ) {

            materialProperties.receiveShadow = object.receiveShadow;
            p_uniforms.setValue( _gl, 'receiveShadow', object.receiveShadow );

        }

        if ( refreshMaterial ) {

            p_uniforms.setValue( _gl, 'toneMappingExposure', _this.toneMappingExposure );

            if ( materialProperties.needsLights ) {

                // the current material requires lighting info

                // note: all lighting uniforms are always set correctly
                // they simply reference the renderer's state for their
                // values
                //
                // use the current material's .needsUpdate flags to set
                // the GL state when required

                markUniformsLightsNeedsUpdate( m_uniforms, refreshLights );

            }

            // refresh uniforms common to several materials

            if ( fog && material.fog === true ) {

                materials.refreshFogUniforms( m_uniforms, fog );

            }

            materials.refreshMaterialUniforms( m_uniforms, material, _pixelRatio, _height, _transmissionRenderTarget );

            WebGLUniforms.upload( _gl, materialProperties.uniformsList, m_uniforms, textures );

        }

        if ( material.isShaderMaterial && material.uniformsNeedUpdate === true ) {

            WebGLUniforms.upload( _gl, materialProperties.uniformsList, m_uniforms, textures );
            material.uniformsNeedUpdate = false;

        }

        if ( material.isSpriteMaterial ) {

            p_uniforms.setValue( _gl, 'center', object.center );

        }

        // common matrices

        p_uniforms.setValue( _gl, 'modelViewMatrix', object.modelViewMatrix );
        p_uniforms.setValue( _gl, 'normalMatrix', object.normalMatrix );
        p_uniforms.setValue( _gl, 'modelMatrix', object.matrixWorld );

        return program;

    }

    GLRenderer& initGLContext(){
        glInfo = std::make_shared<GLInfo>();
        bufferRenderer = std::make_shared<GLBufferRenderer>(glInfo);

        return *this;
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
    std::shared_ptr<GLBufferRenderer> bufferRenderer;
    std::shared_ptr<GLInfo> glInfo;
    std::shared_ptr<GLState> state;


    //struct 包含有成员函数后，不适用于指定初始化，通常需要构造函数
    Scene _emptyScene{nullptr,nullptr,nullptr,true };

    int getTargetPixelRatio() {
        return _currentRenderTarget == nullptr ? _pixelRatio : 1;
    }

};


#endif //THREE_CPP_SRC_RENDERS_GL_RENDERER_H