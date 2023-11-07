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
        scene->setBackgroundColor(Color().set(0xffffff));

        auto ambientLight = AmbientLight::create(Color().setHex(0xcccccc),0.4f);
        scene->add(ambientLight);

        auto spotLight = SpotLight::create(Color().setHex(0xffffff),0.8f);
        spotLight->position.set(-20, 600, -10);
        spotLight->castShadow = true;
        scene->add(spotLight);

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
                                                    .append(threecpp::getFileSeparator()).append("obj").append(threecpp::getFileSeparator());
                                                    //.append("ninja").append(threecpp::getFileSeparator());
                           objGroup = loader.load(dir + relativeDir + filepath);
//                           objGroup->traverse([&](Object3D& o) {
//                               o.material = meshMaterial;
//                               if (instanceOf<Mesh>(&o) && o.materials.size() > 1) {
//                                   int size = o.materials.size();
//                                   for (int i = 0; i < size; i++)
//                                       o.materials.push_back(meshMaterial);
//                               }
//                           });
                           objGroup->scale.set(3, 3, 3);
                           objGroup->position.setZ(-500);
                           objGroup->position.setY(-350);

                           scene->add(objGroup);
                       }
                ,std::string("emerald.obj"));
        thread1.join();

    }

    //virtual void render() override;

    void loadObj(const Material::sptr& material,std::string path,Group::sptr& objGrp);

};


#endif //THREE_CPP_GL_LOAD_OBJ_MTL_H
