//
// Created by SeanR on 2023/10/11.
//

#ifndef THREE_CPP_GL_CUBEMAP_H
#define THREE_CPP_GL_CUBEMAP_H

#include "texture.h"

class GLCubeMap {
private:
    GLRenderer& renderer;
    std::unordered_map<std::string, Texture::sptr> cubemaps;
public:
    using sptr = std::shared_ptr<GLCubeMap>;

    GLCubeMap(GLRenderer& renderer): renderer(renderer){}

    static sptr create(GLRenderer& renderer) {
        return std::make_shared<GLCubeMap>(renderer);
    }

    ~GLCubeMap() = default;

    //parameter is meaningless
    Texture::sptr get(const Texture::sptr texture){
        return Texture::sptr();
    }
};

#endif //THREE_CPP_GL_CUBEMAP_H
