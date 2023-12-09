//
// Created by SeanR on 11/29/2023.
//

#include "gl_materials_channels.h"
#include "imgui.h"

void GLMaterialsChannels::render(){
    if(headMesh){
        auto &material = headMesh->material;

        switch (selMaterial) {
            case 0: material = materialStandard; break;
            case 1: material = materialNormal; break;
            case 2: material = materialDepth; break;
            case 3: material = materialDepthRGBA; break;

        }

        switch ( selSide ) {
            case 0: material->side = Side::FrontSide; break;
            case 1: material->side = Side::BackSide; break;
            case 2: material->side = Side::DoubleSide; break;
        }

        //material->needsUpdate = true;
        material->uniformsNeedUpdate = true;
        material->setNeedsUpdate(true);
    }

    switch ( selCamera ) {
        case 0:
            camera = perspectiveCamera;
            controller = pCameraControl;
            break;
        case 1:
            camera = orthoCamera;
            controller = oCameraControl;
            break;
    }

    pCameraControl->update();
    oCameraControl->update(); // must update both controls for damping to complete

    ApplicationBase::render();
}

void GLMaterialsChannels::showControls(){
//    std::string rootDir = threecpp::getProjectPath();
//    std::string fileSeparator = threecpp::getFileSeparator();
//    std::string resourceDir = rootDir.append(fileSeparator).append("asset").append(fileSeparator)
//            .append("fonts").append(fileSeparator);
//    ImGuiIO io = ImGui::GetIO();
//    ImFont *cFont = io.Fonts->AddFontFromFileTTF((resourceDir+"LXGWWenKaiMono-Regular.ttf").c_str(), 26.0f, NULL, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
//    ImGui::PushFont(cFont);
    ImGui::Begin("控制面板");

    if (ImGui::BeginCombo("##materials", materialVec.size()>0?materialVec[selMaterial].c_str():"")) // The second parameter is the label previewed before opening the combo.
    {
        for (int n = 0; n < materialVec.size(); n++)
        {
            bool is_selected = (selMaterial == n); // You can store your selection however you want, outside or inside your objects
            if (ImGui::Selectable(materialVec[n].c_str(), is_selected))
                selMaterial = n;
            if (is_selected)
                ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
        }
        ImGui::EndCombo();
    }

    if (ImGui::BeginCombo("##cameras", cameraVec.size()>0?cameraVec[selCamera].c_str():"")) // The second parameter is the label previewed before opening the combo.
    {
        for (int n = 0; n < cameraVec.size(); n++)
        {
            bool is_selected = (selCamera == n); // You can store your selection however you want, outside or inside your objects
            if (ImGui::Selectable(cameraVec[n].c_str(), is_selected))
                selCamera = n;
            if (is_selected)
                ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
        }
        ImGui::EndCombo();
    }

    if (ImGui::BeginCombo("##sides", sideVec.size()>0?sideVec[selSide].c_str():"")) // The second parameter is the label previewed before opening the combo.
    {
        for (int n = 0; n < sideVec.size(); n++)
        {
            bool is_selected = (selSide == n); // You can store your selection however you want, outside or inside your objects
            if (ImGui::Selectable(sideVec[n].c_str(), is_selected))
                selSide = n;
            if (is_selected)
                ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
        }
        ImGui::EndCombo();
    }


    ImGui::End();
}