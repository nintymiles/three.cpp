//
// Created by SeanR on 12/28/2023.
//

#include "gl_postprocessing_after_image.h"

void GLPostProcessingAfterImage::render(){

    mesh->rotation.setX(mesh->rotation.getX() + 0.005);
    mesh->rotation.setY(mesh->rotation.getY() + 0.01);

    if(enabled)
        composer->render();
    else
        ApplicationBase::render();
}

void GLPostProcessingAfterImage::showControls(){
//    ImGui::Begin("控制面板-Controls");
//
//    ImGui::Checkbox("postprocessing",&postprocessing);
//
//    ImGui::End();
}