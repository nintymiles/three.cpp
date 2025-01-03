//
// Created by SeanR on 2023/10/20.
//

#ifndef THREE_CPP_INIT_APPLICATION_H
#define THREE_CPP_INIT_APPLICATION_H

#include "application_model.h"

#include "trackball_control.h"
#include "view_objects_stage.h"

#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

// settings
extern int display_w;
extern int display_h;

ApplicationBase::sptr renderClass;

std::unordered_map<std::string, std::shared_ptr<ApplicationBase>> renderClasses;

GLRenderer::sptr glRenderer;

void initRenderer() {
    renderClass = std::make_shared<ViewObjectsStage>(display_w, display_h);
    /*renderClasses.insert({ "02-First-Scene",std::make_shared<FirstScene>(display_w,display_h) });
    renderClasses.insert({ "03-Material-Light",std::make_shared<MaterialLight>(display_w,display_h) });*/
}


static void ShowApplicationMenuBar() {
    if (ImGui::BeginMainMenuBar()){
        if (ImGui::BeginMenu("File")){

            if (ImGui::MenuItem("Open..", "Ctrl+O")) {
                IGFD::FileDialogConfig config;
                config.path = ".";
                config.sidePaneWidth = 256.f;
                ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".*", config);
            }

            if (ImGui::MenuItem("Exit", "CTRL+E")) {
                exit(0);
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Application average Frame Rate")) {}
            ImGui::BeginChild("child", ImVec2(0, 30), true);
            ImGui::Text(" %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::EndChild();
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Basic")){

//            if (ImGui::MenuItem("Demo-Scene", "")) {
//                if (demoClasses.count("Demo-Scene") == 0)
//                    demoClasses["Demo-Scene"] = std::make_shared<DemoScene>(display_w, display_h);
//
//                currentDemoClass = demoClasses["Demo-Scene"];
//                currentDemoClass->renderer->clear();
//            }
//            if (ImGui::MenuItem("Demo-Scene2", "")) {
//                if (demoClasses.count("Demo-Scene2") == 0)
//                    demoClasses["Demo-Scene2"] = std::make_shared<DemoScene2>(display_w, display_h);
//
//                currentDemoClass = demoClasses["Demo-Scene2"];
//                currentDemoClass->renderer->clear();
//            }

            if (renderClass != nullptr) renderClass->initialized = false;
            ImGui::EndMenu();

        }


        ImGui::EndMainMenuBar();
    }


}

#endif //THREE_CPP_INIT_APPLICATION_H
