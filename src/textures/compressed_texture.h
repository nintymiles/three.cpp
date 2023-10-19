//
// Created by SeanR on 2023/10/19.
//

#ifndef THREE_CPP_COMPRESSED_TEXTURE_H
#define THREE_CPP_COMPRESSED_TEXTURE_H

#include "texture.h"
#include "common_types.h"
class CompressedTexture : public Texture {
    CompressedTexture(std::vector<threecpp::MipMap> mipmaps, unsigned width = 0, unsigned height = 0, TextureMapping mapping = TextureMapping::UVMapping,
                      Wrapping wraps = Wrapping::ClampToEdgeWrapping, Wrapping wrapt = Wrapping::ClampToEdgeWrapping, TextureFilter magFilter = TextureFilter::NearestFilter,
                      TextureFilter minFilter = TextureFilter::NearestFilter, PixelFormat format = PixelFormat::RGBAFormat, TextureDataType type = TextureDataType::UnsignedByteType,
                      unsigned anisotropy = 1, TextureEncoding encoding = TextureEncoding::LinearEncoding)
            :Texture(std::vector<unsigned char>(), mapping, wraps, wrapt, magFilter, minFilter, format, type, anisotropy, encoding)
    {
        this->imageWidth = width;
        this->imageHeight = height;
        this->generateMipmaps = false;
        this->flipY = false;
    }
};

#endif //THREE_CPP_COMPRESSED_TEXTURE_H
