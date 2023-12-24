//
// Created by SeanR on 12/18/2023.
//

#include "gl_shader_lava.h"

void GLShaderLava::render(){
#ifdef __APPLE__
    float delta = timer.getDelta()/1000.0; //no vsync for glfw on Mac
#else
    float delta = timer.getDelta();
#endif

    uniforms->set("time", delta);

    mesh->rotation.setY(0.125 * delta);
    mesh->rotation.setY( 0.5 * delta);

    ApplicationBase::render();
}

void GLShaderLava::showControls(){

}