//
// Created by SeanR on 12/18/2023.
//

#include "gl_shader_demo1.h"

void GLShaderDemo1::render(){
    float delta = timer.getDelta();

    uniforms->set("time", delta);

    ApplicationBase::render();
}

void GLShaderDemo1::showControls(){

}