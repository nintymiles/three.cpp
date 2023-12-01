//
// Created by SeanR on 12/1/2023.
//

#ifndef THREE_CPP_IMAGE_LOADER_H
#define THREE_CPP_IMAGE_LOADER_H

#include <stb_image.h>
#include <vector>
#include <string>
#include <memory>

#include "texture.h"

class ImageLoader {
public:
    ImageLoader(){}
    static TexImageInfo::sptr load(const std::string& filePath) {

        TexImageInfo::sptr imageInfo;

        int width, height, nrComponents;

        stbi_set_flip_vertically_on_load(1);

        unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrComponents, 0);

        if (data) {
            imageInfo = std::make_shared<TexImageInfo>();
            imageInfo->imageData.resize(width * height * nrComponents);

            std::memcpy(&imageInfo->imageData[0], data, width * height * nrComponents);
            stbi_image_free(data);

            imageInfo->channels = nrComponents;
            imageInfo->width = width;
            imageInfo->height = height;
        }

        return imageInfo;
    }
};

#endif //THREE_CPP_IMAGE_LOADER_H
