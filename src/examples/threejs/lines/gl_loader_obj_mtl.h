//
// Created by SeanR on 2023/11/3.
//

#ifndef THREE_CPP_GL_LOAD_OBJ_MTL_H
#define THREE_CPP_GL_LOAD_OBJ_MTL_H

#include "application_model.h"
#include "group.h"
#include "mesh_basic_material.h"

#include "obj_loader.h"

#include <thread>
#include <filesystem>

class GLLoaderObjMtl: public ApplicationBase{
    Group::sptr objGroup;
    MeshBasicMaterial::sptr meshMaterial;

public:
    GLLoaderObjMtl(int x, int y):ApplicationBase(x,y){}

    virtual void init() override {
        renderer->setClearColor(Color().setHex(0x000000));
        renderer->shadowMap->enabled = true;
        //renderer->shadowMap->type = ShadowMapType::PCFSoftShadowMap;

        camera = std::make_shared<PerspectiveCamera>(45.0f, screenX/screenY , 1.f, 2000.0f);
        camera->position.set(0.0f, 0.0f, 250.0f);
        //camera->lookAt(Vector3());
        isPerspective = true;

        scene = std::make_shared<Scene>();
        scene->setBackgroundColor(Color().set(0x000000));

        auto ambientLight = AmbientLight::create(Color().setHex(0xcccccc),0.8f);
//        ambientLight->position.set(0, 0, 10);
        scene->add(ambientLight);

        auto pointLight = PointLight::create(Color().setHex(0xffffff),1.0f);
        pointLight->position.set(30, 20, 50);
        pointLight->castShadow = true;
        scene->add(pointLight);

        Vector4 screen = Vector4(0, 0, screenX, screenY);
        controller = std::make_shared<control::TrackballControls>(camera, screen);

        controller->staticMoving = false;
        controller->rotateSpeed = 4.0f;
        controller->zoomSpeed = 3;
        controller->panSpeed = 3;
        controller->noZoom = false;
        controller->noPan = false;
        controller->noRotate = false;
        controller->dynamicDampingFactor = 0.3f;


        meshMaterial = MeshBasicMaterial::create();
        meshMaterial->color.set(0x7777ff);
        meshMaterial->name = "Basic Material";
        meshMaterial->flatShading = true;
        meshMaterial->opacity = 0.01f;
        meshMaterial->fog = true;
        meshMaterial->colorWrite = true;

        std::thread thread1([&](const std::string& filepath){
                           std::string dir = std::filesystem::current_path().parent_path().parent_path().string();
                           OBJLoader loader;
                           std::string relativeDir = threecpp::getFileSeparator().append("asset").append(threecpp::getFileSeparator()).append("models")
                                                    .append(threecpp::getFileSeparator()).append("obj").append(threecpp::getFileSeparator())
                                                    .append("male02").append(threecpp::getFileSeparator());
                           objGroup = loader.load(dir + relativeDir + filepath);

                           //objGroup->scale.set(3, 3, 3);
                           objGroup->position.setZ(-30);
                           objGroup->position.setY(-95);
                           objGroup->position.setX(50);

                           scene->add(objGroup);
                       }
                ,std::string("male02.obj"));
        thread1.join();

        std::thread thread2([&](const std::string& filepath){
                                std::string dir = std::filesystem::current_path().parent_path().parent_path().string();
                                OBJLoader loader;
                                std::string relativeDir = threecpp::getFileSeparator().append("asset").append(threecpp::getFileSeparator()).append("models")
                                        .append(threecpp::getFileSeparator()).append("obj").append(threecpp::getFileSeparator())
                                        .append("female02").append(threecpp::getFileSeparator());
                                objGroup = loader.load(dir + relativeDir + filepath);

                                //objGroup->scale.set(3, 3, 3);
                                objGroup->position.setZ(-30);
                                objGroup->position.setY(-95);
                                objGroup->position.setX(-50);

                                scene->add(objGroup);
                            }
                ,std::string("female02.obj"));
        thread2.join();

    }

    //virtual void render() override;

    void loadObj(const Material::sptr& material,std::string path,Group::sptr& objGrp);

};


#endif //THREE_CPP_GL_LOAD_OBJ_MTL_H
