//
// Created by SeanR on 2023/12/18.
//

#ifndef THREE_CPP_RENDER_PASS_H
#define THREE_CPP_RENDER_PASS_H

#include "pass.h"
#include "scene.h"
#include "camera.h"
#include "material.h"
#include "color.h"

class GLRenderer;
class GLRenderTarget;
namespace threecpp {


class RenderPass: public Pass {
    Scene::sptr scene;
    Camera::sptr camera;
    Material::sptr overrideMaterial;
    Color clearColor;
    bool clearAlpha;
    Color _oldClearColor;
    bool clearDepth;

public:
    RenderPass(Scene::sptr scene,Camera::sptr camera,Material::sptr overrideMaterial
               ,Color clearColor,bool clearAlpha):Pass(),scene(scene),camera(camera),
               overrideMaterial(overrideMaterial),clearColor(clearColor),clearAlpha(clearAlpha),clearDepth{clearDepth}


    void render( std::shared_ptr<GLRenderer> renderer, std::shared_ptr<GLRenderTarget> writeBuffer,
            std::shared_ptr<GLRenderTarget> readBuffer/*, deltaTime, maskActive */);
};

}


#endif //THREE_CPP_RENDER_PASS_H
