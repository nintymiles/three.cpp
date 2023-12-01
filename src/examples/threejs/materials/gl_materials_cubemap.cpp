//
// Created by SeanR on 11/29/2023.
//

#include "gl_materials_cubemap.h"
#include "imgui.h"

void GLMaterialsCubeMap::render(){
    ApplicationBase::render();
}

void GLMaterialsCubeMap::showControls(){
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