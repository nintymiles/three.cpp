//
// Created by SeanR on 2023/10/18.
//

#ifndef THREE_CPP_CUBE_TEXTURE_H
#define THREE_CPP_CUBE_TEXTURE_H

#include "texture.h"

class CubeTexture : public Texture {
public:
    using sptr = std::shared_ptr<CubeTexture>;
    CubeTexture(std::vector<unsigned char> image = std::vector<unsigned char>(), TextureMapping mapping = TextureMapping::CubeReflectionMapping, Wrapping wraps = Wrapping::ClampToEdgeWrapping, Wrapping wrapt = Wrapping::ClampToEdgeWrapping, TextureFilter magFilter = TextureFilter::LinearFilter, TextureFilter minFilter = TextureFilter::LinearMipMapLinearFilter, PixelFormat format = PixelFormat::RGBAFormat, TextureDataType type = TextureDataType::UnsignedByteType, unsigned anisotropy = 1, TextureEncoding encoding = TextureEncoding::LinearEncoding)
            :Texture(image, mapping, wraps, wrapt, magFilter, minFilter, format, type, anisotropy, encoding) {
        flipY = false;
    }

};

#endif //THREE_CPP_CUBE_TEXTURE_H
