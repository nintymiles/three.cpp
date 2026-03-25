//
// Created by SeanR on 03/25/2026.
//

#include "gl_materials_texture_rotation.h"
#include "imgui.h"

void GLMaterialsTextureRotation::render(){
    updateUvTransform();
    ApplicationBase::render();
}

void GLMaterialsTextureRotation::showControls(){

    ImGui::Begin("控制面板");

    ImGui::SliderFloat("offset.x", &API.offsetX, 0.0f, 1.0f);
    ImGui::SliderFloat("offset.y", &API.offsetY, 0.0f, 1.0f);
    ImGui::SliderFloat("repeat.x", &API.repeatX, 0.0f, 2.0f);
    ImGui::SliderFloat("repeat.y", &API.repeatY, 0.0f, 2.0f);
    ImGui::SliderFloat("rotation", &API.rotation, 0.0f, 2.0f);
    ImGui::SliderFloat("center.x", &API.centerX, 0.0f, 1.0f);
    ImGui::SliderFloat("center.y", &API.centerY, 0.0f, 1.0f);

    ImGui::End();
}