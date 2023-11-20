//
// Created by seanren on 11/15/2023.
//
#include "gl_lights_physical.h"

#include "imgui.h"

void GLLightsPhysical::render(){
    renderer->toneMappingExposure = math::pow( exposure, 5.0 ); // to allow for very bright scenes.
    renderer->shadowMap->enabled = shadows;
    bulbLight->castShadow = shadows;
    ballMesh->receiveShadow = shadows;

    if ( shadows != previousShadowMap ) {
        ballMat->needsUpdate = true;
        cubeMat->needsUpdate = true;
        floorMat->needsUpdate = true;
        previousShadowMap = shadows;
    }

    bulbLight->setPower(bulbLuminousPowers[bulbPowerSelIdx].second);
    bulbMat->emissiveIntensity = bulbLight->intensity / math::pow( 0.02, 2.0 ); // convert from intensity to irradiance at bulb surface

    hemiLight->intensity = hemiLuminousIrradiances[hemiIrradianceSelIdx].second;
    float time = threecpp::getSystemTimeInMillis() * 0.0005;

    bulbLight->position.y = math::cos( time ) * 0.75 + 1.25;
    ballMesh->rotation.setY(time);

    ApplicationBase::render();
}

void GLLightsPhysical::showControls(){
    ImGui::Begin("Controls");

    if (ImGui::BeginCombo("##hemiIrridance", hemiLuminousIrradiances.size()>0?hemiLuminousIrradiances[hemiIrradianceSelIdx].first.c_str():"")) // The second parameter is the label previewed before opening the combo.
    {
        for (int n = 0; n < hemiLuminousIrradiances.size(); n++)
        {
            bool is_selected = (hemiIrradianceSelIdx == n); // You can store your selection however you want, outside or inside your objects
            if (ImGui::Selectable(hemiLuminousIrradiances[n].first.c_str(), is_selected))
                hemiIrradianceSelIdx = n;
            if (is_selected)
                ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
        }
        ImGui::EndCombo();
    }


    if (ImGui::BeginCombo("##bulbPower", bulbLuminousPowers.size()>0?bulbLuminousPowers[bulbPowerSelIdx].first.c_str():"")) // The second parameter is the label previewed before opening the combo.
    {
        for (int n = 0; n < bulbLuminousPowers.size(); n++)
        {
            bool is_selected = (bulbPowerSelIdx == n); // You can store your selection however you want, outside or inside your objects
            if (ImGui::Selectable(bulbLuminousPowers[n].first.c_str(), is_selected))
                bulbPowerSelIdx = n;
            if (is_selected)
                ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
        }
        ImGui::EndCombo();
    }

    ImGui::SliderFloat("Exposure", &exposure, 0.0f, 1.f);
    ImGui::Checkbox("Shadows",&shadows);
    ImGui::End();
}