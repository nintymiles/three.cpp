//
// Created by SeanR on 2023/10/11.
//

#ifndef THREE_CPP_GL_CUBE_MAPS_H
#define THREE_CPP_GL_CUBE_MAPS_H

#include "texture.h"

class GLCubeMaps {
private:
    GLRenderer& renderer;
    std::unordered_map<std::string, Texture::sptr> cubemaps;
public:
    using sptr = std::shared_ptr<GLCubeMaps>;

    GLCubeMaps(GLRenderer& renderer): renderer(renderer){}

    static sptr create(GLRenderer& renderer) {
        return std::make_shared<GLCubeMaps>(renderer);
    }

    ~GLCubeMaps() = default;

    Texture::sptr get(const Texture::sptr texture){
        return Texture::sptr();
    }
};

#endif //THREE_CPP_GL_CUBE_MAPS_H
