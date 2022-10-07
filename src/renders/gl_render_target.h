//
// Created by SeanRen on 2022/10/6.
//

#ifndef THREE_CPP_GL_RENDER_TARGET_H
#define THREE_CPP_GL_RENDER_TARGET_H

#include "event_dispatcher.h"
#include "vector4.h"

struct RenderOptions{
    bool generateMipmaps = false;
    int internalFormat;
    int minFilter;  //enum default linearFilter
    bool depthBuffer = true;
    bool stencilBuffer = false;
    int depthTexture;
    int samples;

};

/*
 In options, we can specify:
 * Texture parameters for an auto-generated target texture
 * depthBuffer/stencilBuffer: Booleans to indicate if we should generate these buffers
*/
class GlRenderTarget:EventDispatcher{
public:
    double width,height,depth;
    RenderOptions* options;
    //viewport,scissor,texture,depthBuffer,stencilBuffer

    GlRenderTarget(double height,double width,RenderOptions* options):height(height),width(width), options(options){};


    GlRenderTarget &setSize(double width, double height, double depth = 1) {

        if (this->width != width || this->height != height || this->depth != depth) {

            width = width;
            height = height;
            depth = depth;

            //texture.image.width = width;
            //texture.image.height = height;
            //texture.image.depth = depth;

            dispose();
        }

        //viewport.set( 0, 0, width, height );
        //scissor.set( 0, 0, width, height );

        return *this;
    }

    GlRenderTarget& dispose() {
        dispatchEvent( { type: 'dispose' } );
        return *this;
    }

};

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
