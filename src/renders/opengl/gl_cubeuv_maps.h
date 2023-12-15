//
// Created by SeanR on 12/15/2023.
//

#ifndef THREE_CPP_GL_CUBEUV_MAPS_H
#define THREE_CPP_GL_CUBEUV_MAPS_H

#include "texture.h"

class GLRenderer;

class GLCubeUVMaps {
private:
    GLRenderer& renderer;
    std::unordered_map<std::string, Texture::wptr> cubeuvmaps;
public:
    using sptr = std::shared_ptr<GLCubeUVMaps>;

    GLCubeUVMaps(GLRenderer& renderer): renderer(renderer){}

    static sptr create(GLRenderer& renderer) {
        return std::make_shared<GLCubeUVMaps>(renderer);
    }

    ~GLCubeUVMaps() = default;

    //parameter is meaningless
    Texture::sptr get(const Texture::sptr texture){
        return Texture::sptr();
    }
};

#endif //THREE_CPP_GL_CUBEUV_MAPS_H
