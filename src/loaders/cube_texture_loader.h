//
// Created by SeanR on 12/1/2023.
//

#ifndef THREE_CPP_CUBE_TEXTURE_LOADER_H
#define THREE_CPP_CUBE_TEXTURE_LOADER_H

#include "texture_loader.h"
#include "cube_texture.h"

class CubeTextureLoader{
public:
    CubeTextureLoader(){}
    static CubeTexture::sptr load(const std::vector<std::string>& filePaths,bool isFlipY = false) {
        std::vector<Texture::sptr> images;
        for(std::string filePath:filePaths) {
            Texture::sptr texture = TextureLoader::load(filePath,isFlipY);
            if (texture) {
                images.push_back(texture);
            }
        }

        CubeTexture::sptr cubeTex = CubeTexture::create(images);

        if(images.size()>=6)
            cubeTex->setNeedsUpdate(true);


        return cubeTex;
    }
};

#endif //THREE_CPP_CUBE_TEXTURE_LOADER_H
