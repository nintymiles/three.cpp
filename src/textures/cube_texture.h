//
// Created by SeanR on 2023/10/18.
//

#ifndef THREE_CPP_CUBE_TEXTURE_H
#define THREE_CPP_CUBE_TEXTURE_H

#include "image_loader.h"

class CubeTexture : public Texture {
    //std::vector<Texture::sptr> images;
public:
    using sptr = std::shared_ptr<CubeTexture>;
    CubeTexture(std::vector<Texture::sptr> images = {}, TextureMapping mapping = TextureMapping::CubeReflectionMapping, Wrapping wraps = Wrapping::ClampToEdgeWrapping,
                Wrapping wrapt = Wrapping::ClampToEdgeWrapping, TextureFilter magFilter = TextureFilter::LinearFilter, TextureFilter minFilter = TextureFilter::LinearMipmapLinearFilter,
                PixelFormat format = PixelFormat::RGBAFormat, TextureDataType type = TextureDataType::UnsignedByteType, unsigned anisotropy = 1, TextureEncoding encoding = TextureEncoding::LinearEncoding)
            :Texture(std::make_shared<TexImageInfo>(), mapping, wraps, wrapt, magFilter, minFilter, format, type, anisotropy, encoding) {
        this->images = images;
        flipY = false;
        isCubeTexture = true;
    }

    static sptr create(std::vector<Texture::sptr> images = {},
                       TextureMapping mapping = TextureMapping::CubeReflectionMapping,
                       Wrapping wraps = Wrapping::ClampToEdgeWrapping,
                       Wrapping wrapt = Wrapping::ClampToEdgeWrapping,
                       TextureFilter magFilter = TextureFilter::LinearFilter,
                       TextureFilter minFilter = TextureFilter::LinearMipmapLinearFilter,
                       PixelFormat format = PixelFormat::RGBAFormat,
                       TextureDataType type = TextureDataType::UnsignedByteType,
                       unsigned anisotropy = 1,
                       TextureEncoding encoding = TextureEncoding::LinearEncoding) {
        return std::make_shared<CubeTexture>(images, mapping, wraps, wrapt, magFilter, minFilter, format, type, anisotropy);
    }



};

#endif //THREE_CPP_CUBE_TEXTURE_H
