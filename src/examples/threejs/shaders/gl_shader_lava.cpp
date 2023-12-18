//
// Created by SeanR on 12/18/2023.
//

#include "gl_shader_lava.h"

void GLShaderLava::render(){
    float delta = timer.getDelta();

    uniforms->set("time", delta);

    mesh->rotation.setY(0.125 * delta);
    mesh->rotation.setY( 0.5 * delta);

    ApplicationBase::render();
}

void GLShaderLava::showControls(){

}