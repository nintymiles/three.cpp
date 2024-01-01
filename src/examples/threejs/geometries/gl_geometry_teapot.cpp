//
// Created by SeanR on 2024/1/1.
//

#include "gl_geometry_teapot.h"
#include "imgui.h"

void GLGeometryTeapot::render(){
    effectController.newShading = shadingVec[selShading];
    effectController.newTess = tessVec[selTess];

    if ( effectController.newTess != tess ||
            effectController.bottom != bBottom ||
            effectController.lid != bLid ||
            effectController.body != bBody ||
            effectController.fitLid != bFitLid ||
            effectController.nonblinn != bNonBlinn ||
            effectController.newShading != shading ) {

        tess = effectController.newTess;
        bBottom = effectController.bottom;
        bLid = effectController.lid;
        bBody = effectController.body;
        bFitLid = effectController.fitLid;
        bNonBlinn = effectController.nonblinn;
        shading = effectController.newShading;

        createNewTeapot();

    }

    // skybox is rendered separately, so that it is always behind the teapot.
    if ( shading == "reflective" ) {
        scene->setBackgroundCubeTexture(textureCube);
    } else {
        scene->setBackgroundCubeTexture(nullptr);
    }

    renderer->render( scene, camera );
}

void GLGeometryTeapot::showControls(){

    ImGui::Begin("控制面板");

    if (ImGui::BeginCombo("##newTess", tessVec.size()>0? std::to_string(tessVec[selTess]).c_str() : "")) // The second parameter is the label previewed before opening the combo.
    {
        for (int n = 0; n < tessVec.size(); n++)
        {
            bool is_selected = (selTess == n); // You can store your selection however you want, outside or inside your objects
            if (ImGui::Selectable(std::to_string(tessVec[n]).c_str(), is_selected))
                selTess = n;
            if (is_selected)
                ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
        }
        ImGui::EndCombo();
    }

    ImGui::Checkbox("lid",&effectController.lid);
    ImGui::Checkbox("body",&effectController.body);
    ImGui::Checkbox("bottom",&effectController.bottom);
    ImGui::Checkbox("fitLid",&effectController.fitLid);
    ImGui::Checkbox("nonblinn",&effectController.nonblinn);

    if (ImGui::BeginCombo("##shading", shadingVec.size()>0?shadingVec[selShading].c_str():"")) // The second parameter is the label previewed before opening the combo.
    {
        for (int n = 0; n < shadingVec.size(); n++)
        {
            bool is_selected = (selShading == n); // You can store your selection however you want, outside or inside your objects
            if (ImGui::Selectable(shadingVec[n].c_str(), is_selected))
                selShading = n;
            if (is_selected)
                ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
        }
        ImGui::EndCombo();
    }


    ImGui::End();
}