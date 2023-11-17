//
// Created by SeanR on 2023/11/3.
//

#ifndef THREE_CPP_GL_LIGHTS_POINTLIGHTS_H
#define THREE_CPP_GL_LIGHTS_POINTLIGHTS_H

#include "application_model.h"
#include "group.h"
#include "mesh_basic_material.h"
#include "sphere_geometry.h"

#include "obj_loader.h"

#include "timer.h"

#include <thread>
#include <filesystem>

class GLLightsPointLights: public ApplicationBase{
    Group::sptr objGroup;
    MeshBasicMaterial::sptr meshMaterial;

    PointLight::sptr light1,light2,light3,light4;
    Timer timer;

public:
    GLLightsPointLights(int x, int y):ApplicationBase(x,y){}

    virtual void init() override {
        renderer->setClearColor(Color().setHex(0x000000));
        //renderer->shadowMap->enabled = true;
        //renderer->shadowMap->type = ShadowMapType::PCFSoftShadowMap;

        camera = std::make_shared<PerspectiveCamera>(50.0f, screenX/screenY , 1.f, 1000.0f);
        camera->position.setZ(100.f);
        //camera->position.set(0.0f, 0.0f, 250.0f);
        //camera->lookAt(Vector3());
        //isPerspective = true;

        scene = std::make_shared<Scene>();
        scene->setBackgroundColor(Color().set(0x000000));

        SphereGeometry::sptr sphere = SphereGeometry::create( 0.5, 16, 8 );

        //lights setting
        light1 = PointLight::create( 0xff0040, 2, 50 );
        light1->add( Mesh::create( sphere, MeshBasicMaterial::create( Color(0xff0040) ) ) );
        scene->add( light1 );

        light2 = PointLight::create( 0x0040ff, 2, 50 );
        light2->add( Mesh::create(sphere, MeshBasicMaterial::create( Color(0x0040ff) )));
        scene->add( light2 );

        light3 = PointLight::create( 0x80ff80, 2, 50 );
        light3->add( Mesh::create( sphere, MeshBasicMaterial::create(  Color( 0x80ff80 ) ) ) );
        scene->add( light3 );

        light4 = PointLight::create( 0xffaa00, 2, 50 );
        light4->add( Mesh::create( sphere, MeshBasicMaterial::create(  Color(0xffaa00 ) )));
        scene->add( light4 );



        Vector4 screen = Vector4(0, 0, screenX, screenY);
        controller = std::make_shared<control::TrackballControls>(camera, screen);

        controller->staticMoving = false;
        controller->rotateSpeed = 4.0f;
        controller->zoomSpeed = 3;
        controller->panSpeed = 3;
        controller->noZoom = true;
        controller->noPan = true;
        controller->noRotate = false;
        controller->dynamicDampingFactor = 0.3f;


        meshMaterial = MeshBasicMaterial::create();
        meshMaterial->color.set(0x7777ff);
        meshMaterial->name = "Basic Material";
        meshMaterial->flatShading = true;
        meshMaterial->opacity = 0.01f;
        meshMaterial->fog = true;
        meshMaterial->colorWrite = true;

        std::string dir = threecpp::getProjectPath();
        //material的解析还有问题，需要继续改进
        std::thread thread1([&](const std::string& filepath){

                           OBJLoader loader;
                           std::string relativeDir = threecpp::getFileSeparator().append("asset").append(threecpp::getFileSeparator()).append("models")
                                                    .append(threecpp::getFileSeparator()).append("obj").append(threecpp::getFileSeparator())
                                                    .append("walt").append(threecpp::getFileSeparator());
                           objGroup = loader.load(dir + relativeDir + filepath);

                           objGroup->scale.multiplyScalar( 0.8 );
                           //objGroup->position.setZ(-30);
                           objGroup->position.setY(-30);
                           //objGroup->position.setX(50);
                           //auto mesh = Mesh::create(objGroup->geometry,meshMaterial);

                           scene->add(objGroup);
                       }
                ,std::string("WaltHead.obj"));
        thread1.join();

        timer = Timer();
    }

    virtual void render() override;

};


#endif //THREE_CPP_GL_LIGHTS_POINTLIGHTS_H
