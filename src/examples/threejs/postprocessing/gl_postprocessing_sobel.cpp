//
// Created by SeanR on 12/23/2023.
//

#include "gl_postprocessing_sobel.h"
#include "imgui.h"

void GLPostProcessingSobel::render(){

    if(postprocessing)
        composer->render();
    else
        ApplicationBase::render();
}

void GLPostProcessingSobel::showControls(){
    ImGui::Begin("Controls");

    ImGui::Checkbox("postprocessing",&postprocessing);

    ImGui::End();
}