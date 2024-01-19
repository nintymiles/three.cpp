//
// Created by SeanR on 12/17/2023.
//

#ifndef THREE_CPP_GL_CUBE_RENDER_TARGET_H
#define THREE_CPP_GL_CUBE_RENDER_TARGET_H

#include "gl_render_target.h"


class GLRenderer;

class GLCubeRenderTarget : public GLRenderTarget, public std::enable_shared_from_this<GLCubeRenderTarget>{
public:
    using sptr = std::shared_ptr<GLCubeRenderTarget>;

    GLCubeRenderTarget(int size, GLRenderTargetParameter::sptr options = GLRenderTargetParameter::create());

    static sptr create(int size, GLRenderTargetParameter::sptr options = GLRenderTargetParameter::create()) {
        return std::make_shared<GLCubeRenderTarget>(size, options);
    }

    GLCubeRenderTarget &fromEquirectangularTexture(GLRenderer *renderer, Texture::sptr texture);

    GLCubeRenderTarget& clone(GLCubeRenderTarget* target);

    GLCubeRenderTarget& copy(const GLCubeRenderTarget& source);

    void dispose() {}

};

#endif //THREE_CPP_GL_CUBE_RENDER_TARGET_H
