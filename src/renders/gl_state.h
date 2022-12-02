//
// Created by SeanR on 2022/11/24.
//

#ifndef THREE_CPP_SRC_RENDERS_GL_STATE_H
#define THREE_CPP_SRC_RENDERS_GL_STATE_H

#include "vector4.h"
#include "constants.h"
#include "common_types.h"

#include <map>
#include <GLES3/gl3.h>

struct BoundTexture{
    GLenum type;
    GLuint texture;
};

const threecpp::GLViewPort defaultViewPort{0,0,640,480};

class GLState {
public:
    const bool isWebGL2;
    GLState() : isWebGL2(true) {}

    class ColorBuffer {
    public:
        bool locked = false;
        Vector4d color{};

        bool currentColorMask = false;
        Vector4d currentColorClear{0, 0, 0, 0};

        ColorBuffer &setMask(bool colorMask) {
            if (currentColorMask != colorMask && !locked) {
                glColorMask(colorMask, colorMask, colorMask, colorMask);
                currentColorMask = colorMask;
            }
            return *this;
        }

        ColorBuffer &setLocked(bool lock) {
            locked = lock;
            return *this;
        }

        ColorBuffer &setClear(double r, double g, double b, double a, bool premultipliedAlpha) {
            if (premultipliedAlpha == true) {

                r *= a;
                g *= a;
                b *= a;

            }

            color.set(r, g, b, a);

            if (currentColorClear.equals(color) == false) {
                glClearColor(r, g, b, a);
                currentColorClear.copy(color);
            }

            return *this;
        }

        ColorBuffer &reset() {
            locked = false;

            currentColorMask = false;
            currentColorClear.set(-1, 0, 0, 0); // set to invalid state
            return *this;
        }
    };

    class DepthBuffer {
    public:
        bool locked = false;

        bool currentDepthMask = false;
        int currentDepthFunc = 0;
        float currentDepthClear = 0.f;

        DepthBuffer &setTest(bool depthTest){
            if (depthTest) {
                glEnable(GL_DEPTH_TEST);
            }else {
                glDisable(GL_DEPTH_TEST);
            }
            return *this;
        }

        DepthBuffer &setMask( bool depthMask ) {
            if ( currentDepthMask != depthMask && !locked ) {
                glDepthMask( depthMask );
                currentDepthMask = depthMask;
            }
            return *this;
        }

        DepthBuffer &setFunc( int depthFunc ) {
            if ( currentDepthFunc == depthFunc ) {
                if ( depthFunc ) {
                    switch ( depthFunc ) {
                        case NeverDepth:
                            glDepthFunc( GL_NEVER );
                            break;
                        case AlwaysDepth:
                            glDepthFunc( GL_ALWAYS );
                            break;
                        case LessDepth:
                            glDepthFunc( GL_LESS );
                            break;
                        case LessEqualDepth:
                            glDepthFunc( GL_LEQUAL );
                            break;
                        case EqualDepth:
                            glDepthFunc( GL_EQUAL );
                            break;
                        case GreaterEqualDepth:
                            glDepthFunc( GL_GEQUAL );
                            break;
                        case GreaterDepth:
                            glDepthFunc( GL_GREATER );
                            break;
                        case NotEqualDepth:
                            glDepthFunc( GL_NOTEQUAL );
                            break;
                        default:
                            glDepthFunc( GL_LEQUAL );
                    }
                } else {
                    glDepthFunc( GL_LEQUAL );
                }
                currentDepthFunc = depthFunc;
            }

        }

        DepthBuffer &setLocked( bool lock ) {
            locked = lock;
            return *this;
        }

        DepthBuffer &setClear( float depth ) {
            if ( currentDepthClear != depth ) {
                glClearDepthf( depth );
                currentDepthClear = depth;
            }
            return *this;
        }

        DepthBuffer &reset(){
            locked = false;

            currentDepthMask = false;
            currentDepthFunc = 0;
            currentDepthClear = 0.f;

            return *this;
        }

    };

    class StencilBuffer{
    public:
        bool locked = false;

        int currentStencilMask;
        int currentStencilFunc = 0;
        int currentStencilRef = 0;
        int currentStencilFuncMask = 0;
        GLenum currentStencilFail;
        GLenum currentStencilZFail;
        GLenum currentStencilZPass;
        int currentStencilClear = 0;
        StencilBuffer &setTest( bool stencilTest ) {
            if ( ! locked ) {
                if ( stencilTest ) {
                    glEnable( GL_STENCIL_TEST );
                } else {
                    glDisable( GL_STENCIL_TEST );
                }
            }
            return *this;
        }

        StencilBuffer &setMask( int stencilMask ) {
            if ( currentStencilMask != stencilMask && ! locked ) {
                glStencilMask( stencilMask );
                currentStencilMask = stencilMask;
            }
            return *this;
        }

        StencilBuffer &setFunc( int stencilFunc, int stencilRef, int stencilMask ) {
            if ( currentStencilFunc != stencilFunc ||
                currentStencilRef != stencilRef ||
                currentStencilFuncMask != stencilMask ) {

                glStencilFunc( stencilFunc, stencilRef, stencilMask );

                currentStencilFunc = stencilFunc;
                currentStencilRef = stencilRef;
                currentStencilFuncMask = stencilMask;
            }
            return *this;
        }

        StencilBuffer &setOp( GLenum stencilFail, GLenum stencilZFail, GLenum stencilZPass ) {
            if ( currentStencilFail != stencilFail ||
                currentStencilZFail != stencilZFail ||
                currentStencilZPass != stencilZPass ) {

                glStencilOp( stencilFail, stencilZFail, stencilZPass );

                currentStencilFail = stencilFail;
                currentStencilZFail = stencilZFail;
                currentStencilZPass = stencilZPass;
            }
            return *this;
        }

        StencilBuffer &setLocked( bool lock ) {
            locked = lock;
            return *this;
        }

        StencilBuffer &setClear( int stencil ) {
            if ( currentStencilClear != stencil ) {
                glClearStencil( stencil );
                currentStencilClear = stencil;
            }
            return *this;
        }

        StencilBuffer &reset(){
            locked = false;

            currentStencilMask = 0;
            currentStencilFunc = 0;
            currentStencilRef = 0;
            currentStencilFuncMask = 0;
            currentStencilFail = GL_NONE;
            currentStencilZFail = GL_NONE;
            currentStencilZPass = GL_NONE;
            currentStencilClear = 0;

        }

    };


    ColorBuffer colorBuffer{};
    DepthBuffer depthBuffer{};
    StencilBuffer stencilBuffer{};

    std::map<GLuint,bool> enabledCapabilities = {};

    std::map<GLenum ,GLuint> currentBoundFramebuffers = {};
//    let currentDrawbuffers = new WeakMap();
//    let defaultDrawbuffers = [];

    bool currentProgram = 0;

    bool currentBlendingEnabled = false;
    bool currentBlending = false;
    bool currentBlendEquation = -1;
    bool currentBlendSrc = -1;
    bool currentBlendDst = -1;
    int currentBlendEquationAlpha = -1;
    int currentBlendSrcAlpha = -1;
    int currentBlendDstAlpha = -1;
    bool currentPremultipledAlpha = false;

    bool currentFlipSided = false;
    int currentCullFace;

    double currentLineWidth;

    double currentPolygonOffsetFactor;
    double currentPolygonOffsetUnits;

    //const int maxTextures = glGetParameteri( GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS );

    bool lineWidthAvailable = false;
    int version = 0;
    const GLubyte * glVersion = glGetString( GL_VERSION );

//    if ( glVersion.indexOf( 'WebGL' ) !== - 1 ) {
//        version = parseFloat( /^WebGL (\d)/.exec( glVersion )[ 1 ] );
//        lineWidthAvailable = ( version >= 1.0 );
//    } else if ( glVersion.indexOf( 'OpenGL ES' ) !== - 1 ) {
//        version = parseFloat( /^OpenGL ES (\d)/.exec( glVersion )[ 1 ] );
//        lineWidthAvailable = ( version >= 2.0 );
//    }
//
    int currentTextureSlot = -1;
    std::vector<BoundTexture> currentBoundTextures = {};
//
//    const scissorParam = gl.getParameter( GL_SCISSOR_BOX );
//    const viewportParam = gl.getParameter( GL_VIEWPORT );
//
    Vector4d currentScissor; /*new Vector4().fromArray( scissorParam );*/
    Vector4d currentViewport; /*new Vector4().fromArray( viewportParam );*/
//
//    function createTexture( type, target, count ) {
//
//        const data = new Uint8Array( 4 ); // 4 is required to match default unpack alignment of 4.
//        const texture = gl.createTexture();
//
//        gl.bindTexture( type, texture );
//        gl.texParameteri( type, gl.TEXTURE_MIN_FILTER, gl.NEAREST );
//        gl.texParameteri( type, gl.TEXTURE_MAG_FILTER, gl.NEAREST );
//
//        for ( let i = 0; i < count; i ++ ) {
//
//            gl.texImage2D( target + i, 0, gl.RGBA, 1, 1, 0, gl.RGBA, gl.UNSIGNED_BYTE, data );
//
//        }
//
//        return texture;
//
//    }
//
//    const emptyTextures = {};
//    emptyTextures[ gl.TEXTURE_2D ] = createTexture( gl.TEXTURE_2D, gl.TEXTURE_2D, 1 );
//    emptyTextures[ gl.TEXTURE_CUBE_MAP ] = createTexture( gl.TEXTURE_CUBE_MAP, gl.TEXTURE_CUBE_MAP_POSITIVE_X, 6 );
//
//    // init
//
//    colorBuffer.setClear( 0, 0, 0, 1 );
//    depthBuffer.setClear( 1 );
//    stencilBuffer.setClear( 0 );
//
//    enable( glDEPTH_TEST );
//    depthBuffer.setFunc( LessEqualDepth );
//
//    setFlipSided( false );
//    setCullFace( CullFaceBack );
//    enable( gl.CULL_FACE );
//
//    setBlending( NoBlending );
//
//    //

    GLState & enable( GLuint id ) {
        if ( enabledCapabilities[ id ] != true ) {
            glEnable( id );
            enabledCapabilities[ id ] = true;
        }
        return *this;
    }

    GLState &disable( GLuint id ) {
        if ( enabledCapabilities[ id ] != false ) {

            glDisable( id );
            enabledCapabilities[ id ] = false;

        }
        return *this;
    }

    GLState &bindFramebuffer( GLenum target, GLuint framebuffer ) {
        if ( currentBoundFramebuffers[ target ] != framebuffer ) {
            glBindFramebuffer( target, framebuffer );

            currentBoundFramebuffers[ target ] = framebuffer;

            if ( true/**isWebGL2*/ ) {

                // glDRAW_FRAMEBUFFER is equivalent to gl.FRAMEBUFFER
                if ( target == GL_DRAW_FRAMEBUFFER ) {
                    currentBoundFramebuffers[ GL_FRAMEBUFFER ] = framebuffer;
                }

                if ( target == GL_FRAMEBUFFER ) {
                    currentBoundFramebuffers[ GL_DRAW_FRAMEBUFFER ] = framebuffer;
                }

            }

        }

        return *this;
    }

//    function drawBuffers( renderTarget, framebuffer ) {
//
//        let drawBuffers = defaultDrawbuffers;
//
//        let needsUpdate = false;
//
//        if ( renderTarget ) {
//
//            drawBuffers = currentDrawbuffers.get( framebuffer );
//
//            if ( drawBuffers === undefined ) {
//
//                drawBuffers = [];
//                currentDrawbuffers.set( framebuffer, drawBuffers );
//
//            }
//
//            if ( renderTarget.isWebGLMultipleRenderTargets ) {
//
//                const textures = renderTarget.texture;
//
//                if ( drawBuffers.length !== textures.length || drawBuffers[ 0 ] !== gl.COLOR_ATTACHMENT0 ) {
//
//                    for ( let i = 0, il = textures.length; i < il; i ++ ) {
//
//                        drawBuffers[ i ] = gl.COLOR_ATTACHMENT0 + i;
//
//                    }
//
//                    drawBuffers.length = textures.length;
//
//                    needsUpdate = true;
//
//                }
//
//            } else {
//
//                if ( drawBuffers[ 0 ] !== gl.COLOR_ATTACHMENT0 ) {
//
//                    drawBuffers[ 0 ] = gl.COLOR_ATTACHMENT0;
//
//                    needsUpdate = true;
//
//                }
//
//            }
//
//        } else {
//
//            if ( drawBuffers[ 0 ] !== gl.BACK ) {
//
//                drawBuffers[ 0 ] = gl.BACK;
//
//                needsUpdate = true;
//
//            }
//
//        }
//
//        if ( needsUpdate ) {
//
//            if ( capabilities.isWebGL2 ) {
//
//                glDrawBuffers( drawBuffers );
//
//            } else {
//
//                extensions.get( 'WEBGL_draw_buffers' ).drawBuffersWEBGL( drawBuffers );
//
//            }
//
//        }
//
//
//    }

    GLState &useProgram( GLuint program ) {
        if ( currentProgram != program ) {
            glUseProgram( program );
            currentProgram = program;
            //return true;
        }
        //return false;
        return *this;
    }

    std::map<int,GLenum>  equationToGL = {
        { AddEquation , GL_FUNC_ADD},
        { SubtractEquation , GL_FUNC_SUBTRACT},
        { ReverseSubtractEquation , GL_FUNC_REVERSE_SUBTRACT},
        { MinEquation , GL_MIN},
        { MaxEquation , GL_MAX}
    };

    std::map<int,GLenum> factorToGL = {
        { ZeroFactor, GL_ZERO},
        { OneFactor , GL_ONE},
        { SrcColorFactor , GL_SRC_COLOR},
        { SrcAlphaFactor , GL_SRC_ALPHA},
        { SrcAlphaSaturateFactor , GL_SRC_ALPHA_SATURATE},
        { DstColorFactor , GL_DST_COLOR},
        { DstAlphaFactor , GL_DST_ALPHA},
        { OneMinusSrcColorFactor , GL_ONE_MINUS_SRC_COLOR},
        { OneMinusSrcAlphaFactor , GL_ONE_MINUS_SRC_ALPHA},
        { OneMinusDstColorFactor , GL_ONE_MINUS_DST_COLOR},
        { OneMinusDstAlphaFactor , GL_ONE_MINUS_DST_ALPHA}
    };

    GLState &setBlending( int blending, int blendEquation, int blendSrc, int blendDst, int blendEquationAlpha, int blendSrcAlpha, int blendDstAlpha, bool premultipliedAlpha ) {
        if ( blending == NoBlending ) {
            if ( currentBlendingEnabled == true ) {
                disable( GL_BLEND );
                currentBlendingEnabled = false;
            }
            return *this;
        }

        if ( currentBlendingEnabled == false ) {
            enable( GL_BLEND );
            currentBlendingEnabled = true;
        }

        if ( blending != CustomBlending ) {
            if ( blending != currentBlending || premultipliedAlpha != currentPremultipledAlpha ) {

                if ( currentBlendEquation != AddEquation || currentBlendEquationAlpha != AddEquation ) {

                    glBlendEquation( GL_FUNC_ADD );

                    currentBlendEquation = AddEquation;
                    currentBlendEquationAlpha = AddEquation;
                }

                if ( premultipliedAlpha ) {
                    switch ( blending ) {
                        case NormalBlending:
                            glBlendFuncSeparate( GL_ONE, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA );
                            break;

                        case AdditiveBlending:
                            glBlendFunc( GL_ONE, GL_ONE );
                            break;

                        case SubtractiveBlending:
                            glBlendFuncSeparate( GL_ZERO, GL_ONE_MINUS_SRC_COLOR, GL_ZERO, GL_ONE );
                            break;

                        case MultiplyBlending:
                            glBlendFuncSeparate( GL_ZERO, GL_SRC_COLOR, GL_ZERO, GL_SRC_ALPHA );
                            break;

                        default:
                            //console.error( 'THREE.WebGLState: Invalid blending: ', blending );
                            break;

                    }

                } else {
                    switch ( blending ) {
                        case NormalBlending:
                            glBlendFuncSeparate( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA );
                            break;

                        case AdditiveBlending:
                            glBlendFunc( GL_SRC_ALPHA, GL_ONE );
                            break;

                        case SubtractiveBlending:
                            glBlendFuncSeparate( GL_ZERO, GL_ONE_MINUS_SRC_COLOR, GL_ZERO, GL_ONE );
                            break;

                        case MultiplyBlending:
                            glBlendFunc( GL_ZERO, GL_SRC_COLOR );
                            break;

                        default:
                            //console.error( 'THREE.WebGLState: Invalid blending: ', blending );
                            break;

                    }

                }

                currentBlendSrc = -1;
                currentBlendDst = -1;
                currentBlendSrcAlpha = -1;
                currentBlendDstAlpha = -1;

                currentBlending = blending;
                currentPremultipledAlpha = premultipliedAlpha;

            }

            return *this;
        }

        // custom blending

        blendEquationAlpha = blendEquationAlpha || blendEquation;
        blendSrcAlpha = blendSrcAlpha || blendSrc;
        blendDstAlpha = blendDstAlpha || blendDst;

        if ( blendEquation != currentBlendEquation || blendEquationAlpha != currentBlendEquationAlpha ) {

            glBlendEquationSeparate( equationToGL[blendEquation], equationToGL[blendEquationAlpha] );

            currentBlendEquation = blendEquation;
            currentBlendEquationAlpha = blendEquationAlpha;
        }

        if ( blendSrc != currentBlendSrc || blendDst != currentBlendDst || blendSrcAlpha != currentBlendSrcAlpha || blendDstAlpha != currentBlendDstAlpha ) {

            glBlendFuncSeparate( factorToGL[ blendSrc ], factorToGL[ blendDst ], factorToGL[ blendSrcAlpha ], factorToGL[ blendDstAlpha ] );

            currentBlendSrc = blendSrc;
            currentBlendDst = blendDst;
            currentBlendSrcAlpha = blendSrcAlpha;
            currentBlendDstAlpha = blendDstAlpha;

        }

        currentBlending = blending;
        currentPremultipledAlpha = false;

        return *this;
    }

//    function setMaterial( material, frontFaceCW ) {
//
//        material.side === DoubleSide
//                          ? disable( gl.CULL_FACE )
//                          : enable( gl.CULL_FACE );
//
//        let flipSided = ( material.side === BackSide );
//        if ( frontFaceCW ) flipSided = ! flipSided;
//
//        setFlipSided( flipSided );
//
//        ( material.blending === NormalBlending && material.transparent === false )
//        ? setBlending( NoBlending )
//        : setBlending( material.blending, material.blendEquation, material.blendSrc, material.blendDst, material.blendEquationAlpha, material.blendSrcAlpha, material.blendDstAlpha, material.premultipliedAlpha );
//
//        depthBuffer.setFunc( material.depthFunc );
//        depthBuffer.setTest( material.depthTest );
//        depthBuffer.setMask( material.depthWrite );
//        colorBuffer.setMask( material.colorWrite );
//
//        const stencilWrite = material.stencilWrite;
//        stencilBuffer.setTest( stencilWrite );
//        if ( stencilWrite ) {
//
//            stencilBuffer.setMask( material.stencilWriteMask );
//            stencilBuffer.setFunc( material.stencilFunc, material.stencilRef, material.stencilFuncMask );
//            stencilBuffer.setOp( material.stencilFail, material.stencilZFail, material.stencilZPass );
//
//        }
//
//        setPolygonOffset( material.polygonOffset, material.polygonOffsetFactor, material.polygonOffsetUnits );
//
//        material.alphaToCoverage === true
//                                     ? enable( gl.SAMPLE_ALPHA_TO_COVERAGE )
//                                     : disable( gl.SAMPLE_ALPHA_TO_COVERAGE );
//
//    }
//
//    //

    GLState &setFlipSided( bool flipSided ) {
        if ( currentFlipSided != flipSided ) {

            if ( flipSided ) {
                glFrontFace( GL_CW );
            } else {
                glFrontFace( GL_CCW );
            }
            currentFlipSided = flipSided;
        }
        return *this;
    }

    GLState &setCullFace( int cullFace ) {
        if ( cullFace != CullFaceNone ) {
            enable( GL_CULL_FACE );

            if ( cullFace != currentCullFace ) {
                if ( cullFace == CullFaceBack ) {
                    glCullFace( GL_BACK );
                } else if ( cullFace == CullFaceFront ) {
                    glCullFace( GL_FRONT );
                } else {
                    glCullFace( GL_FRONT_AND_BACK );
                }
            }

        } else {
            disable( GL_CULL_FACE );
        }

        currentCullFace = cullFace;

        return *this;
    }

    GLState &setLineWidth( float width ) {
        if ( width != currentLineWidth ) {
            if ( lineWidthAvailable ) glLineWidth( width );
            currentLineWidth = width;
        }
        return *this;
    }

    GLState &setPolygonOffset( bool polygonOffset, double factor, double units ) {
        if ( polygonOffset ) {
            enable( GL_POLYGON_OFFSET_FILL );
            if ( currentPolygonOffsetFactor != factor || currentPolygonOffsetUnits != units ) {

                glPolygonOffset( factor, units );

                currentPolygonOffsetFactor = factor;
                currentPolygonOffsetUnits = units;
            }
        } else {
            disable( GL_POLYGON_OFFSET_FILL );
        }
        return *this;
    }

    GLState &setScissorTest( bool scissorTest ) {
        if ( scissorTest ) {
            enable( GL_SCISSOR_TEST );
        } else {
            disable( GL_SCISSOR_TEST );
        }
        return *this;
    }

    // texture
    GLState &activeTexture( int glSlot = -1 ) {
        //if ( webglSlot === undefined ) webglSlot = gl.TEXTURE0 + maxTextures - 1;
        if ( currentTextureSlot != glSlot ) {
            glActiveTexture( glSlot );
            currentTextureSlot = glSlot;
        }
        return *this;
    }

    GLState &bindTexture( GLenum glType, GLuint glTexture ) {
        if ( currentTextureSlot == -1 ) {
            activeTexture();
        }

        BoundTexture boundTexture{GL_NONE,0};
        if(currentBoundTextures.size() >= currentTextureSlot)
            boundTexture = currentBoundTextures[ currentTextureSlot ];
//        if ( boundTexture == undefined ) {
//            boundTexture = { type: undefined, texture: undefined };
//            currentBoundTextures[ currentTextureSlot ] = boundTexture;
//        }

        if ( boundTexture.type != glType || boundTexture.texture != glTexture ) {

            glBindTexture( glType, glTexture /**|| emptyTextures[ glType ]*/ );

            boundTexture.type = glType;
            boundTexture.texture = glTexture;
        }
        return *this;
    }

    GLState &unbindTexture() {
        if(currentBoundTextures.size()>=currentTextureSlot) {
            BoundTexture boundTexture = currentBoundTextures[currentTextureSlot];

            glBindTexture(boundTexture.type, 0);

            boundTexture.type = GL_NONE;
            boundTexture.texture = 0;
        }
        return *this;
    }

//    function compressedTexImage2D() {
//        glCompressedTexImage2D()
//        try {
//
//            gl.compressedTexImage2D.apply( gl, arguments );
//
//        } catch ( error ) {
//
//            console.error( 'THREE.WebGLState:', error );
//
//        }
//
//    }

//    function texSubImage2D() {
//
//        try {
//
//            gl.texSubImage2D.apply( gl, arguments );
//
//        } catch ( error ) {
//
//            console.error( 'THREE.WebGLState:', error );
//
//        }
//
//    }
//
//    function texSubImage3D() {
//
//        try {
//
//            gl.texSubImage3D.apply( gl, arguments );
//
//        } catch ( error ) {
//
//            console.error( 'THREE.WebGLState:', error );
//
//        }
//
//    }
//
//    function compressedTexSubImage2D() {
//
//        try {
//
//            gl.compressedTexSubImage2D.apply( gl, arguments );
//
//        } catch ( error ) {
//
//            console.error( 'THREE.WebGLState:', error );
//
//        }
//
//    }
//
//    function texStorage2D() {
//
//        try {
//
//            gl.texStorage2D.apply( gl, arguments );
//
//        } catch ( error ) {
//
//            console.error( 'THREE.WebGLState:', error );
//
//        }
//
//    }
//
//    function texStorage3D() {
//
//        try {
//
//            gl.texStorage3D.apply( gl, arguments );
//
//        } catch ( error ) {
//
//            console.error( 'THREE.WebGLState:', error );
//
//        }
//
//    }
//
//    function texImage2D() {
//
//        try {
//
//            gl.texImage2D.apply( gl, arguments );
//
//        } catch ( error ) {
//
//            console.error( 'THREE.WebGLState:', error );
//
//        }
//
//    }
//
//    function texImage3D() {
//        try {
//            gl.texImage3D.apply( gl, arguments );
//        } catch ( error ) {
//            console.error( 'THREE.WebGLState:', error );
//        }
//    }
    //

    GLState &scissor( Vector4d scissor ) {
        if ( currentScissor.equals( scissor ) == false ) {
            glScissor( scissor.x, scissor.y, scissor.z, scissor.w );
            currentScissor.copy( scissor );
        }
        return *this;
    }

    GLState &viewport( Vector4d viewport ) {
        if ( currentViewport.equals( viewport ) == false ) {
            glViewport( viewport.x, viewport.y, viewport.z, viewport.w );
            currentViewport.copy( viewport );
        }
        return *this;
    }

    GLState &reset() {

        // reset state
        glDisable( GL_BLEND );
        glDisable( GL_CULL_FACE );
        glDisable( GL_DEPTH_TEST );
        glDisable( GL_POLYGON_OFFSET_FILL );
        glDisable( GL_SCISSOR_TEST );
        glDisable( GL_STENCIL_TEST );
        glDisable( GL_SAMPLE_ALPHA_TO_COVERAGE );

        glBlendEquation( GL_FUNC_ADD );
        glBlendFunc( GL_ONE, GL_ZERO );
        glBlendFuncSeparate( GL_ONE, GL_ZERO, GL_ONE, GL_ZERO );

        glColorMask( true, true, true, true );
        glClearColor( 0, 0, 0, 0 );

        glDepthMask( true );
        glDepthFunc( GL_LESS );
        glClearDepthf( 1 );

        glStencilMask( 0xffffffff );
        glStencilFunc( GL_ALWAYS, 0, 0xffffffff );
        glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
        glClearStencil( 0 );

        glCullFace( GL_BACK );
        glFrontFace( GL_CCW );

        glPolygonOffset( 0, 0 );

        glActiveTexture( GL_TEXTURE0 );

        glBindFramebuffer( GL_FRAMEBUFFER, 0 );

        if ( isWebGL2 == true ) {

            glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
            glBindFramebuffer( GL_READ_FRAMEBUFFER, 0 );

        }

        glUseProgram( 0 );

        glLineWidth( 1 );

        glScissor( 0, 0, defaultViewPort.width, defaultViewPort.height );
        glViewport( 0, 0, defaultViewPort.width, defaultViewPort.height );

        // reset internals

        enabledCapabilities = {};

        currentTextureSlot = -1;
        currentBoundTextures = {};

        currentBoundFramebuffers = {};
//        currentDrawbuffers = new WeakMap();
//        defaultDrawbuffers = [];

        GLuint currentProgram = 0;

        currentBlendingEnabled = false;
        currentBlending = -1;
        currentBlendEquation = -1;
        currentBlendSrc = -1;
        currentBlendDst = -1;
        currentBlendEquationAlpha = -1;
        currentBlendSrcAlpha = -1;
        currentBlendDstAlpha = -1;
        currentPremultipledAlpha = false;

        currentFlipSided = -1;
        currentCullFace = -1;

        currentLineWidth = -1;

        currentPolygonOffsetFactor = -1;
        currentPolygonOffsetUnits = -1;

        currentScissor.set( 0, 0, defaultViewPort.width, defaultViewPort.height );
        currentViewport.set( 0, 0, defaultViewPort.width, defaultViewPort.height );

        colorBuffer.reset();
        depthBuffer.reset();
        stencilBuffer.reset();

    }

//    return {
//
//        buffers: {
//            color: colorBuffer,
//            depth: depthBuffer,
//            stencil: stencilBuffer
//        },
//
//        enable: enable,
//        disable: disable,
//
//        bindFramebuffer: bindFramebuffer,
//        drawBuffers: drawBuffers,
//
//        useProgram: useProgram,
//
//        setBlending: setBlending,
//        setMaterial: setMaterial,
//
//        setFlipSided: setFlipSided,
//        setCullFace: setCullFace,
//
//        setLineWidth: setLineWidth,
//        setPolygonOffset: setPolygonOffset,
//
//        setScissorTest: setScissorTest,
//
//        activeTexture: activeTexture,
//        bindTexture: bindTexture,
//        unbindTexture: unbindTexture,
//        compressedTexImage2D: compressedTexImage2D,
//        texImage2D: texImage2D,
//        texImage3D: texImage3D,
//
//        texStorage2D: texStorage2D,
//        texStorage3D: texStorage3D,
//        texSubImage2D: texSubImage2D,
//        texSubImage3D: texSubImage3D,
//        compressedTexSubImage2D: compressedTexSubImage2D,
//
//        scissor: scissor,
//        viewport: viewport,
//
//        reset: reset
//
//    };

};

//import { NotEqualDepth, GreaterDepth, GreaterEqualDepth, EqualDepth, LessEqualDepth, LessDepth, AlwaysDepth, NeverDepth, CullFaceFront, CullFaceBack, CullFaceNone, DoubleSide, BackSide, CustomBlending, MultiplyBlending, SubtractiveBlending, AdditiveBlending, NoBlending, NormalBlending, AddEquation, SubtractEquation, ReverseSubtractEquation, MinEquation, MaxEquation, ZeroFactor, OneFactor, SrcColorFactor, SrcAlphaFactor, SrcAlphaSaturateFactor, DstColorFactor, DstAlphaFactor, OneMinusSrcColorFactor, OneMinusSrcAlphaFactor, OneMinusDstColorFactor, OneMinusDstAlphaFactor } from '../../constants.js';
//import { Vector4 } from '../../math/Vector4.js';
//
//function WebGLState( gl, extensions, capabilities ) {
//
//    const isWebGL2 = capabilities.isWebGL2;

//    function StencilBuffer() {
//

//
//    }
//
//    //

//
//}
//
//export { WebGLState };


#endif //THREE_CPP_SRC_RENDERS_GL_STATE_H
