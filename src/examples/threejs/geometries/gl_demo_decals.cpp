//
// Created by SeanR on 2024/1/12.
//

#include "gl_demo_decals.h"

void GLDemoDecals::render(){
    Vector2 mousePos = controller->getMousePos();
    std::cout << "mouse Pos.x = " << mousePos.x << "  mouse Pos.y = " << mousePos.y << std::endl;
    checkIntersection(mousePos.x,mousePos.y);

//    if(intersection.intersected)
//        shoot();

    ApplicationBase::render();
}