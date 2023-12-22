//
// Created by ultraman on 12/22/2023.
//

#include "gl_postprocessing_rgb_halftone.h"

#include "imgui.h"

void GLPostProcessingRGBHalfTone::render(){
//    ApplicationBase::render();
    uniforms->set("radius", setting.radius);
    uniforms->set("rotateR", setting.rotateR);
    uniforms->set("rotateG", setting.rotateG);
    uniforms->set("rotateB", setting.rotateB);
    uniforms->set("scatter", setting.scatter);
    uniforms->set("greyscale", setting.greyscale);
    uniforms->set("blending", setting.blending);
    uniforms->set("disable", setting.disable);
    uniforms->set("shape", setting.shape);
    uniforms->set("blendingMode", setting.blendingMode);


    float delta = timer.getDelta() * 0.01;

    group->rotation.setY(group->rotation.getY() + delta * rotationSpeed);

    composer->render(delta);
}

void GLPostProcessingRGBHalfTone::showControls(){
    ImGui::Begin("Controls");

    if (ImGui::BeginCombo("##shapes", shapeVec.size()>0?shapeVec[setting.shape-1].c_str():"")) // The second parameter is the label previewed before opening the combo.
    {
        for (int n = 0; n < shapeVec.size(); n++)
        {
            bool is_selected = ((setting.shape-1) == n); // You can store your selection however you want, outside or inside your objects
            if (ImGui::Selectable(shapeVec[n].c_str(), is_selected))
                setting.shape = n + 1;
            if (is_selected)
                ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
        }
        ImGui::EndCombo();
    }
    ImGui::SliderFloat("radius", &setting.radius, 1.0f, 25.f);
    ImGui::SliderFloat("rotateR", &setting.rotateR, 0.0f, 90.f);
    ImGui::SliderFloat("rotateG", &setting.rotateG, 0.0f, 90.f);
    ImGui::SliderFloat("rotateB", &setting.rotateB, 0.0f, 90.f);
    ImGui::SliderFloat("scatter", &setting.scatter, 0.0f, 1.f);
    ImGui::Checkbox("greyscale",&setting.greyscale);
    ImGui::SliderFloat("blending", &setting.blending, 0.0f, 1.f);
    if (ImGui::BeginCombo("##blendingMode", blendModeVec.size()>0?blendModeVec[setting.blendingMode-1].c_str():"")) // The second parameter is the label previewed before opening the combo.
    {
        for (int n = 0; n < blendModeVec.size(); n++)
        {
            bool is_selected = ((setting.blendingMode-1) == n); // You can store your selection however you want, outside or inside your objects
            if (ImGui::Selectable(blendModeVec[n].c_str(), is_selected))
                setting.blendingMode = n + 1;
            if (is_selected)
                ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
        }
        ImGui::EndCombo();
    }
    ImGui::Checkbox("disable",&setting.disable);


    //    gui.add( controller, 'shape', { 'Dot': 1, 'Ellipse': 2, 'Line': 3, 'Square': 4 } ).onChange( onGUIChange );
//    gui.add( controller, 'radius', 1, 25 ).onChange( onGUIChange );
//    gui.add( controller, 'rotateR', 0, 90 ).onChange( onGUIChange );
//    gui.add( controller, 'rotateG', 0, 90 ).onChange( onGUIChange );
//    gui.add( controller, 'rotateB', 0, 90 ).onChange( onGUIChange );
//    gui.add( controller, 'scatter', 0, 1, 0.01 ).onChange( onGUIChange );
//    gui.add( controller, 'greyscale' ).onChange( onGUIChange );
//    gui.add( controller, 'blending', 0, 1, 0.01 ).onChange( onGUIChange );
//    gui.add( controller, 'blendingMode', { 'Linear': 1, 'Multiply': 2, 'Add': 3, 'Lighter': 4, 'Darker': 5 } ).onChange( onGUIChange );
//    gui.add( controller, 'disable' ).onChange( onGUIChange );

    ImGui::End();
}