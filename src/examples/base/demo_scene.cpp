//
// Created by SeanR on 2023/10/25.
//
#include "demo_scene.h"

#include "object_3d.h"
#include "color.h"
#include "box_geometry.h"
#include "math_utils.h"

#include <cmath>
#include <imgui/imgui.h>

namespace demoscene {
    float rotationSpeed = 0.02f;
    int numberOfObjects = 0;
}
using namespace demoscene;

void rotateChildrenCallback(Object3D& e) {
    if (instanceOf<Mesh>(&e)) {
        if (!instanceOf<PlaneGeometry>(e.geometry.get())) {
            e.rotation.setX(e.rotation.getX() + rotationSpeed);
            e.rotation.setY(e.rotation.getY() + rotationSpeed);
            e.rotation.setZ(e.rotation.getZ() + rotationSpeed);
        }
    }
}

void DemoScene::addCube(){
    auto cubeSize = ceil(math::random() * 3);
    auto cubeGeometry = std::make_shared<BoxGeometry>(cubeSize, cubeSize, cubeSize);
    auto cubeMaterial = std::make_shared<MeshPhongMaterial>();
    cubeMaterial->color = Color((unsigned )math::random(0,0xffffff));

    auto cube = std::make_shared<Mesh>(cubeGeometry, cubeMaterial);
    cube->name = "cube-" + std::to_string(scene->children.size());
    cube->position.x = -30 + round(math::random() * planeGeometry->parameters.width);
    cube->position.y = round(math::random() * 5);
    cube->position.z = -20 + round(math::random() * planeGeometry->parameters.height);
    cube->castShadow = true;
    scene->add(cube);
}

void DemoScene::removeCube(){
    if (scene->children.size() == 0) return;
    auto lastObject = scene->children[scene->children.size() - 1];
    if (instanceOf<Mesh>(lastObject.get())) {
        if (!instanceOf<PlaneGeometry>(lastObject->geometry.get())) {
            scene->remove(lastObject);
            numberOfObjects = scene->children.size();

        }
    }
}

void DemoScene::render(){
    numberOfObjects = scene->children.size();
    scene->traverse(&rotateChildrenCallback);
    ApplicationBase::render();
}

void DemoScene::showControls(){
    ImGui::Begin("Controls");
    ImGui::Text("This is Rotaion Speed Control box");
    ImGui::SliderFloat("RotationSpeed", &rotationSpeed, 0.0f, 0.5f);
    if (ImGui::Button("Add Cube")) {
        addCube();
    }
    if (ImGui::Button("Remove Cube")) {
        removeCube();
    }
    ImGui::Text("Number of Objects : %d", numberOfObjects);
    ImGui::End();
}

