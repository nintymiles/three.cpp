//
// Created by SeanR on 2023/12/19.
//

#include "gl_postprocessing.h"

void GLPostprocessing::render(){
//    ApplicationBase::render();

    object->rotation.setX(object->rotation.getX() + 0.005);
    object->rotation.setY(object->rotation.getY() + 0.01);

    composer->render();
}

void GLPostprocessing::showControls(){

}