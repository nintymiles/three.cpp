//
// Created by SeanR on 2023/12/19.
//

#include "gl_blackhole.h"

void GLBlackHole::render(){
//    ApplicationBase::render();

    object->rotation.setX(object->rotation.getX() + 0.005);
    object->rotation.setY(object->rotation.getY() + 0.01);

    composer->render();
}

void GLBlackHole::showControls(){

}