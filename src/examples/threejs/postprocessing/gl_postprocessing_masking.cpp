//
// Created by SeanR on 2023/12/30.
//

#include "gl_postprocessing_masking.h"
#include "imgui.h"

void GLPostProcessingMasking::render(){
    float time = threecpp::getSystemTimeInMillis();

    box->position.x = math::cos( time / 1.5 ) * 2;
    box->position.y = math::sin( time ) * 2;
    box->rotation.setX( time );
    box->rotation.setY( time / 2 );

    torus->position.x = math::cos( time ) * 2;
    torus->position.y = math::sin( time / 1.5 ) * 2;
    torus->rotation.setX( time );;
    torus->rotation.setY( time / 2 );

    renderer->clear();
    composer->render( time );
}

void GLPostProcessingMasking::showControls(){
    ImGui::Begin("控制面板-Controls");

//    ImGui::Text(" %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
//
//    ImGui::Checkbox("postprocessing",&enabled);
//    ImGui::SliderFloat("dampling", &damp, 0, 1);

    ImGui::End();
}