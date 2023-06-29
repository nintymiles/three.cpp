//
// Created by SeanR on 2023/1/13.
//

#ifndef THREE_CPP_SRC_RENDERS_GL_RENDER_STATE_H
#define THREE_CPP_SRC_RENDERS_GL_RENDER_STATE_H

#include "gl_lights.h"
#include "light/light_shadow.h"
#include "light/light.h"

#include <vector>

class GLRenderState{
    std::vector<std::shared_ptr<Light>> lightsArray;
    std::vector<std::shared_ptr<Light>> shadowsArray;
    GLLights lights;

    GLRenderState& init(){

    }

    void pushLight( Light& light ) {
        lightsArray.push_back( std::make_shared<Light>(light) );
    }

    void pushShadow( Light& shadowLight ) {
        shadowsArray.push_back( std::make_shared<Light>(shadowLight) );
    }

    void setupLights( bool physicallyCorrectLights ) {
        lights.setup( lightsArray, physicallyCorrectLights );
    }

//    function setupLightsView( camera ) {
//
//        lights.setupView( lightsArray, camera );
//
//    }


};

//function WebGLRenderState( extensions, capabilities ) {
//
//    const lights = new WebGLLights( extensions, capabilities );
//
//    const lightsArray = [];
//    const shadowsArray = [];
//
//    function init() {
//
//        lightsArray.length = 0;
//        shadowsArray.length = 0;
//
//    }
//
//    function pushLight( light ) {
//
//        lightsArray.push( light );
//
//    }
//
//    function pushShadow( shadowLight ) {
//
//        shadowsArray.push( shadowLight );
//
//    }
//
//    function setupLights( physicallyCorrectLights ) {
//
//        lights.setup( lightsArray, physicallyCorrectLights );
//
//    }
//
//    function setupLightsView( camera ) {
//
//        lights.setupView( lightsArray, camera );
//
//    }
//
//    const state = {
//        lightsArray: lightsArray,
//        shadowsArray: shadowsArray,
//
//        lights: lights
//    };
//
//    return {
//        init: init,
//        state: state,
//        setupLights: setupLights,
//        setupLightsView: setupLightsView,
//
//        pushLight: pushLight,
//        pushShadow: pushShadow
//    };
//
//}
//
//function WebGLRenderStates( extensions, capabilities ) {
//
//    let renderStates = new WeakMap();
//
//    function get( scene, renderCallDepth = 0 ) {
//
//        let renderState;
//
//        if ( renderStates.has( scene ) === false ) {
//
//            renderState = new WebGLRenderState( extensions, capabilities );
//            renderStates.set( scene, [ renderState ] );
//
//        } else {
//
//            if ( renderCallDepth >= renderStates.get( scene ).length ) {
//
//                renderState = new WebGLRenderState( extensions, capabilities );
//                renderStates.get( scene ).push( renderState );
//
//            } else {
//
//                renderState = renderStates.get( scene )[ renderCallDepth ];
//
//            }
//
//        }
//
//        return renderState;
//
//    }
//
//    function dispose() {
//
//        renderStates = new WeakMap();
//
//    }
//
//    return {
//        get: get,
//        dispose: dispose
//    };
//
//}
//
//
//export { WebGLRenderStates };


#endif //THREE_CPP_SRC_RENDERS_GL_RENDER_STATE_H
