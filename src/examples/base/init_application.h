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

// settings
extern int display_w;
extern int display_h;

ApplicationBase::sptr currentDemoClass;

std::unordered_map<std::string, std::shared_ptr<ApplicationBase>> demoClasses;

GLRenderer::sptr glRenderer;

void initRenderer() {

    /*demoClasses.insert({ "02-First-Scene",std::make_shared<FirstScene>(display_w,display_h) });
    demoClasses.insert({ "03-Material-Light",std::make_shared<MaterialLight>(display_w,display_h) });
    demoClasses.insert({ "04-Material-Light-Animation",std::make_shared<MaterialLightAnimation>(display_w,display_h) });
    demoClasses.insert({ "01-Basic-Scene",std::make_shared<BasicScene>(display_w,display_h) });
    demoClasses.insert({ "02-Foggy-Scene",std::make_shared<FoggyScene>(display_w,display_h) });
    demoClasses.insert({ "03-Forced-Materials",std::make_shared<ForcedMaterials>(display_w,display_h) });
    demoClasses.insert({ "04-Geometries",std::make_shared<Geometries>(display_w,display_h) });
    demoClasses.insert({ "05-Custom-Geometry",std::make_shared<CustomGeometry>(display_w,display_h) });
    demoClasses.insert({ "06-Custom-Geometry",std::make_shared<CustomGeometry>(display_w,display_h) });*/
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

            if (currentDemoClass != nullptr) currentDemoClass->initialized = false;
            ImGui::EndMenu();

        }
//        if (ImGui::BeginMenu("Chapter2")) {
//            if (ImGui::MenuItem("01-Basic-Scene", "")) {
//                if (demoClasses.count("01-Basic-Scene") == 0)
//                    demoClasses["01-Basic-Scene"] = std::make_shared<BasicScene>(display_w, display_h);
//
//                currentDemoClass = demoClasses["01-Basic-Scene"];
//                currentDemoClass->renderer->clear();
//            }
//            if (ImGui::MenuItem("02-Foggy-Scene", "")) {
//                if (demoClasses.count("02-Foggy-Scene") == 0)
//                    demoClasses["02-Foggy-Scene"] = std::make_shared<FoggyScene>(display_w, display_h);
//
//                currentDemoClass = demoClasses["02-Foggy-Scene"];
//                currentDemoClass->renderer->clear();
//            }
//            if (ImGui::MenuItem("03-Forced-Materials", "")) {
//                if (demoClasses.count("03-Forced-Materials") == 0)
//                    demoClasses["03-Forced-Materials"] = std::make_shared<ForcedMaterials>(display_w, display_h);
//
//                currentDemoClass = demoClasses["03-Forced-Materials"];
//                currentDemoClass->renderer->clear();
//            }
//            if (ImGui::MenuItem("04-Geometries", "")) {
//                if (demoClasses.count("04-Geometries") == 0)
//                    demoClasses["04-Geometries"] = std::make_shared<Geometries>(display_w, display_h);
//
//                currentDemoClass = demoClasses["04-Geometries"];
//                currentDemoClass->renderer->clear();
//            }
//            if (ImGui::MenuItem("05-Custom-Geometry", "")) {
//                if (demoClasses.count("05-Custom-Geometry") == 0)
//                    demoClasses["05-Custom-Geometry"] = std::make_shared<CustomGeometry>(display_w, display_h);
//
//                currentDemoClass = demoClasses["05-Custom-Geometry"];
//
//                currentDemoClass->renderer->clear();
//            }
//            if (ImGui::MenuItem("06-Mesh-Properties", "")) {
//                if (demoClasses.count("06-Mesh-Properties") == 0)
//                    demoClasses["06-Mesh-Properties"] = std::make_shared<MeshProperties>(display_w, display_h);
//
//                currentDemoClass = demoClasses["06-Mesh-Properties"];
//
//                currentDemoClass->renderer->clear();
//            }
//            if (ImGui::MenuItem("07-Both-Camera", "")) {
//                if (demoClasses.count("07-Both-Camera") == 0)
//                    demoClasses["07-Both-Camera"] = std::make_shared<BothCameraExample>(display_w, display_h);
//
//                currentDemoClass = demoClasses["07-Both-Camera"];
//
//                currentDemoClass->renderer->clear();
//            }
//            if (ImGui::MenuItem("08-Cameras-lookat", "")) {
//                if (demoClasses.count("08-Cameras-lookat") == 0)
//                    demoClasses["08-Cameras-lookat"] = std::make_shared<CamerasLookAt>(display_w, display_h);
//
//                currentDemoClass = demoClasses["08-Cameras-lookat"];
//
//                currentDemoClass->renderer->clear();
//            }
//            if (currentDemoClass != nullptr) currentDemoClass->initialized = false;
//            ImGui::EndMenu();
//        }
//        if (ImGui::BeginMenu("Chapter3")) {
//            if (ImGui::MenuItem("01-Ambient-Light")) {
//                if (demoClasses.count("01-Ambient-Light") == 0)
//                    demoClasses["01-Ambient-Light"] = std::make_shared<AmbientLightExample>(display_w, display_h);
//
//                currentDemoClass = demoClasses["01-Ambient-Light"];
//                currentDemoClass->renderer->clear();
//            }
//            if (ImGui::MenuItem("02-Spot-Light")) {
//                if (demoClasses.count("02-Spot-Light") == 0)
//                    demoClasses["02-Spot-Light"] = std::make_shared<SpotLightExample>(display_w, display_h);
//
//                currentDemoClass = demoClasses["02-Spot-Light"];
//                currentDemoClass->renderer->clear();
//            }
//            if (ImGui::MenuItem("03-Point-Light")) {
//                if (demoClasses.count("03-Point-Light") == 0)
//                    demoClasses["03-Point-Light"] = std::make_shared<PointLightExample>(display_w, display_h);
//
//                currentDemoClass = demoClasses["03-Point-Light"];
//                currentDemoClass->renderer->clear();
//            }
//
//            if (ImGui::MenuItem("04-Directional-Light")) {
//                if (demoClasses.count("04-Directional-Light") == 0)
//                    demoClasses["04-Directional-Light"] = std::make_shared<DirectionalLightExample>(display_w, display_h);
//
//                currentDemoClass = demoClasses["04-Directional-Light"];
//                currentDemoClass->renderer->clear();
//            }
//            if (ImGui::MenuItem("05-Hemisphere-Light")) {
//                if (demoClasses.count("05-Hemisphere-Light") == 0)
//                    demoClasses["05-Hemisphere-Light"] = std::make_shared<HemisphereLightExample>(display_w, display_h);
//
//                currentDemoClass = demoClasses["05-Hemisphere-Light"];
//                currentDemoClass->renderer->clear();
//            }
//
//            if (ImGui::MenuItem("06-Area-Light")) {
//                if (demoClasses.count("06-Area-Light") == 0)
//                    demoClasses["06-Area-Light"] = std::make_shared<AreaLightExample>(display_w, display_h);
//
//                currentDemoClass = demoClasses["06-Area-Light"];
//                currentDemoClass->renderer->clear();
//            }
//
//            if (ImGui::MenuItem("07-Lensflares")) {
//                if (demoClasses.count("07-Lensflares") == 0)
//                    demoClasses["07-Lensflares"] = std::make_shared<LensflaresExample>(display_w, display_h);
//
//                currentDemoClass = demoClasses["07-Lensflares"];
//                currentDemoClass->renderer->clear();
//            }
//
//            ImGui::EndMenu();
//        }
//        if (ImGui::BeginMenu("Chapter4")) {
//            if (ImGui::MenuItem("01-Basic-MeshMaterial")) {
//                if (demoClasses.count("01-Basic-MeshMaterial") == 0)
//                    demoClasses["01-Basic-MeshMaterial"] = std::make_shared<BasicMeshMaterialExample>(display_w, display_h);
//
//                currentDemoClass = demoClasses["01-Basic-MeshMaterial"];
//                currentDemoClass->renderer->clear();
//            }
//            ImGui::EndMenu();
//        }
        ImGui::EndMainMenuBar();
    }


}

#endif //THREE_CPP_INIT_APPLICATION_H
