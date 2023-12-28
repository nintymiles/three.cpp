//
// Created by SeanR on 12/28/2023.
//

#include "gl_postprocessing_after_image.h"

#include "imgui.h"

void GLPostProcessingAfterImage::render(){

    mesh->rotation.setX(mesh->rotation.getX() + 0.005);
    mesh->rotation.setY(mesh->rotation.getY() + 0.01);

    uniforms->set("damp", damp);

    if(enabled)
        composer->render();
    else
        ApplicationBase::render();
}

void GLPostProcessingAfterImage::showControls(){
    ImGui::Begin("控制面板-Controls");

    ImGui::Text(" %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::Checkbox("postprocessing",&enabled);
    ImGui::SliderFloat("dampling", &damp, 0, 1);

    ImGui::End();
}