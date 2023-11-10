//
// Created by SeanR on 2023/11/6.
//

#include "gl_buffer_geometry_attributes_none.h"

#include "common_utils.h"

#include "imgui.h"

void GLBufferGeometryAttributesNone::render(){
    auto time = threecpp::getSystemTimeInMillis();

    for (int i = 0; i < scene->children.size(); i++ ) {
        auto object = scene->children[i];
        if ( object->isMesh ) {
            object->rotation.setX(time / 1000.0 * 0.25);
            object->rotation.setY(time / 1000.0 * 0.50);
        }
    }

    ApplicationBase::render();
}

void GLBufferGeometryAttributesNone::showControls(){
    ImGui::SetWindowPos(ImVec2( 0, 0));
    bool is_open;
    ImGui::Begin("Info",&is_open, ImGuiWindowFlags_NoTitleBar);
    ImGui::Text("Open GLES 3.0 buffer geometry - attributes - none");
    ImGui::End();
}