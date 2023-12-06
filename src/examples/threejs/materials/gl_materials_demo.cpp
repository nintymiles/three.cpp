//
// Created by SeanR on 2023/12/6.
//

#include "gl_materials_demo.h"
#include "imgui.h"

void GLMaterialsDemo::render(){

    float time = threecpp::getSystemTimeInMillis()/10000.0;

    camera->position.x = math::cos( time ) * 1000;
    camera->position.z = math::sin( time ) * 1000;

    camera->lookAt( scene->position );

    for ( int i = 0, l = objects.size(); i < l; i ++ ) {
        auto object = objects[ i ];

        object->rotation.setX( object->rotation.getX() + 0.01);
        object->rotation.setY( object->rotation.getY() + 0.005);

    }

    materials[ materials.size() - 2 ]->emissive.setHSL( 0.54, 1, 0.35 * ( 0.5 + 0.5 * math::sin( 35 * time ) ) );
    materials[ materials.size() - 3 ]->emissive.setHSL( 0.04, 1, 0.35 * ( 0.5 + 0.5 * math::cos( 35 * time ) ) );

    pointLight->position.x = math::sin( time * 7 ) * 300;
    pointLight->position.y = math::cos( time * 5 ) * 400;
    pointLight->position.z = math::cos( time * 3 ) * 300;

    ApplicationBase::render();
}

void GLMaterialsDemo::showControls(){

//    ImGui::Begin("控制面板");
//
//    if (ImGui::BeginCombo("##materials", materialVec.size()>0?materialVec[selMaterial].c_str():"")) // The second parameter is the label previewed before opening the combo.
//    {
//        for (int n = 0; n < materialVec.size(); n++)
//        {
//            bool is_selected = (selMaterial == n); // You can store your selection however you want, outside or inside your objects
//            if (ImGui::Selectable(materialVec[n].c_str(), is_selected))
//                selMaterial = n;
//            if (is_selected)
//                ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
//        }
//        ImGui::EndCombo();
//    }
//
//    if (ImGui::BeginCombo("##cameras", cameraVec.size()>0?cameraVec[selCamera].c_str():"")) // The second parameter is the label previewed before opening the combo.
//    {
//        for (int n = 0; n < cameraVec.size(); n++)
//        {
//            bool is_selected = (selCamera == n); // You can store your selection however you want, outside or inside your objects
//            if (ImGui::Selectable(cameraVec[n].c_str(), is_selected))
//                selCamera = n;
//            if (is_selected)
//                ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
//        }
//        ImGui::EndCombo();
//    }
//
//    if (ImGui::BeginCombo("##sides", sideVec.size()>0?sideVec[selSide].c_str():"")) // The second parameter is the label previewed before opening the combo.
//    {
//        for (int n = 0; n < sideVec.size(); n++)
//        {
//            bool is_selected = (selSide == n); // You can store your selection however you want, outside or inside your objects
//            if (ImGui::Selectable(sideVec[n].c_str(), is_selected))
//                selSide = n;
//            if (is_selected)
//                ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
//        }
//        ImGui::EndCombo();
//    }
//
//
//    ImGui::End();
}