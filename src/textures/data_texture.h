//
// Created by SeanR on 2023/10/18.
//

#ifndef THREE_CPP_DATA_TEXTURE_H
#define THREE_CPP_DATA_TEXTURE_H

#include "texture.h"

class DataTexture : public Texture {
public:
    using sptr = std::shared_ptr<DataTexture>;

    DataTexture(unsigned width=0,unsigned height=0,TextureMapping mapping = TextureMapping::UVMapping, Wrapping wraps = Wrapping::ClampToEdgeWrapping,
                Wrapping wrapt = Wrapping::ClampToEdgeWrapping, TextureFilter magFilter = TextureFilter::NearestFilter,
                TextureFilter minFilter = TextureFilter::NearestFilter, PixelFormat format = PixelFormat::RGBAFormat,
                TextureDataType type = TextureDataType::UnsignedByteType, unsigned anisotropy = 1,
                TextureEncoding encoding = TextureEncoding::LinearEncoding)
            :Texture(std::make_shared<TexImageInfo>(),mapping,wraps, wrapt, magFilter, minFilter, format, type, anisotropy, encoding){
        this->isDataTexture = true;
        this->image->width = width;
        this->image->height = height;
        this->generateMipmaps = false;
        this->flipY = false;
        this->unpackAlignment = 1;
        this->setNeedsUpdate(true);
    }

    DataTexture(std::vector<unsigned char> image,unsigned width = 0, unsigned height = 0, TextureMapping mapping = TextureMapping::UVMapping,
                Wrapping wraps = Wrapping::ClampToEdgeWrapping, Wrapping wrapt = Wrapping::ClampToEdgeWrapping,
                TextureFilter magFilter = TextureFilter::NearestFilter, TextureFilter minFilter = TextureFilter::NearestFilter,
                PixelFormat format = PixelFormat::RGBAFormat, TextureDataType type = TextureDataType::UnsignedByteType,
                unsigned anisotropy = 1, TextureEncoding encoding = TextureEncoding::LinearEncoding)
            :Texture(std::make_shared<TexImageInfo>(),mapping, wraps, wrapt, magFilter, minFilter, format, type, anisotropy, encoding){
        this->isDataTexture = true;
        this->image->imageData = image;
        this->image->width = width;
        this->image->height = height;
        this->generateMipmaps = false;
        this->flipY = false;
        this->unpackAlignment = 1;
        this->setNeedsUpdate(true);
    }

    static sptr create(unsigned width = 0, unsigned height = 0, TextureMapping mapping = TextureMapping::UVMapping,
                       Wrapping wraps = Wrapping::ClampToEdgeWrapping, Wrapping wrapt = Wrapping::ClampToEdgeWrapping,
                       TextureFilter magFilter = TextureFilter::NearestFilter, TextureFilter minFilter = TextureFilter::NearestFilter,
                       PixelFormat format = PixelFormat::RGBAFormat, TextureDataType type = TextureDataType::UnsignedByteType,
                       unsigned anisotropy = 1, TextureEncoding encoding = TextureEncoding::LinearEncoding) {
        return std::make_shared<DataTexture>(width, height, mapping, wraps, wrapt, magFilter, minFilter, format, type, anisotropy, encoding);
    }

    static sptr create(std::vector<unsigned char> image,unsigned width = 0, unsigned height = 0, TextureMapping mapping = TextureMapping::UVMapping,
                       Wrapping wraps = Wrapping::ClampToEdgeWrapping, Wrapping wrapt = Wrapping::ClampToEdgeWrapping,
                       TextureFilter magFilter = TextureFilter::NearestFilter, TextureFilter minFilter = TextureFilter::NearestFilter,
                       PixelFormat format = PixelFormat::RGBAFormat, TextureDataType type = TextureDataType::UnsignedByteType,
                       unsigned anisotropy = 1, TextureEncoding encoding = TextureEncoding::LinearEncoding) {
        return std::make_shared<DataTexture>(image, width, height, mapping, wraps, wrapt, magFilter, minFilter, format, type, anisotropy, encoding);
    }
};

#endif //THREE_CPP_DATA_TEXTURE_H
