//
// Created by SeanR on 12/18/2024.
//

#include "gl_shader_demo2.h"

void GLShaderDemo2::render(){
#ifdef __APPLE__
    float delta = timer.getDelta()/1000.0; //no vsync for glfw on Mac
#else
    float delta = timer.getDelta() / 1000;
#endif

    uniforms1->set("time",uniforms1->get("time").get<float>() + delta * 5);
    uniforms2->set("time",timer.get_elapsed_time());

    for ( auto i = 0; i < scene->children.size(); i ++ ) {
        auto object = scene->children[ i ];

        object->rotation.setY(object->rotation.getY() + delta * 0.5 * ( i % 2 ? 1 : - 1 ));
        object->rotation.setX(object->rotation.getX() + delta * 0.5 * ( i % 2 ? - 1 : 1 ));

    }

    ApplicationBase::render();
}

void GLShaderDemo2::showControls(){

}