//
// Created by ultraman on 12/22/2023.
//

#include "gl_postprocessing_rgb_halftone.h"

void GLPostProcessingRGBHalfTone::render(){
//    ApplicationBase::render();

    float delta = timer.getDelta() * 0.01;

    group->rotation.setY(group->rotation.getY() + delta * rotationSpeed);

    composer->render(delta);
}

void GLPostProcessingRGBHalfTone::showControls(){

}