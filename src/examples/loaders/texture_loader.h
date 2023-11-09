#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include "texture.h"

#include <stb_image.h>
#include <string>

class TextureLoader {
public:
    TextureLoader(){}
    static Texture::sptr load(const std::string& filePath) {

        Texture::sptr texture = nullptr;

        int width, height, nrComponents;

        stbi_set_flip_vertically_on_load(1);

        unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrComponents, 0);
        std::vector<unsigned char> image(width * height * nrComponents);

        if (data) {
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

            texture = Texture::create(image);
            texture->format = format;
            texture->imageWidth = width;
            texture->imageHeight = height;
            texture->setNeedsUpdate(true);
            texture->channel = nrComponents;
            texture->wrapS = Wrapping::ClampToEdgeWrapping;
            texture->wrapT = Wrapping::ClampToEdgeWrapping;
            texture->wrapR = Wrapping::ClampToEdgeWrapping;
        }


        return texture;

    }
};

#endif