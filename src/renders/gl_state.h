//
// Created by SeanR on 2022/11/24.
//

#ifndef THREE_CPP_SRC_RENDERS_GL_STATE_H
#define THREE_CPP_SRC_RENDERS_GL_STATE_H

#include "vector4.h"
#include "constants.h"
#include "common_types.h"
#include "gl_headers.h"

#include <unordered_map>
#include <map>


//struct BoundTexture{
//    GLenum type;
//    GLuint texture;
//};

const threecpp::GLViewPort defaultViewPort{0,0,640,480};

class GLState {
public:
    const bool isGLES3 = true;

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

        ColorBuffer &setClear(double r, double g, double b, double a, bool premultipliedAlpha = true) {
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
        GLState& state;
        bool locked = false;

        bool currentDepthMask = false;
        DepthModes currentDepthFunc = DepthModes::NeverDepth;
        float currentDepthClear = 0.f;

        DepthBuffer(GLState& state) : state(state) {}

        DepthBuffer &setTest(bool depthTest){
            if (depthTest) {
                //glEnable(GL_DEPTH_TEST);
                state.enable(GL_DEPTH_TEST);
            }else {
                //glDisable(GL_DEPTH_TEST);
                state.disable(GL_DEPTH_TEST);
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

        DepthBuffer &setFunc( DepthModes depthFunc ) {
            if ( currentDepthFunc == depthFunc ) {
//                if ( depthFunc ) {
                    switch ( depthFunc ) {
                        case DepthModes::NeverDepth:
                            glDepthFunc( GL_NEVER );
                            break;
                        case DepthModes::AlwaysDepth:
                            glDepthFunc( GL_ALWAYS );
                            break;
                        case DepthModes::LessDepth:
                            glDepthFunc( GL_LESS );
                            break;
                        case DepthModes::LessEqualDepth:
                            glDepthFunc( GL_LEQUAL );
                            break;
                        case DepthModes::EqualDepth:
                            glDepthFunc( GL_EQUAL );
                            break;
                        case DepthModes::GreaterEqualDepth:
                            glDepthFunc( GL_GEQUAL );
                            break;
                        case DepthModes::GreaterDepth:
                            glDepthFunc( GL_GREATER );
                            break;
                        case DepthModes::NotEqualDepth:
                            glDepthFunc( GL_NOTEQUAL );
                            break;
                        default:
                            glDepthFunc( GL_LEQUAL );
                    }
//                } else {
//                    glDepthFunc( GL_LEQUAL );
//                }
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
        GLState& state;
        bool locked = false;

        int currentStencilMask;
        int currentStencilFunc = 0;
        int currentStencilRef = 0;
        int currentStencilFuncMask = 0;
        GLenum currentStencilFail;
        GLenum currentStencilZFail;
        GLenum currentStencilZPass;
        int currentStencilClear = 0;

        StencilBuffer(GLState& state) : state(state) {}

        StencilBuffer &setTest( bool stencilTest ) {
            if ( ! locked ) {
                if ( stencilTest ) {
                    //glEnable( GL_STENCIL_TEST );
                    state.enable(GL_STENCIL_TEST);
                } else {
                    //glDisable( GL_STENCIL_TEST );
                    state.disable(GL_STENCIL_TEST);
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

//        void reset() {
//            locked = false;
//
//            currentStencilFuncMask = 0;
//            //currentStencilFunc = StencilFunc::NeverStencilFunc;
//            currentStencilRef = 0;
//            currentStencilFuncMask = 0;
//            //currentStencilFail = StencilOp::ZeroStencilOp;
//            //currentStencilZFail = StencilOp::ZeroStencilOp;
//            //currentStencilZPass = StencilOp::ZeroStencilOp;
//            currentStencilClear =0;
//
//        }

    };

    struct BoundTexture {
        TextureTarget target;
        GLint texture;

        BoundTexture(TextureTarget target, GLint texture) : target(target), texture(texture) {}
        BoundTexture() :target(TextureTarget::Texture2D), texture(-1) {}
    };


    ColorBuffer colorBuffer{};
    DepthBuffer depthBuffer;
    StencilBuffer stencilBuffer;

    GLint maxVertexAttributes;

    std::vector<GLuint> newAttributes;
    std::vector<GLuint> enabledAttributes;
    std::vector<GLuint> attributeDivisors;


    std::unordered_map<GLenum, bool> enabledCapabilities;
    //std::map<GLuint,bool> enabledCapabilities = {};

    std::map<GLenum ,GLuint> currentBoundFramebuffers = {};
//    let currentDrawbuffers = new WeakMap();
//    let defaultDrawbuffers = [];

    GLuint currentProgram = 0;

    bool currentBlendingEnabled = false;
    Blending currentBlending = Blending::NoBlending;
    BlendingEquation currentBlendEquation = BlendingEquation::None;
    BlendingDstFactor currentBlendSrc = BlendingDstFactor::None;
    BlendingDstFactor currentBlendDst = BlendingDstFactor::None;
    BlendingEquation currentBlendEquationAlpha = BlendingEquation::None;
    BlendingDstFactor currentBlendSrcAlpha = BlendingDstFactor::None;
    BlendingDstFactor currentBlendDstAlpha = BlendingDstFactor::None;
    bool currentPremultipliedAlpha = false;

    bool currentFlipSided = false;
    CullFace currentCullFace = CullFace::CullFaceNone;
    float currentLineWidth;
    float currentPolygonOffsetFactor;
    float currentPolygonOffsetUnits;
    GLint maxTextures;

    bool lineWidthAvailable = false;
    GLuint version = 0;
    std::string glVersion;

    int currentTextureSlot;

    //const GLubyte * glVersion = glGetString( GL_VERSION );

//    if ( glVersion.indexOf( 'WebGL' ) !== - 1 ) {
//        version = parseFloat( /^WebGL (\d)/.exec( glVersion )[ 1 ] );
//        lineWidthAvailable = ( version >= 1.0 );
//    } else if ( glVersion.indexOf( 'OpenGL ES' ) !== - 1 ) {
//        version = parseFloat( /^OpenGL ES (\d)/.exec( glVersion )[ 1 ] );
//        lineWidthAvailable = ( version >= 2.0 );
//    }


//    const scissorParam = gl.getParameter( GL_SCISSOR_BOX );
//    const viewportParam = gl.getParameter( GL_VIEWPORT );
    Vector4d currentScissor; /*new Vector4().fromArray( scissorParam );*/
    Vector4d currentViewport; /*new Vector4().fromArray( viewportParam );*/

//    std::vector<BoundTexture> currentBoundTextures = {};
    std::unordered_map<int, BoundTexture> currentBoundTextures;
    std::unordered_map<GLuint, GLuint> emptyTextures;
    std::vector<int> compressedTextureFormats;

    GLState() : depthBuffer(*this),stencilBuffer(*this),currentTextureSlot(-1){
        //using TextureTarget = threecpp::TextureTarget;
        glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTextures);
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttributes);

        lineWidthAvailable = true;

        newAttributes.resize(maxVertexAttributes);
        enabledAttributes.resize(maxVertexAttributes);
        attributeDivisors.resize(maxVertexAttributes);

        emptyTextures[(GLuint)TextureTarget::Texture2D] = createTexture(TextureTarget::Texture2D, TextureTarget::Texture2D, 1);
        emptyTextures[(GLuint)TextureTarget::cubeMap] = createTexture(TextureTarget::cubeMap, TextureTarget::cubeMapNegativeX, 6);

        colorBuffer.setClear(0, 0, 0, 1);
        depthBuffer.setClear(1);
        stencilBuffer.setClear(0);


        enable(GL_DEPTH_TEST);
        depthBuffer.setFunc(DepthModes::LessEqualDepth);

        setFlipSided(false);
        setCullFace(CullFace::CullFaceBack);
        enable(GL_CULL_FACE);

        setBlending(Blending::NoBlending);
    }
    virtual ~GLState() = default;

    GLState& copy(const GLState& source) {
        colorBuffer = source.colorBuffer;
        depthBuffer = source.depthBuffer;
        stencilBuffer = source.stencilBuffer;

        maxVertexAttributes = source.maxVertexAttributes;

        if (source.newAttributes.size() > 0) {
            newAttributes.resize(source.newAttributes.size());
            std::copy(source.newAttributes.begin(), source.newAttributes.end(), newAttributes.begin());
        }
        if (source.enabledAttributes.size() > 0) {
            enabledAttributes.resize(source.enabledAttributes.size());
            std::copy(source.enabledAttributes.begin(), source.enabledAttributes.end(), enabledAttributes.begin());
        }

        if (source.attributeDivisors.size() > 0) {
            attributeDivisors.resize(source.attributeDivisors.size());
            std::copy(source.attributeDivisors.begin(), source.attributeDivisors.end(), attributeDivisors.begin());
        }


        enabledCapabilities = source.enabledCapabilities;


        currentProgram = source.currentProgram;

        currentBlendingEnabled = source.currentBlendingEnabled;
        currentBlending = source.currentBlending;
        currentBlendEquation = source.currentBlendEquation;
        currentBlendSrc = source.currentBlendSrc;
        currentBlendDst = source.currentBlendDst;
        currentBlendEquationAlpha = source.currentBlendEquationAlpha;
        currentBlendSrcAlpha = source.currentBlendSrcAlpha;
        currentBlendDstAlpha = source.currentBlendDstAlpha;
        currentPremultipliedAlpha = source.currentPremultipliedAlpha;

        currentFlipSided = source.currentFlipSided;
        currentCullFace = source.currentCullFace;
        currentLineWidth = source.currentLineWidth;
        currentPolygonOffsetFactor = source.currentPolygonOffsetFactor;
        currentPolygonOffsetUnits = source.currentPolygonOffsetUnits;
        maxTextures = source.maxTextures;

        lineWidthAvailable = source.lineWidthAvailable;
        version = source.version;
        glVersion = source.glVersion;
        currentTextureSlot = source.currentTextureSlot;

        currentScissor = source.currentScissor;
        currentViewport = source.currentViewport;

        currentBoundTextures = source.currentBoundTextures;
        emptyTextures = source.emptyTextures;

        if (source.compressedTextureFormats.size() > 0)
            compressedTextureFormats.resize(source.compressedTextureFormats.size());
        std::copy(source.compressedTextureFormats.begin(), source.compressedTextureFormats.end(),compressedTextureFormats.begin());

        return *this;
    }

    GLState& operator = (const GLState& source) {
        return copy(source);
    }

    GLuint createTexture(TextureTarget type, TextureTarget target, unsigned count) {
        threecpp::byte data[4] = { 0,0,0,0 };
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture((GLenum)type, texture);
        glTexParameteri((GLenum)type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri((GLenum)type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        for (unsigned i = 0;i < count;i++) {
            glTexImage2D((GLenum)target + i, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }

        return texture;
    }

    void initAttributes() {
        for (unsigned i = 0;i< newAttributes.size(); i++) {
            newAttributes[i] = 0;
        }
    }

    void enableAttribute(GLuint attribute) {
        enableAttributeAndDivisor(attribute, 0);
    }
    void enableAttributeAndDivisor(GLuint attribute, GLuint meshPerAttribute) {
        newAttributes[attribute] = 1;

        if (enabledAttributes[attribute] == 0) {
            glEnableVertexAttribArray(attribute);
            enabledAttributes[attribute] = 1;
        }

        if (attributeDivisors[attribute] != meshPerAttribute) {
            //glVertexAttribDivisor(attribute, meshPerAttribute);
            attributeDivisors[attribute] = meshPerAttribute;
        }
    }
    void disableUnusedAttributes() {
        for (unsigned i = 0;i< enabledAttributes.size(); ++i) {

            if (enabledAttributes[i] !=  newAttributes[i]) {

                glDisableVertexAttribArray(i);
                enabledAttributes[i] = 0;
            }
        }
    }
    void vertexAttribPointer(GLuint index,GLint size,GLenum type,bool normalized,GLuint stride,GLuint offset) {
        //if(type==GL_INT || type==GL_UNSIGNED_INT)
        //	glVertexAttribIPointer(index, size, type, normalized,(const void*)offset);
        //else
        glVertexAttribPointer(index, size, type, normalized, stride, (void*)offset);
    }

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

    GLState &useProgram( GLuint program ) {
        if ( currentProgram != program ) {
            glUseProgram( program );
            currentProgram = program;
            //return true;
        }
        //return false;
        return *this;
    }

//    std::map<int,GLenum>  equationToGL = {
//        { AddEquation , GL_FUNC_ADD},
//        { SubtractEquation , GL_FUNC_SUBTRACT},
//        { ReverseSubtractEquation , GL_FUNC_REVERSE_SUBTRACT},
//        { MinEquation , GL_MIN},
//        { MaxEquation , GL_MAX}
//    };

//    std::map<int,GLenum> factorToGL = {
//        { ZeroFactor, GL_ZERO},
//        { OneFactor , GL_ONE},
//        { SrcColorFactor , GL_SRC_COLOR},
//        { SrcAlphaFactor , GL_SRC_ALPHA},
//        { SrcAlphaSaturateFactor , GL_SRC_ALPHA_SATURATE},
//        { DstColorFactor , GL_DST_COLOR},
//        { DstAlphaFactor , GL_DST_ALPHA},
//        { OneMinusSrcColorFactor , GL_ONE_MINUS_SRC_COLOR},
//        { OneMinusSrcAlphaFactor , GL_ONE_MINUS_SRC_ALPHA},
//        { OneMinusDstColorFactor , GL_ONE_MINUS_DST_COLOR},
//        { OneMinusDstAlphaFactor , GL_ONE_MINUS_DST_ALPHA}
//    };

//    GLState &setBlending( int blending, int blendEquation, int blendSrc, int blendDst, int blendEquationAlpha, int blendSrcAlpha, int blendDstAlpha, bool premultipliedAlpha ) {
//        if ( blending == NoBlending ) {
//            if ( currentBlendingEnabled == true ) {
//                disable( GL_BLEND );
//                currentBlendingEnabled = false;
//            }
//            return *this;
//        }
//
//        if ( currentBlendingEnabled == false ) {
//            enable( GL_BLEND );
//            currentBlendingEnabled = true;
//        }
//
//        if ( blending != CustomBlending ) {
//            if ( blending != currentBlending || premultipliedAlpha != currentPremultipledAlpha ) {
//
//                if ( currentBlendEquation != AddEquation || currentBlendEquationAlpha != AddEquation ) {
//
//                    glBlendEquation( GL_FUNC_ADD );
//
//                    currentBlendEquation = AddEquation;
//                    currentBlendEquationAlpha = AddEquation;
//                }
//
//                if ( premultipliedAlpha ) {
//                    switch ( blending ) {
//                        case NormalBlending:
//                            glBlendFuncSeparate( GL_ONE, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA );
//                            break;
//
//                        case AdditiveBlending:
//                            glBlendFunc( GL_ONE, GL_ONE );
//                            break;
//
//                        case SubtractiveBlending:
//                            glBlendFuncSeparate( GL_ZERO, GL_ONE_MINUS_SRC_COLOR, GL_ZERO, GL_ONE );
//                            break;
//
//                        case MultiplyBlending:
//                            glBlendFuncSeparate( GL_ZERO, GL_SRC_COLOR, GL_ZERO, GL_SRC_ALPHA );
//                            break;
//
//                        default:
//                            //console.error( 'THREE.WebGLState: Invalid blending: ', blending );
//                            break;
//
//                    }
//
//                } else {
//                    switch ( blending ) {
//                        case NormalBlending:
//                            glBlendFuncSeparate( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA );
//                            break;
//
//                        case AdditiveBlending:
//                            glBlendFunc( GL_SRC_ALPHA, GL_ONE );
//                            break;
//
//                        case SubtractiveBlending:
//                            glBlendFuncSeparate( GL_ZERO, GL_ONE_MINUS_SRC_COLOR, GL_ZERO, GL_ONE );
//                            break;
//
//                        case MultiplyBlending:
//                            glBlendFunc( GL_ZERO, GL_SRC_COLOR );
//                            break;
//
//                        default:
//                            //console.error( 'THREE.WebGLState: Invalid blending: ', blending );
//                            break;
//
//                    }
//
//                }
//
//                currentBlendSrc = -1;
//                currentBlendDst = -1;
//                currentBlendSrcAlpha = -1;
//                currentBlendDstAlpha = -1;
//
//                currentBlending = blending;
//                currentPremultipledAlpha = premultipliedAlpha;
//
//            }
//
//            return *this;
//        }
//
//        // custom blending
//
//        blendEquationAlpha = blendEquationAlpha || blendEquation;
//        blendSrcAlpha = blendSrcAlpha || blendSrc;
//        blendDstAlpha = blendDstAlpha || blendDst;
//
//        if ( blendEquation != currentBlendEquation || blendEquationAlpha != currentBlendEquationAlpha ) {
//
//            glBlendEquationSeparate( equationToGL[blendEquation], equationToGL[blendEquationAlpha] );
//
//            currentBlendEquation = blendEquation;
//            currentBlendEquationAlpha = blendEquationAlpha;
//        }
//
//        if ( blendSrc != currentBlendSrc || blendDst != currentBlendDst || blendSrcAlpha != currentBlendSrcAlpha || blendDstAlpha != currentBlendDstAlpha ) {
//
//            glBlendFuncSeparate( factorToGL[ blendSrc ], factorToGL[ blendDst ], factorToGL[ blendSrcAlpha ], factorToGL[ blendDstAlpha ] );
//
//            currentBlendSrc = blendSrc;
//            currentBlendDst = blendDst;
//            currentBlendSrcAlpha = blendSrcAlpha;
//            currentBlendDstAlpha = blendDstAlpha;
//
//        }
//
//        currentBlending = blending;
//        currentPremultipledAlpha = false;
//
//        return *this;
//    }
    GLState& setBlending(Blending blending, BlendingEquation blendEquation=BlendingEquation::None, BlendingDstFactor blendSrc=BlendingDstFactor::None, BlendingDstFactor blendDst=BlendingDstFactor::None, BlendingEquation blendEquationAlpha=BlendingEquation::None,BlendingDstFactor blendSrcAlpha=BlendingDstFactor::None,BlendingDstFactor blendDstAlpha=BlendingDstFactor::None, bool premultipliedAlpha = false) {
        if (blending == Blending::NoBlending) {
            if (currentBlendingEnabled) {
                disable(GL_BLEND);
                currentBlendingEnabled = false;
                return *this;
            }
            return *this;
        }


        if (!currentBlendingEnabled) {
            enable(GL_BLEND);
            currentBlendingEnabled = true;
        }
        if (blending != Blending::CustomBlending) {
            if (blending != currentBlending || premultipliedAlpha != currentPremultipliedAlpha) {
                if (currentBlendEquation != BlendingEquation::AddEquation || currentBlendEquationAlpha != BlendingEquation::AddEquation) {
                    glBlendEquation((GLenum)BlendingEquation::AddEquation);
                    currentBlendEquation = BlendingEquation::AddEquation;
                    currentBlendEquationAlpha = BlendingEquation::AddEquation;
                }

                if (premultipliedAlpha) {
                    switch (blending) {
                        case Blending::NormalBlending:
                            glBlendFuncSeparate(GL_ONE, GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
                            break;
                        case Blending::AdditiveBlending:
                            glBlendFunc(GL_ONE, GL_ONE);
                            break;

                        case Blending::SubtractiveBlending:
                            glBlendFuncSeparate(GL_ZERO, GL_ZERO, GL_ONE_MINUS_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA);
                            break;

                        case Blending::MultiplyBlending:
                            glBlendFuncSeparate(GL_ZERO, GL_SRC_COLOR, GL_ZERO, GL_SRC_ALPHA);
                            break;
                    }
                }
                else {
                    switch (blending) {

                        case Blending::NormalBlending:
                            glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
                            break;

                        case Blending::AdditiveBlending:
                            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
                            break;

                        case Blending::SubtractiveBlending:
                            glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_COLOR);
                            break;

                        case Blending::MultiplyBlending:
                            glBlendFunc(GL_ZERO, GL_SRC_COLOR);
                            break;
                    }
                }
                currentBlendSrc = BlendingDstFactor::None;
                currentBlendDst = BlendingDstFactor::None;
                currentBlendSrcAlpha = BlendingDstFactor::None;
                currentBlendDstAlpha = BlendingDstFactor::None;

                currentBlending = blending;
                currentPremultipliedAlpha = premultipliedAlpha;
            }
            return *this;
        }
        //custom blending
        blendEquationAlpha = blendEquationAlpha!=BlendingEquation::None ? blendEquationAlpha:blendEquation;
        blendSrcAlpha = blendSrcAlpha!=BlendingDstFactor::None? blendSrcAlpha : blendSrc;
        blendDstAlpha = blendDstAlpha!=BlendingDstFactor::None? blendDstAlpha : blendDst;

        if (blendEquation !=  currentBlendEquation || blendEquationAlpha !=  currentBlendEquationAlpha) {

            glBlendEquationSeparate((GLenum)blendEquation, (GLenum)blendEquationAlpha);

            currentBlendEquation = blendEquation;
            currentBlendEquationAlpha = blendEquationAlpha;

        }

        if (blendSrc != currentBlendSrc || blendDst !=  currentBlendDst || blendSrcAlpha !=  currentBlendSrcAlpha || blendDstAlpha !=  currentBlendDstAlpha) {

            glBlendFuncSeparate((GLenum)blendSrc, (GLenum)blendDst, (GLenum)blendSrcAlpha, (GLenum)blendDstAlpha);

            currentBlendSrc = blendSrc;
            currentBlendDst = blendDst;
            currentBlendSrcAlpha = blendSrcAlpha;
            currentBlendDstAlpha = blendDstAlpha;

        }

        currentBlending = blending;
        currentPremultipliedAlpha = false;
    }

    GLState& setMaterial(const Material& material, bool frontFaceCW=false) {
        //material.side == Side::DoubleSide ? disable(GL_CULL_FACE) : enable(GL_CULL_FACE);
        if (material.side == Side::DoubleSide) {
            glDisable(GL_CULL_FACE);
        }
        else {
            glEnable(GL_CULL_FACE);
        }

        bool flipSided = material.side == Side::BackSide;

        if (frontFaceCW) flipSided = !flipSided;

        setFlipSided(flipSided);

        if (material.blending == Blending::NormalBlending && material.transparent == false) {
            setBlending(Blending::NoBlending);
        }
        else {
            setBlending(material.blending, material.blendEquation, material.blendSrc, material.blendDst, material.blendEquationAlpha, material.blendSrcAlpha, material.blendDstAlpha, material.premultipliedAlpha);
        }

        depthBuffer.setFunc(material.depthFunc);
        depthBuffer.setTest(material.depthTest);
        depthBuffer.setMask(material.depthWrite);
        colorBuffer.setMask(material.colorWrite);

        if(material.depthTest)
            enable(GL_DEPTH_TEST);
        else
            disable(GL_DEPTH_TEST);

        auto stencilWrite = material.stencilWrite;

        stencilBuffer.setTest(stencilWrite);

        /*if (stencilWrite) {
            enable(GL_STENCIL_TEST);
        }
        else {
            disable(GL_STENCIL_TEST);
        }*/

        if (stencilWrite) {
            stencilBuffer.setMask(material.stencilWriteMask);
            stencilBuffer.setFunc(material.stencilFunc, material.stencilRef, material.stencilFuncMask);
            stencilBuffer.setOp(material.stencilFail, material.stencilZFail, material.stencilZPass);
        }

        setPolygonOffset(material.polygonOffset, material.polygonOffsetFactor, material.polygonOffsetUnits);
    }

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

    GLState &setCullFace( CullFace cullFace ) {
        if ( cullFace != CullFace::CullFaceNone ) {
            enable( GL_CULL_FACE );

            if ( cullFace != currentCullFace ) {
                if ( cullFace == CullFace::CullFaceBack ) {
                    glCullFace( GL_BACK );
                } else if ( cullFace == CullFace::CullFaceFront ) {
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

//    GLState &setPolygonOffset( bool polygonOffset, double factor, double units ) {
//        if ( polygonOffset ) {
//            enable( GL_POLYGON_OFFSET_FILL );
//            if ( currentPolygonOffsetFactor != factor || currentPolygonOffsetUnits != units ) {
//
//                glPolygonOffset( factor, units );
//
//                currentPolygonOffsetFactor = factor;
//                currentPolygonOffsetUnits = units;
//            }
//        } else {
//            disable( GL_POLYGON_OFFSET_FILL );
//        }
//        return *this;
//    }
    GLState &setPolygonOffset(bool polygonOffset, float factor=std::numeric_limits<float>::quiet_NaN(), float units=std::numeric_limits<float>::quiet_NaN()) {
        if (polygonOffset) {
            enable(GL_POLYGON_OFFSET_FILL);
            if ((!std::isnan(factor) && currentPolygonOffsetFactor != factor) || (!std::isnan(units) && currentPolygonOffsetUnits != units)) {
                glPolygonOffset(factor, units);

                currentPolygonOffsetFactor = factor;
                currentPolygonOffsetUnits = units;
            }
        }
        else {
            disable(GL_POLYGON_OFFSET_FILL);
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
//    GLState &activeTexture( int glSlot = -1 ) {
//        //if ( webglSlot === undefined ) webglSlot = gl.TEXTURE0 + maxTextures - 1;
//        if ( currentTextureSlot != glSlot ) {
//            glActiveTexture( glSlot );
//            currentTextureSlot = glSlot;
//        }
//        return *this;
//    }
    GLState &activeTexture(int glSlot=-1) {
        if (glSlot == -1) glSlot = GL_TEXTURE0 + maxTextures - 1;
        if (currentTextureSlot !=  glSlot) {
            glActiveTexture(glSlot);
            currentTextureSlot = glSlot;
        }
        return *this;
    }

//    GLState &bindTexture( GLenum glType, GLuint glTexture ) {
//        if ( currentTextureSlot == -1 ) {
//            activeTexture();
//        }
//
//        BoundTexture boundTexture{GL_NONE,0};
//        if(currentBoundTextures.size() >= currentTextureSlot)
//            boundTexture = currentBoundTextures[ currentTextureSlot ];
////        if ( boundTexture == undefined ) {
////            boundTexture = { type: undefined, texture: undefined };
////            currentBoundTextures[ currentTextureSlot ] = boundTexture;
////        }
//
//        if ( boundTexture.type != glType || boundTexture.texture != glTexture ) {
//
//            glBindTexture( glType, glTexture /**|| emptyTextures[ glType ]*/ );
//
//            boundTexture.type = glType;
//            boundTexture.texture = glTexture;
//        }
//        return *this;
//    }
    GLState &bindTexture(threecpp::TextureTarget target, GLint texture = -1) {
        if (currentTextureSlot < 0 ) {
            activeTexture();
        }

        /*if (currentTextureSlot == 33985)
            std::cout << "CurrentTextureSlot :" << currentTextureSlot << " texture:" << texture << std::endl;*/

        BoundTexture* boundTexture;

        if (texture == 0)
            std::cout << "Texture is 0" << std::endl;

        auto found = currentBoundTextures.find(currentTextureSlot);
        if (found == currentBoundTextures.end()) {
            currentBoundTextures.emplace(currentTextureSlot, BoundTexture(threecpp::TextureTarget::None, -1));
            boundTexture = &currentBoundTextures[currentTextureSlot];
        }
        else {
            boundTexture = &found->second;
        }

        if (boundTexture->target != target || boundTexture->texture !=  texture) {
            if(texture>0)
                glBindTexture((GLenum)target, (GLuint)texture);
            else
                glBindTexture((GLenum)target,emptyTextures[(GLuint)target]);

            boundTexture->target = target;
            boundTexture->texture = texture;

        }
        return *this;
    }

//    GLState &unbindTexture() {
//        if(currentBoundTextures.size()>=currentTextureSlot) {
//            BoundTexture boundTexture = currentBoundTextures[currentTextureSlot];
//
//            glBindTexture(boundTexture.type, 0);
//
//            boundTexture.type = GL_NONE;
//            boundTexture.texture = 0;
//        }
//        return *this;
//    }
    GLState &unbindTexture() {
        BoundTexture* boundTexture;
        auto found = currentBoundTextures.find(currentTextureSlot);
        if (found != currentBoundTextures.end()) {
            boundTexture = &found->second;
            glBindTexture((GLenum)boundTexture->target, 0);

            boundTexture->target = threecpp::TextureTarget::None;
            boundTexture->texture = -1;
        }

        return *this;
    }

    void compressedTexImage2D(GLenum target, GLint level, GLint internalFormat,GLsizei width, GLsizei height, const std::vector<unsigned char>& data)
    {
        glCompressedTexImage2D(target, level, internalFormat, width, height, 0, data.size(), data.data());
    }

    void texImage2D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border,GLenum format, GLenum type, byte* pixels)
    {
        glTexImage2D(target, level, internalFormat, width, height, border, format, type, (const void*)pixels);
    }
    void texImage2D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const std::vector<unsigned char>& pixels)
    {
        glTexImage2D(target, level, internalFormat, width, height, border, format, type, (const void*) &pixels[0]);
    }

    void texImage2D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type)
    {
        glTexImage2D(target, level, internalFormat, width, height, border, format, type, nullptr);
    }

    void texImage2D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const threecpp::MipMap& mipmap)
    {
        glTexImage2D(target, level, internalFormat, width, height, border, format, type, mipmap.data.data());
    }

    void texImage3D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth,GLint border,GLenum format,GLenum type, threecpp::byte* pixels)
    {
        glTexImage3D(target, level, format, width, height, depth, border, format, type, pixels);
    }

    void texImage3D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const threecpp::MipMap& mipmap)
    {
        glTexImage3D(target, level, (GLenum)format, width, height, depth, border, (GLenum)format, (GLenum)type, mipmap.data.data());
    }

    void texImage3D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const std::vector<byte>& pixels)
    {
        glTexImage3D(target, level, format, width, height, depth, border, format, type, &pixels[0]);
    }

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

//    GLState &reset() {
//
//        // reset state
//        glDisable( GL_BLEND );
//        glDisable( GL_CULL_FACE );
//        glDisable( GL_DEPTH_TEST );
//        glDisable( GL_POLYGON_OFFSET_FILL );
//        glDisable( GL_SCISSOR_TEST );
//        glDisable( GL_STENCIL_TEST );
//        glDisable( GL_SAMPLE_ALPHA_TO_COVERAGE );
//
//        glBlendEquation( GL_FUNC_ADD );
//        glBlendFunc( GL_ONE, GL_ZERO );
//        glBlendFuncSeparate( GL_ONE, GL_ZERO, GL_ONE, GL_ZERO );
//
//        glColorMask( true, true, true, true );
//        glClearColor( 0, 0, 0, 0 );
//
//        glDepthMask( true );
//        glDepthFunc( GL_LESS );
//        glClearDepthf( 1 );
//
//        glStencilMask( 0xffffffff );
//        glStencilFunc( GL_ALWAYS, 0, 0xffffffff );
//        glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
//        glClearStencil( 0 );
//
//        glCullFace( GL_BACK );
//        glFrontFace( GL_CCW );
//
//        glPolygonOffset( 0, 0 );
//
//        glActiveTexture( GL_TEXTURE0 );
//
//        glBindFramebuffer( GL_FRAMEBUFFER, 0 );
//
//        if ( isWebGL2 == true ) {
//
//            glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
//            glBindFramebuffer( GL_READ_FRAMEBUFFER, 0 );
//
//        }
//
//        glUseProgram( 0 );
//
//        glLineWidth( 1 );
//
//        glScissor( 0, 0, defaultViewPort.width, defaultViewPort.height );
//        glViewport( 0, 0, defaultViewPort.width, defaultViewPort.height );
//
//        // reset internals
//
//        enabledCapabilities = {};
//
//        currentTextureSlot = -1;
//        currentBoundTextures = {};
//
//        currentBoundFramebuffers = {};
////        currentDrawbuffers = new WeakMap();
////        defaultDrawbuffers = [];
//
//        GLuint currentProgram = 0;
//
//        currentBlendingEnabled = false;
//        currentBlending = -1;
//        currentBlendEquation = -1;
//        currentBlendSrc = -1;
//        currentBlendDst = -1;
//        currentBlendEquationAlpha = -1;
//        currentBlendSrcAlpha = -1;
//        currentBlendDstAlpha = -1;
//        currentPremultipledAlpha = false;
//
//        currentFlipSided = -1;
//        currentCullFace = -1;
//
//        currentLineWidth = -1;
//
//        currentPolygonOffsetFactor = -1;
//        currentPolygonOffsetUnits = -1;
//
//        currentScissor.set( 0, 0, defaultViewPort.width, defaultViewPort.height );
//        currentViewport.set( 0, 0, defaultViewPort.width, defaultViewPort.height );
//
//        colorBuffer.reset();
//        depthBuffer.reset();
//        stencilBuffer.reset();
//
//    }

    GLState &reset() {
        for (unsigned i = 0; i < enabledAttributes.size(); i++) {
            glDisableVertexAttribArray(i);
            enabledAttributes[i] = 0;
        }

        enabledCapabilities.clear();

        compressedTextureFormats.clear();

        currentTextureSlot = -1;

        currentBoundTextures.clear();

        currentProgram = -1;

        currentBlending = Blending::NoBlending;

        currentFlipSided = false;

        currentCullFace = CullFace::CullFaceNone;

        colorBuffer.reset();
        depthBuffer.reset();
        stencilBuffer.reset();

        return *this;
    }

};




#endif //THREE_CPP_SRC_RENDERS_GL_STATE_H
