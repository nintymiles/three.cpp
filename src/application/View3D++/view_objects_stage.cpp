//
// Created by SeanR on 03/06/2024.
//
#include "view_objects_stage.h"

#include "object_3d.h"
#include "color.h"
#include "box_geometry.h"
#include "math_utils.h"

#include <cmath>
#include <imgui/imgui.h>

namespace ViewObjectsStage__ {
    float rotationSpeed = 0.02f;
    int numberOfObjects = 0;
}
using namespace ViewObjectsStage__;

void ViewObjectsStage::loadObj(std::string fileName){
    if(objGroup)
        scene->remove(objGroup);

    OBJLoader loader;

    objGroup = loader.load(fileName);


    objGroup->traverse([&](Object3D& o) {
        o.material = meshMaterial;
        if (instanceOf<Mesh>(&o) && o.materials.size() > 1) {
            int size = o.materials.size();
            for (int i = 0; i < size; i++)
                o.materials.push_back(meshMaterial);
        }
    });
    objGroup->scale.set(4, 4, 4);

    scene->add(objGroup);
}

void ViewObjectsStage::render(){
//    Vector2 mousePos = controller->getMousePos();
//    camera->position.x += ( mousePos.x - camera->position.x ) * .05;
//    camera->position.y += ( mousePos.y - camera->position.y ) * .05;
//
//    //std::cout << "mouse.x:"<< mousePos.x << "|| mouse.y:" << mousePos.y << std::endl;
//    camera->lookAt( scene->position );

    ApplicationBase::render();
}