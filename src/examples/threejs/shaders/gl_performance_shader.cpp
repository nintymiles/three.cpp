//
// Created by SeanR on 12/25/2023.
//

#include "gl_performance_shader.h"

void GLPerformanceShader::render(){
#ifdef __APPLE__
    float delta = timer.getDelta()/1000.0; //no vsync for glfw on Mac
#else
    float delta = timer.getDelta();
#endif

//    uniforms->set("time", delta);
//
//    mesh->rotation.setY(0.125 * delta);
//    mesh->rotation.setY( 0.5 * delta);

//    for(auto material:materials){
//        material->setNeedsUpdate(true);
//    }

    delta *= 5;
    uniforms->set("time", 0.2f * delta);

    ApplicationBase::render();
}

void GLPerformanceShader::showControls(){

}