//
// Created by ultraman on 12/25/2023.
//

#include "gl_custom_attributes.h"

void GLCustomAttributes::render(){
#ifdef __APPLE__
    float delta = timer.getDelta()/1000.0; //no vsync for glfw on Mac
#else
    float delta = timer.getDelta();
#endif

    uniforms->set("time", delta);

    ApplicationBase::render();
}

void GLCustomAttributes::showControls(){

}