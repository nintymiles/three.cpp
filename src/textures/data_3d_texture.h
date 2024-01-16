//
// Created by SeanR on 11/12/2023.
//

#ifndef THREE_CPP_DATA_3D_TEXTURE_H
#define THREE_CPP_DATA_3D_TEXTURE_H

#include "texture.h"

class Data3DTexture : public Texture{
public:
    using sptr = std::shared_ptr<Data3DTexture>;

    Data3DTexture(std::vector<unsigned char> data = {},unsigned width = 0, unsigned height = 0, unsigned depth = 1)
            :Texture(std::make_shared<TexImageInfo>(),mapping, Wrapping::ClampToEdgeWrapping, Wrapping::ClampToEdgeWrapping, TextureFilter::NearestFilter, TextureFilter::NearestFilter, PixelFormat::RGBAFormat, TextureDataType::UnsignedByteType, 1, TextureEncoding::LinearEncoding){
        this->isDataTexture3D = true;
        this->image->imageData = data;
        this->image->width = width;
        this->image->height = height;
        this->image->depth = depth;
        this->generateMipmaps = false;
        this->flipY = false;
        this->unpackAlignment = 1;
        this->setNeedsUpdate(true);
    }


    static sptr create(std::vector<unsigned char> data = {},unsigned width = 0, unsigned height = 0, float depth = 1) {
        return std::make_shared<Data3DTexture>(data, width, height,depth);
    }
};

#endif //THREE_CPP_DATA_3D_TEXTURE_H
