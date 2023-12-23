//
// Created by ultraman on 12/21/2023.
//

#include "gl_postprocessing_pixel.h"
#include "imgui.h"

void GLPostprocessingPixel::render(){

    uniforms->set("pixelSize", pixelSize);

    grp->rotation.setY(grp->rotation.getY() + 0.0015f);
    grp->rotation.setZ(grp->rotation.getZ() + 0.001f);

    if(postprocessing)
        composer->render();
    else
        ApplicationBase::render();
}

void GLPostprocessingPixel::showControls(){
    ImGui::Begin("Controls");

    ImGui::SliderFloat("pixelSize", &pixelSize, 2, 32);
    ImGui::Checkbox("postprocessing",&postprocessing);

    ImGui::End();
}