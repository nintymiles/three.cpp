//
// Created by SeanRen on 2022/10/6.
//

#ifndef THREE_CPP_GL_RENDER_TARGET_H
#define THREE_CPP_GL_RENDER_TARGET_H

//#include "event_dispatcher.h"
#include "vector4.h"
#include "constants.h"
#include "texture.h"
#include "depth_texture.h"

struct GLRenderTargetParameter {
    TextureMapping mapping = TextureMapping::UVMapping;
    Wrapping wrapS = Wrapping::ClampToEdgeWrapping;
    Wrapping wrapT = Wrapping::ClampToEdgeWrapping;
    TextureFilter magFilter = TextureFilter::LinearFilter;
    TextureFilter minFilter = TextureFilter::LinearMipMapLinearFilter;
    PixelFormat format = PixelFormat::RGBAFormat;
    TextureDataType type = TextureDataType::UnsignedByteType;
    int anisotropy = 1;
    TextureEncoding encoding = TextureEncoding::LinearEncoding;
    bool generateMipmaps = false;
    bool depthBuffer = true;
    bool stencilBuffer = false;
    DepthTexture::sptr depthTexture = nullptr;
};
class GLRenderTarget {
    static size_t renderTargetId;
public:
    using sptr = std::shared_ptr<GLRenderTarget>;

    std::string type;
    size_t id;



    bool isGLMultiviewRenderTarget = false;

    int numViews = 0;

    sole::uuid uuid = sole::uuid1();

    size_t width = 0;

    size_t height = 0;

    Vector4f scissor;

    bool scissorTest = false;

    Vector4f viewport;

    Texture::sptr texture = nullptr;

    bool depthBuffer = false;

    bool stencilBuffer = false;

    DepthTexture::sptr depthTexture = nullptr;

    GLuint samples = 0;

    //ThreeDataTable options;
    std::unordered_map<std::string, int> options;

    bool isGLMultisampleRenderTarget = false;
    bool isGLCubeRenderTarget = false;

    GLRenderTarget() : id(renderTargetId++){
        type = "GLRenderTarget";
    }

    GLRenderTarget(size_t width, size_t height,GLRenderTargetParameter* options = nullptr);

    GLRenderTarget(GLRenderTarget& source);

    bool isEmpty()  const {
        return id == std::numeric_limits<size_t>::infinity() || id == 0;
    }
    virtual ~GLRenderTarget() = default;

    void setSize(size_t width, size_t height);

    GLRenderTarget& clone(GLRenderTarget* target);

    GLRenderTarget& copy(const GLRenderTarget& source);

    virtual void dispose();

    threecpp::Signal<void(GLRenderTarget&)> onDispose;



    GLRenderTarget& operator = (const GLRenderTarget& source) {
        return copy(*this);
    }
};

//struct RenderOptions{
//    bool generateMipmaps = false;
//    int internalFormat;
//    int minFilter;  //enum default linearFilter
//    bool depthBuffer = true;
//    bool stencilBuffer = false;
//    int depthTexture;
//    int samples;
//
//};
//
///*
// In options, we can specify:
// * Texture parameters for an auto-generated target texture
// * depthBuffer/stencilBuffer: Booleans to indicate if we should generate these buffers
//*/
//class GLRenderTarget:EventDispatcher{
//public:
//    bool isGLRenderTarget = true;
//    double width,height,depth = 1;
//    RenderOptions* options;
//    Vector4d viewport,scissor;
//    bool scissorTest=false;
//    //viewport,scissor,texture,depthBuffer,stencilBuffer
//
//    GLRenderTarget(double height,double width,RenderOptions* options):height(height),width(width),
//                        viewport(0,0,width,height),scissor(0,0,width,height),options(options){};
//
//
//    GLRenderTarget &setSize(double width, double height, double depth = 1) {
//
//        if (this->width != width || this->height != height || this->depth != depth) {
//
//            width = width;
//            height = height;
//            depth = depth;
//
//            //texture.image.width = width;
//            //texture.image.height = height;
//            //texture.image.depth = depth;
//
//            dispose();
//        }
//
//        //viewport.set( 0, 0, width, height );
//        //scissor.set( 0, 0, width, height );
//
//        return *this;
//    }
//
//    GLRenderTarget& dispose() {
//        dispatchEvent({type: "dispose"});
//        return *this;
//    }
//
//};

//import { EventDispatcher } from '../core/EventDispatcher.js';
//import { Texture } from '../textures/Texture.js';
//import { LinearFilter } from '../constants.js';
//import { Vector4 } from '../math/Vector4.js';
//import { Source } from '../textures/Source.js';



//class WebGLRenderTarget extends EventDispatcher {
//
//constructor( width, height, options = {} ) {
//
//super();
//
//this.isWebGLRenderTarget = true;
//
//this.width = width;
//this.height = height;
//this.depth = 1;
//
//this.scissor = new Vector4( 0, 0, width, height );
//this.scissorTest = false;
//
//this.viewport = new Vector4( 0, 0, width, height );
//
//const image = { width: width, height: height, depth: 1 };
//
//this.texture = new Texture( image, options.mapping, options.wrapS, options.wrapT, options.magFilter, options.minFilter, options.format, options.type, options.anisotropy, options.encoding );
//this.texture.isRenderTargetTexture = true;
//
//this.texture.flipY = false;
//this.texture.generateMipmaps = options.generateMipmaps !== undefined ? options.generateMipmaps : false;
//this.texture.internalFormat = options.internalFormat !== undefined ? options.internalFormat : null;
//this.texture.minFilter = options.minFilter !== undefined ? options.minFilter : LinearFilter;
//
//this.depthBuffer = options.depthBuffer !== undefined ? options.depthBuffer : true;
//this.stencilBuffer = options.stencilBuffer !== undefined ? options.stencilBuffer : false;
//
//this.depthTexture = options.depthTexture !== undefined ? options.depthTexture : null;
//
//this.samples = options.samples !== undefined ? options.samples : 0;
//
//}

//clone() {
//
//    return new this.constructor().copy( this );
//
//}
//
//copy( source ) {
//
//        this.width = source.width;
//        this.height = source.height;
//        this.depth = source.depth;
//
//        this.viewport.copy( source.viewport );
//
//        this.texture = source.texture.clone();
//        this.texture.isRenderTargetTexture = true;
//
//        // ensure image object is not shared, see #20328
//
//        const image = Object.assign( {}, source.texture.image );
//        this.texture.source = new Source( image );
//
//        this.depthBuffer = source.depthBuffer;
//        this.stencilBuffer = source.stencilBuffer;
//
//        if ( source.depthTexture !== null ) this.depthTexture = source.depthTexture.clone();
//
//        this.samples = source.samples;
//
//        return this;
//
//}
//

//
//}
//
//export { WebGLRenderTarget };



#endif //THREE_CPP_GL_RENDER_TARGET_H
