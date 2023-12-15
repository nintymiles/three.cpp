//
// Created by SeanR on 2023/10/20.
//

#ifndef THREE_CPP_INIT_APPLICATION_H
#define THREE_CPP_INIT_APPLICATION_H

#include "application_model.h"

#include <GLFW/glfw3.h>
#include "trackball_control.h"
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include "demo_scene.h"
#include "demo_scene2.h"

#include "gl_lines_sphere.h"
#include "gl_lines_dashed.h"

#include "gl_loader_obj_mtl.h"
#include "gl_buffer_geometry_attributes_none.h"

#include "gl_lights_pointlights.h"
#include "gl_lights_physical.h"
#include "gl_lights_spotlight.h"

#include "gl_materials_channels.h"
#include "gl_materials_cubemap.h"
#include "gl_materials_demo.h"
#include "gl_materials_displacementmap.h"
#include "gl_materials_cubemap_refraction.h"
#include "gl_materials_cubemap_envmaps.h"

// settings
extern int display_w;
extern int display_h;

ApplicationBase::sptr currentDemoClass;

std::unordered_map<std::string, std::shared_ptr<ApplicationBase>> demoClasses;

GLRenderer::sptr glRenderer;

void initRenderer() {

    /*demoClasses.insert({ "02-First-Scene",std::make_shared<FirstScene>(display_w,display_h) });
    demoClasses.insert({ "03-Material-Light",std::make_shared<MaterialLight>(display_w,display_h) });*/
}


static void ShowApplicationMenuBar() {
    if (ImGui::BeginMainMenuBar()){
        if (ImGui::BeginMenu("File")){

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

            if (ImGui::MenuItem("Demo-Scene", "")) {
                if (demoClasses.count("Demo-Scene") == 0)
                    demoClasses["Demo-Scene"] = std::make_shared<DemoScene>(display_w, display_h);

                currentDemoClass = demoClasses["Demo-Scene"];
                currentDemoClass->renderer->clear();
            }
            if (ImGui::MenuItem("Demo-Scene2", "")) {
                if (demoClasses.count("Demo-Scene2") == 0)
                    demoClasses["Demo-Scene2"] = std::make_shared<DemoScene2>(display_w, display_h);

                currentDemoClass = demoClasses["Demo-Scene2"];
                currentDemoClass->renderer->clear();
            }
            if (ImGui::MenuItem("Lines-Sphere", "")) {
                if (demoClasses.count("Lines-Sphere") == 0)
                    demoClasses["Lines-Sphere"] = std::make_shared<GLLinesSphere>(display_w, display_h);

                currentDemoClass = demoClasses["Lines-Sphere"];
                currentDemoClass->renderer->clear();
            }
            if (ImGui::MenuItem("Lines-Dashed", "")) {
                if (demoClasses.count("Lines-Dashed") == 0)
                    demoClasses["Lines-Dashed"] = std::make_shared<GLLinesDashed>(display_w, display_h);

                currentDemoClass = demoClasses["Lines-Dashed"];
                currentDemoClass->renderer->clear();
            }
            if (ImGui::MenuItem("Obj-Mtl-Loader", "")) {
                if (demoClasses.count("Obj-Mtl-Loader") == 0)
                    demoClasses["Obj-Mtl-Loader"] = std::make_shared<GLLoaderObjMtl>(display_w, display_h);

                currentDemoClass = demoClasses["Obj-Mtl-Loader"];
                currentDemoClass->renderer->clear();
            }
            if (ImGui::MenuItem("Buffer_geometry_attributes_none", "")) {
                if (demoClasses.count("Buffer_geometry_attributes_none") == 0)
                    demoClasses["Buffer_geometry_attributes_none"] = std::make_shared<GLBufferGeometryAttributesNone>(display_w, display_h);

                currentDemoClass = demoClasses["Buffer_geometry_attributes_none"];
                currentDemoClass->renderer->clear();
            }

            if (currentDemoClass != nullptr) currentDemoClass->initialized = false;
            ImGui::EndMenu();

        }

        if (ImGui::BeginMenu("Lights")){

            if (ImGui::MenuItem("Gl_Lights_PointLights", "")) {
                if (demoClasses.count("Gl_Lights_PointLights") == 0)
                    demoClasses["Gl_Lights_PointLights"] = std::make_shared<GLLightsPointLights>(display_w, display_h);

                currentDemoClass = demoClasses["Gl_Lights_PointLights"];
                currentDemoClass->renderer->clear();
            }
            if (ImGui::MenuItem("Gl_Lights_Physical", "")) {
                if (demoClasses.count("Gl_Lights_Physical") == 0)
                    demoClasses["Gl_Lights_Physical"] = std::make_shared<GLLightsPhysical>(display_w, display_h);

                currentDemoClass = demoClasses["Gl_Lights_Physical"];
                currentDemoClass->renderer->clear();
            }
            if (ImGui::MenuItem("Gl_Lights_Spotlight", "")) {
                if (demoClasses.count("Gl_Lights_Spotlight") == 0)
                    demoClasses["Gl_Lights_Spotlight"] = std::make_shared<GLLightsSpotlight>(display_w, display_h);

                currentDemoClass = demoClasses["Gl_Lights_Spotlight"];
                currentDemoClass->renderer->clear();
            }

            if (currentDemoClass != nullptr) currentDemoClass->initialized = false;
            ImGui::EndMenu();

        }
        if (ImGui::BeginMenu("Materials")){

            if (ImGui::MenuItem("Gl_Materials_Channels", "")) {
                if (demoClasses.count("Gl_Materials_Channels") == 0)
                    demoClasses["Gl_Materials_Channels"] = std::make_shared<GLMaterialsChannels>(display_w, display_h);

                currentDemoClass = demoClasses["Gl_Materials_Channels"];
                currentDemoClass->renderer->clear();
            }
            if (ImGui::MenuItem("Gl_Materials_CubeMap", "")) {
                if (demoClasses.count("Gl_Materials_CubeMap") == 0)
                    demoClasses["Gl_Materials_CubeMap"] = std::make_shared<GLMaterialsCubeMap>(display_w, display_h);

                currentDemoClass = demoClasses["Gl_Materials_CubeMap"];
                currentDemoClass->renderer->clear();
            }

            if (ImGui::MenuItem("Gl_Materials", "")) {
                if (demoClasses.count("Gl_Materials") == 0)
                    demoClasses["Gl_Materials"] = std::make_shared<GLMaterialsDemo>(display_w, display_h);

                currentDemoClass = demoClasses["Gl_Materials"];
                currentDemoClass->renderer->clear();
            }

            if (ImGui::MenuItem("Gl_Materials_Displacementmap", "")) {
                if (demoClasses.count("Gl_Materials_Displacementmap") == 0)
                    demoClasses["Gl_Materials_Displacementmap"] = std::make_shared<GLMaterialsDisplacementMap>(display_w, display_h);

                currentDemoClass = demoClasses["Gl_Materials_Displacementmap"];
                currentDemoClass->renderer->clear();
            }

            if (ImGui::MenuItem("Gl_Materials_CubeMap_Refraction", "")) {
                if (demoClasses.count("Gl_Materials_CubeMap_Refraction") == 0)
                    demoClasses["Gl_Materials_CubeMap_Refraction"] = std::make_shared<GLMaterialsCubeMapRefraction>(display_w, display_h);

                currentDemoClass = demoClasses["Gl_Materials_CubeMap_Refraction"];
                currentDemoClass->renderer->clear();
            }

            if (ImGui::MenuItem("Gl_Materials_Envmaps", "")) {
                if (demoClasses.count("Gl_Materials_Envmaps") == 0)
                    demoClasses["Gl_Materials_Envmaps"] = std::make_shared<GLMaterialsCubeMapEnvmaps>(display_w, display_h);

                currentDemoClass = demoClasses["Gl_Materials_Envmaps"];
                currentDemoClass->renderer->clear();
            }

            if (currentDemoClass != nullptr) currentDemoClass->initialized = false;
            ImGui::EndMenu();

        }
        ImGui::EndMainMenuBar();
    }


}

#endif //THREE_CPP_INIT_APPLICATION_H
