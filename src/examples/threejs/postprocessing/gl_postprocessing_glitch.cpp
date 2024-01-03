//
// Created by SeanR on 1/3/2024.
//

#include "gl_postprocessing_glitch.h"
#include "imgui.h"

void GLPostProcessingGlitch::render(){

    glitchPass->goWild = wildGlitch;

    object->rotation.setX( object->rotation.x() + 0.005);
    object->rotation.setY( object->rotation.y() + 0.01);

    composer->render();

}

void GLPostProcessingGlitch::showControls(){
    ImGui::Begin("控制面板-Controls");

    ImGui::Checkbox("wildGlitch",&wildGlitch);

    ImGui::End();
}