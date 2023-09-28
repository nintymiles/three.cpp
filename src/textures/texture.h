//
// Created by SeanR on 2022/12/11.
//

#ifndef THREE_CPP_SRC_TEXTURE_H
#define THREE_CPP_SRC_TEXTURE_H

#include "event_dispatcher.h"
#include "constants.h"
#include "math_utils.h"
#include "vector2.h"

#include <fstream>
#include <string>

static int _textureId{0};
class Texture:public EventDispatcher{
public:
    int id = _textureId++;
    std::string UUID = MathUtils::generate_uuid();
    std::string name = "";


    //this.source = new Source( image );
//this.mipmaps = [];

    int mapping;

    int wrapS;
    int wrapT;

    int magFilter;
    int minFilter;

    int anisotropy;

    int format;
//  int internalFormat = null;

    int type = type;

    static std::fstream DEFAULT_IMAGE;
    static const int DEFAULT_MAPPING = UVMapping;

    Vector2 offset = {0, 0};
    Vector2 repeat = {1, 1};
    Vector2 center = {0, 0} ;
    double rotation = 0;

    bool matrixAutoUpdate = true;
    Matrix3 matrix;

    bool generateMipmaps = true;
    bool premultiplyAlpha = false;
    bool flipY = true;

    int unpackAlignment = 4;	// valid values: 1, 2, 4, 8 (see http://www.khronos.org/opengles/sdk/docs/man/xhtml/glPixelStorei.xml)

    // Values of encoding !== THREE.LinearEncoding only supported on map, envMap and emissiveMap.
    // Also changing the encoding after already used by a Material will not automatically make the Material
    // update. You need to explicitly call Material.needsUpdate to trigger it to recompile.
    int encoding = encoding;

//this.userData = {};

    int version = 0;
//this.onUpdate = null;

    bool isRenderTargetTexture = false; // indicates whether a texture belongs to a render target or not
    bool needsPMREMUpdate = false; // indicates whether this texture should be processed by PMREMGenerator or not (only relevant for render target textures)

    Texture(std::fstream image, int mapping = DEFAULT_MAPPING,
            int wrapS = ClampToEdgeWrapping, int wrapT = ClampToEdgeWrapping,
            int magFilter = LinearFilter, int minFilter = LinearMipmapLinearFilter,
            int format = RGBAFormat, int type = UnsignedByteType,
            int anisotropy = 1, int encoding = LinearEncoding)
            :mapping(mapping),wrapS{wrapS},wrapT(wrapT),magFilter(magFilter),minFilter(minFilter),
             format(format),type(type),anisotropy(anisotropy),encoding(encoding){}

//
//get image() {
//
//    return this.source.data;
//
//}
//
//set image( value ) {
//
//    this.source.data = value;
//
//}

    Texture &updateMatrix(){
        this->matrix.setUvTransform( this->offset.x, this->offset.y,
                                     this->repeat.x, this->repeat.y,
                                     this->rotation, this->center.x, this->center.y );
        return *this;
    }


    Texture &dispose() {
        this->dispatchEvent( Event{ "type", nullptr } );

        return *this;
    }

    Vector2 &transformUv( Vector2 &uv ) {
        if ( this->mapping != UVMapping ) return uv;

        uv.applyMatrix3( this->matrix );

        if ( uv.x < 0 || uv.x > 1 ) {

            switch ( this->wrapS ) {

                case RepeatWrapping:
                    uv.x = uv.x - floor( uv.x );
                    break;
                case ClampToEdgeWrapping:
                    uv.x = uv.x < 0 ? 0 : 1;
                    break;
                case MirroredRepeatWrapping:
                    if ( std::abs( (int)std::floor( uv.x ) % 2 ) == 1 ) {
                        uv.x = std::ceil( uv.x ) - uv.x;
                    } else {
                        uv.x = uv.x - std::floor( uv.x );
                    }
                    break;

            }

        }

        if ( uv.y < 0 || uv.y > 1 ) {

            switch ( this->wrapT ) {

                case RepeatWrapping:
                    uv.y = uv.y - std::floor( uv.y );
                    break;
                case ClampToEdgeWrapping:
                    uv.y = uv.y < 0 ? 0 : 1;
                    break;
                case MirroredRepeatWrapping:
                    if ( (int)std::abs( std::floor( uv.y ) % 2 ) == 1 ) {
                        uv.y = std::ceil( uv.y ) - uv.y;
                    } else {
                        uv.y = uv.y - std::floor( uv.y );
                    }
                    break;

            }

        }

        if ( this->flipY ) {
            uv.y = 1 - uv.y;
        }

        return uv;
    }

    Texture &needsUpdate( bool value ) {
        if ( value == true ) {
            this->version ++;
            this->source.needsUpdate = true;
        }

    }


};



//clone() {
//
//    return new this.constructor().copy( this );
//
//}
//
//copy( source ) {
//
//    this.name = source.name;
//
//    this.source = source.source;
//    this.mipmaps = source.mipmaps.slice( 0 );
//
//    this.mapping = source.mapping;
//
//    this.wrapS = source.wrapS;
//    this.wrapT = source.wrapT;
//
//    this.magFilter = source.magFilter;
//    this.minFilter = source.minFilter;
//
//    this.anisotropy = source.anisotropy;
//
//    this.format = source.format;
//    this.internalFormat = source.internalFormat;
//    this.type = source.type;
//
//    this.offset.copy( source.offset );
//    this.repeat.copy( source.repeat );
//    this.center.copy( source.center );
//    this.rotation = source.rotation;
//
//    this.matrixAutoUpdate = source.matrixAutoUpdate;
//    this.matrix.copy( source.matrix );
//
//    this.generateMipmaps = source.generateMipmaps;
//    this.premultiplyAlpha = source.premultiplyAlpha;
//    this.flipY = source.flipY;
//    this.unpackAlignment = source.unpackAlignment;
//    this.encoding = source.encoding;
//
//    this.userData = JSON.parse( JSON.stringify( source.userData ) );
//
//    this.needsUpdate = true;
//
//    return this;
//
//}
//
//toJSON( meta ) {
//
//    const isRootObject = ( meta === undefined || typeof meta === 'string' );
//
//    if ( ! isRootObject && meta.textures[ this.uuid ] !== undefined ) {
//
//        return meta.textures[ this.uuid ];
//
//    }
//
//    const output = {
//
//        metadata: {
//        version: 4.5,
//        type: 'Texture',
//            generator: 'Texture.toJSON'
//    },
//
//        uuid: this.uuid,
//        name: this.name,
//
//            image: this.source.toJSON( meta ).uuid,
//
//            mapping: this.mapping,
//
//            repeat: [ this.repeat.x, this.repeat.y ],
//        offset: [ this.offset.x, this.offset.y ],
//        center: [ this.center.x, this.center.y ],
//        rotation: this.rotation,
//
//        wrap: [ this.wrapS, this.wrapT ],
//
//        format: this.format,
//        type: this.type,
//            encoding: this.encoding,
//
//            minFilter: this.minFilter,
//            magFilter: this.magFilter,
//            anisotropy: this.anisotropy,
//
//            flipY: this.flipY,
//
//            premultiplyAlpha: this.premultiplyAlpha,
//            unpackAlignment: this.unpackAlignment
//
//    };
//
//    if ( JSON.stringify( this.userData ) !== '{}' ) output.userData = this.userData;
//
//    if ( ! isRootObject ) {
//
//        meta.textures[ this.uuid ] = output;
//
//    }
//
//    return output;
//
//}

//
//}
//
//Texture.DEFAULT_IMAGE = null;
//Texture.DEFAULT_MAPPING = UVMapping;
//
//Texture.prototype.isTexture = true;
//
//export { Texture };

#endif //THREE_CPP_SRC_TEXTURE_H