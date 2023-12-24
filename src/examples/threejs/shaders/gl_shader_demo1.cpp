//
// Created by SeanR on 12/18/2023.
//

#include "gl_shader_demo1.h"

void GLShaderDemo1::render(){
#ifdef __APPLE__
    float delta = timer.getDelta()/1000.0; //no vsync for glfw on Mac
#else
    float delta = timer.getDelta();
#endif

    uniforms->set("time", delta);

    ApplicationBase::render();
}

void GLShaderDemo1::showControls(){

}