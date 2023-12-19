//
// Created by SeanR on 2023/12/19.
//

#ifndef THREE_CPP_MASK_PASS_H
#define THREE_CPP_MASK_PASS_H

#include "pass.h"

class Scene;
class Camera;

namespace threecpp {

    class MaskPass: public Pass{
        std::shared_ptr<Scene> scene;
        std::shared_ptr<Camera> camera;

        bool clear = true;
        bool needsSwap = false;

        bool inverse = false;

    public:
        MaskPass(std::shared_ptr<Scene> scene,std::shared_ptr<Camera> camera):Pass(),scene(scene),camera(camera){}


        MaskPass& render( std::shared_ptr<GLRenderer> renderer, std::shared_ptr<GLRenderTarget> writeBuffer,
                            std::shared_ptr<GLRenderTarget> readBuffer/*, deltaTime, maskActive */) override;
    };

    class ClearMaskPass: public Pass{
        bool needsSwap = false;

    public:
        ClearMaskPass():Pass(){}


        ClearMaskPass& render( std::shared_ptr<GLRenderer> renderer, std::shared_ptr<GLRenderTarget> writeBuffer,
                          std::shared_ptr<GLRenderTarget> readBuffer/*, deltaTime, maskActive */) override;
    };




}

#endif //THREE_CPP_MASK_PASS_H
