//
// Created by SeanR on 2023/10/11.
//

#ifndef THREE_CPP_DEPTH_TEXTURE_H
#define THREE_CPP_DEPTH_TEXTURE_H

#include "texture.h"

#include "common_types.h"

class DepthTexture : public Texture {
private:
    size_t width, height;
public:
    using sptr = std::shared_ptr<DepthTexture>;

    DepthTexture(size_t width, size_t height,
                 TextureDataType type = TextureDataType::None,
                 TextureMapping mapping = TextureMapping::UVMapping,
                 Wrapping wraps = Wrapping::ClampToEdgeWrapping,
                 Wrapping wrapt = Wrapping::ClampToEdgeWrapping,
                 TextureFilter magFilter = TextureFilter::NearestFilter,
                 TextureFilter minFilter = TextureFilter::NearestFilter,
                 unsigned anisotropy = 1,
                 PixelFormat  format = PixelFormat::None):Texture( {}, mapping, wraps, wrapt, magFilter, minFilter, format, type, anisotropy ){

        if( format == PixelFormat::None ) format = PixelFormat::DepthFormat;
        if ( format != PixelFormat::DepthFormat && format != PixelFormat::DepthStencilFormat ) {
            std::cerr << "DepthTexture format must be either DepthFormat or DepthStencilFormat" << std::endl;
        }
        if(type == TextureDataType::None && format == PixelFormat::DepthFormat) type = TextureDataType::UnsignedIntType;
        if(type == TextureDataType::None && format == PixelFormat::DepthStencilFormat) type = TextureDataType::UnsignedInt248Type;

//        Texture( {}, mapping, wrapS, wrapT, magFilter, minFilter, format, type, anisotropy );
        isDepthTexture = true;

        imageWidth = width;
        imageHeight = height;

        flipY = false;
        generateMipmaps = false;
    };
};

#endif //THREE_CPP_DEPTH_TEXTURE_H
