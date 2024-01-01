//
// Created by SeanR on 2023/12/31.
//

#ifndef THREE_CPP_CLEAR_PASS_H
#define THREE_CPP_CLEAR_PASS_H

#include "pass.h"

#include "color.h"

class Material;
namespace threecpp {

class ClearPass : public Pass {
    Color _oldClearColor;
    Color clearColor;
    float clearAlpha;



public:

    ClearPass(Color clearColor = Color{},float clearAlpha = .0f);

    ClearPass& render( std::shared_ptr<GLRenderer> renderer, std::shared_ptr<GLRenderTarget> writeBuffer,
                         std::shared_ptr<GLRenderTarget> readBuffer ) override;

    //void dispose();
};

}

#endif //THREE_CPP_CLEAR_PASS_H
