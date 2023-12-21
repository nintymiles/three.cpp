//
// Created by ultraman on 12/21/2023.
//

#include "gl_postprocessing_pixel.h"

void GLPostprocessingPixel::render(){
//    ApplicationBase::render();

    object->rotation.setX(object->rotation.getX() + 0.005);
    object->rotation.setY(object->rotation.getY() + 0.01);

    composer->render();
}

void GLPostprocessingPixel::showControls(){

}