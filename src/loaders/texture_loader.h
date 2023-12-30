#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include "texture.h"

#include <stb_image.h>
#include <string>

class TextureLoader {
public:
    TextureLoader(){}
    static Texture::sptr load(const std::string& filePath,bool isFlipY = true) {

        Texture::sptr texture = nullptr;

        int width, height, nrComponents;

        if(isFlipY) stbi_set_flip_vertically_on_load(1);

        unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrComponents, 0);


        if (data) {
            std::vector<unsigned char> image(width * height * nrComponents);
            std::memcpy(&image[0], data, width * height * nrComponents);
            //delete[] data;
            stbi_image_free(data);
            PixelFormat format = PixelFormat::None;
            if (nrComponents == 1)
                format = PixelFormat::RedFormat;
            else if (nrComponents == 3)
                format = PixelFormat::RGBFormat;
            else if (nrComponents == 4)
                format = PixelFormat::RGBAFormat;

            TexImageInfo::sptr texImage = std::make_shared<TexImageInfo>();
            texImage->imageData = image;
            texImage->width = width;
            texImage->height = height;
            texImage->channels = nrComponents;

            texture = Texture::create(texImage);
            texture->format = format;
//            texture->imageWidth = width;
//            texture->imageHeight = height;
//            texture->channel = nrComponents;
            texture->wrapS = Wrapping::ClampToEdgeWrapping;
            texture->wrapT = Wrapping::ClampToEdgeWrapping;
            texture->wrapR = Wrapping::ClampToEdgeWrapping;

            texture->setNeedsUpdate(true);
        }


        return texture;

    }
};

#endif