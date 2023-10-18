//
// Created by SeanR on 2023/1/13.
//

#ifndef THREE_CPP_SRC_RENDERS_GL_RENDER_STATE_H
#define THREE_CPP_SRC_RENDERS_GL_RENDER_STATE_H

#include "gl_lights.h"
#include "lights.h"
#include "scene.h"

#include <vector>

class GLRenderState
{
public:
    struct RenderState {
        std::vector<Light::sptr> lightsArray;
        std::vector<Light::sptr> shadowsArray;
        GLLights lights;
    };

    RenderState state;

    GLRenderState() {}

    using sptr = std::shared_ptr<GLRenderState>;

    static sptr create() {
        return std::make_shared<GLRenderState>();
    }
    virtual ~GLRenderState() = default;

    void init() {
        state.lightsArray.clear();
        state.shadowsArray.clear();
    }

    void pushLight(const Light::sptr& light) {
        state.lightsArray.push_back(light);
    }
    void pushShadow(const Light::sptr& shadowLight) {
        state.shadowsArray.push_back(shadowLight);
    }
    void setupLights(const Camera::sptr& camera) {
        state.lights.setup(state.lightsArray, camera);
    }
};


class GLRenderStates {
public:
    using sptr = std::shared_ptr<GLRenderStates>;

    std::unordered_map<sole::uuid,std::unordered_map<sole::uuid,GLRenderState::sptr>> renderStates;


    GLRenderStates() {}
    virtual ~GLRenderStates() = default;

    static sptr create() {
        return std::make_shared<GLRenderStates>();
    }
    void onSceneDispose(const Scene& scene) {

        if (renderStates.count(scene.uuid) > 0) {
            renderStates[scene.uuid].clear();
            renderStates.erase(scene.uuid);
        }
    }

    const GLRenderState::sptr& get(const Scene::sptr& scene, const Camera::sptr& camera) {
        GLRenderState::sptr renderState = nullptr;
        if (renderStates.count(scene->uuid)<1) {
            renderState = GLRenderState::create();
            renderStates.insert({ scene->uuid, std::unordered_map<sole::uuid,GLRenderState::sptr>()});
            renderStates[scene->uuid].insert({ camera->uuid,renderState });
            scene->onDisposed.connect(*this, &GLRenderStates::onSceneDispose);
            return renderStates[scene->uuid][camera->uuid];
            //return renderState;
        }
        else {
            std::unordered_map<sole::uuid,GLRenderState::sptr> list = renderStates[scene->uuid];
            GLRenderState::sptr tempState = list[camera->uuid];
            int count = list.count(camera->uuid);
            if (renderStates[scene->uuid].count(camera->uuid) == 0) {
                renderState = GLRenderState::create();
                renderStates[scene->uuid].insert({ camera->uuid,renderState });

            }

            return renderStates[scene->uuid][camera->uuid];
        }
    }


};


#endif //THREE_CPP_SRC_RENDERS_GL_RENDER_STATE_H


//class GLRenderState{
//    std::vector<std::shared_ptr<Light>> lightsArray;
//    std::vector<std::shared_ptr<Light>> shadowsArray;
//    GLLights lights;
//
//    GLRenderState& init(){
//
//    }
//
//    void pushLight( Light& light ) {
//        lightsArray.push_back( std::make_shared<Light>(light) );
//    }
//
//    void pushShadow( Light& shadowLight ) {
//        shadowsArray.push_back( std::make_shared<Light>(shadowLight) );
//    }
//
//    void setupLights( bool physicallyCorrectLights ) {
//        lights.setup( lightsArray, physicallyCorrectLights );
//    }
//
////    function setupLightsView( camera ) {
////
////        lights.setupView( lightsArray, camera );
////
////    }
//
//
//};
//
////function WebGLRenderState( extensions, capabilities ) {
////
////    const lights = new WebGLLights( extensions, capabilities );
////
////    const lightsArray = [];
////    const shadowsArray = [];
////
////    function init() {
////
////        lightsArray.length = 0;
////        shadowsArray.length = 0;
////
////    }
////
////    function pushLight( light ) {
////
////        lightsArray.push( light );
////
////    }
////
////    function pushShadow( shadowLight ) {
////
////        shadowsArray.push( shadowLight );
////
////    }
////
////    function setupLights( physicallyCorrectLights ) {
////
////        lights.setup( lightsArray, physicallyCorrectLights );
////
////    }
////
////    function setupLightsView( camera ) {
////
////        lights.setupView( lightsArray, camera );
////
////    }
////
////    const state = {
////        lightsArray: lightsArray,
////        shadowsArray: shadowsArray,
////
////        lights: lights
////    };
////
////    return {
////        init: init,
////        state: state,
////        setupLights: setupLights,
////        setupLightsView: setupLightsView,
////
////        pushLight: pushLight,
////        pushShadow: pushShadow
////    };
////
////}
////
////function WebGLRenderStates( extensions, capabilities ) {
////
////    let renderStates = new WeakMap();
////
////    function get( scene, renderCallDepth = 0 ) {
////
////        let renderState;
////
////        if ( renderStates.has( scene ) === false ) {
////
////            renderState = new WebGLRenderState( extensions, capabilities );
////            renderStates.set( scene, [ renderState ] );
////
////        } else {
////
////            if ( renderCallDepth >= renderStates.get( scene ).length ) {
////
////                renderState = new WebGLRenderState( extensions, capabilities );
////                renderStates.get( scene ).push( renderState );
////
////            } else {
////
////                renderState = renderStates.get( scene )[ renderCallDepth ];
////
////            }
////
////        }
////
////        return renderState;
////
////    }
////
////    function dispose() {
////
////        renderStates = new WeakMap();
////
////    }
////
////    return {
////        get: get,
////        dispose: dispose
////    };
////
////}
////
////
////export { WebGLRenderStates };