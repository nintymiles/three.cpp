//
// Created by SeanR on 12/12/2023.
//

#include "gl_materials_displacementmap.h"
#include "imgui.h"

void GLMaterialsDisplacementMap::render(){
    material->roughness = setting.roughness,
    material->metalness = setting.metalness,
    material->normalScale = Vector2( 1, - 1 ); // why does the normal map require negation in this case?
    material->aoMapIntensity = setting.aoMapIntensity;
    material->displacementScale = setting.displacementScale,
    material->displacementBias = - 0.428408; // from original model
    material->envMapIntensity = setting.envMapIntensity;

    ambientLight->intensity = setting.ambientIntensity;

    pointLight->position.x = 2500 * math::cos( r );
    pointLight->position.z = 2500 * math::sin( r );

    r += 0.01;

    ApplicationBase::render();
}

void GLMaterialsDisplacementMap::showControls(){
    ImGui::Begin("Controls");

    ImGui::SliderFloat("metalness", &setting.metalness, 0.0f, 1.f);
    ImGui::SliderFloat("roughness", &setting.roughness, 0.0f, 1.f);
    ImGui::SliderFloat("aoMapIntensity", &setting.aoMapIntensity, 0.0f, 1.f);
    ImGui::SliderFloat("ambientIntensity", &setting.ambientIntensity, 0.0f, 1.f);
    ImGui::SliderFloat("envMapIntensity", &setting.envMapIntensity, 0.0f, 1.f);
    ImGui::SliderFloat("displacementScale", &setting.displacementScale, 0.0f, 4.0000f);
    ImGui::SliderFloat("normalScale", &setting.normalScale, 0.0f, 1.f);

    ImGui::End();
}