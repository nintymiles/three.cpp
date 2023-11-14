//
// Created by ultraman on 11/12/2023.
//

#ifndef THREE_CPP_DATA_3D_TEXTURE_H
#define THREE_CPP_DATA_3D_TEXTURE_H

#include "texture.h"

class Data3DTexture : public Texture{
public:
    using sptr = std::shared_ptr<Data3DTexture>;

//    Data3DTexture(unsigned width=0,unsigned height=0,TextureMapping mapping = TextureMapping::UVMapping, Wrapping wraps = Wrapping::ClampToEdgeWrapping, Wrapping wrapt = Wrapping::ClampToEdgeWrapping, TextureFilter magFilter = TextureFilter::NearestFilter, TextureFilter minFilter = TextureFilter::NearestFilter, PixelFormat format = PixelFormat::RGBAFormat, TextureDataType type = TextureDataType::UnsignedByteType, unsigned anisotropy = 1, TextureEncoding encoding = TextureEncoding::LinearEncoding)
//            :Texture(std::vector<unsigned char>(),mapping,wraps, wrapt, magFilter, minFilter, format, type, anisotropy, encoding){
//        this->isDataTexture = true;
//        this->imageWidth = width;
//        this->imageHeight = height;
//        this->generateMipmaps = false;
//        this->flipY = false;
//        this->unpackAlignment = 1;
//        this->setNeedsUpdate(true);
//    }

    Data3DTexture(const std::vector<unsigned char>& image,unsigned width = 0, unsigned height = 0, float depth = 1.f)
            :Texture(image,mapping, Wrapping::ClampToEdgeWrapping, Wrapping::ClampToEdgeWrapping, TextureFilter::NearestFilter, TextureFilter::NearestFilter, PixelFormat::RGBAFormat, TextureDataType::UnsignedByteType, 1, TextureEncoding::LinearEncoding){
        this->isDataTexture = true;
        this->imageWidth = width;
        this->imageHeight = height;
        this->generateMipmaps = false;
        this->flipY = false;
        this->unpackAlignment = 1;
        this->setNeedsUpdate(true);
    }

//    static sptr create(unsigned width = 0, unsigned height = 0, TextureMapping mapping = TextureMapping::UVMapping, Wrapping wraps = Wrapping::ClampToEdgeWrapping, Wrapping wrapt = Wrapping::ClampToEdgeWrapping, TextureFilter magFilter = TextureFilter::NearestFilter, TextureFilter minFilter = TextureFilter::NearestFilter, PixelFormat format = PixelFormat::RGBAFormat, TextureDataType type = TextureDataType::UnsignedByteType, unsigned anisotropy = 1, TextureEncoding encoding = TextureEncoding::LinearEncoding) {
//        return std::make_shared<Data3DTexture>(std::vector<unsigned char>(),width, height, mapping, wraps, wrapt, magFilter, minFilter, format, type, anisotropy, encoding);
//    }
    static sptr create(const std::vector<unsigned char>& image = std::vector<unsigned char>{},unsigned width = 0, unsigned height = 0, float depth = 1) {
        return std::make_shared<Data3DTexture>(image, width, height,depth);
    }
};

#endif //THREE_CPP_DATA_3D_TEXTURE_H
