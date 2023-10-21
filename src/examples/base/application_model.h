//
// Created by SeanR on 2023/10/20.
//

#ifndef THREE_CPP_APPLICATION_MODEL_H
#define THREE_CPP_APPLICATION_MODEL_H

#include "scene.h"
#include "perspective_camera.h"
#include "orthographic_camera.h"
#include "trackball_control.h"

#include "gl_renderer.h"

#include <memory>

class ApplicationBase {
public:
    using sptr = std::shared_ptr<ApplicationBase>;
    bool initialized = false;
    std::shared_ptr<Scene> scene;
    std::shared_ptr<Camera> camera;
    std::shared_ptr<PerspectiveCamera> pcamera;
    std::shared_ptr<OrthographicCamera> ocamera;
    std::shared_ptr<control::TrackballControls> controller;
    std::shared_ptr<GLRenderer> renderer;
    int screenX;
    int screenY;
    bool isPerspective;
    ApplicationBase(int x,int y) :screenX(x),screenY(y) {
        renderer = std::make_shared<GLRenderer>(x, y);
        //renderer->debug.add("checkShaderErrors", true);
        init();
    }

    virtual void init(){}
    virtual void showControls() {}
    virtual void render() {
        renderer->render(scene, camera);
    }
};

#endif //THREE_CPP_APPLICATION_MODEL_H
