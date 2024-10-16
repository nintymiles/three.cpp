//
// Created by SeanR on 11/29/2023.
//

#ifndef THREE_CPP_GL_MATERIALS_CUBEMAP_H
#define THREE_CPP_GL_MATERIALS_CUBEMAP_H

#include "application_model.h"
#include "orbit_control.h"
#include "cube_texture_loader.h"
#include "obj_loader.h"

#include "ambient_light.h"
#include "point_light.h"

#include "mesh_standard_material.h"
#include "mesh_depth_material.h"
#include "mesh_normal_material.h"
#include "mesh_lambert_material.h"
#include "mesh.h"

#include <thread>

class GLMaterialsCubeMap: public ApplicationBase{
    PerspectiveCamera::sptr perspectiveCamera;
    OrbitControl::sptr pCameraControl;

public:
    GLMaterialsCubeMap(int x, int y):ApplicationBase(x,y){}

    virtual void init() override {
        renderer->setClearColor(Color().setHex(0x000000));
        //renderer->physicallyCorrectLights = true;
        renderer->outputEncoding = TextureEncoding::sRGBEncoding;
        renderer->shadowMap->enabled = true;
        renderer->shadowMap->type = ShadowMapType::PCFSoftShadowMap;
        renderer->toneMapping = ToneMapping::ACESFilmicToneMapping;
        //renderer->setAnimationLoop( render );

        auto aspect = (float)screenX / screenY;
        perspectiveCamera = std::make_shared<PerspectiveCamera>(45.f, aspect , 500.f, 3000.f);
        perspectiveCamera->position.set( 0, 0, 1500 );
        //scene->add(camera);

        camera = perspectiveCamera;

        scene = std::make_shared<Scene>();
        scene->setBackgroundColor(Color().set(0x000000));

        std::string rootDir = threecpp::getProjectPath();
        std::string fileSeparator = threecpp::getFileSeparator();
        std::string resourceDir = std::string(rootDir).append(fileSeparator).append("asset").append(fileSeparator)
                .append("textures").append(fileSeparator)
                .append("cube").append(fileSeparator)
                .append("SwedishRoyalCastle").append(fileSeparator);

        auto format = ".jpg";
        auto fileurls = {
                resourceDir + "px" + format, resourceDir + "nx" + format,
                resourceDir + "py" + format, resourceDir + "ny" + format,
                resourceDir + "pz" + format, resourceDir + "nz" + format
        };

        auto reflectionCube = CubeTextureLoader::load( fileurls );
        reflectionCube->setNeedsUpdate(true);
        reflectionCube->format = PixelFormat::RGBFormat;
        auto refractionCube = CubeTextureLoader::load( fileurls );
        refractionCube->mapping = TextureMapping::CubeRefractionMapping;
        refractionCube->setNeedsUpdate(true);
        refractionCube->format = PixelFormat::RGBFormat;

        scene->setBackgroundCubeTexture(reflectionCube);

        // lights
        auto ambientLight = AmbientLight::create( 0xffffff );
        scene->add( ambientLight );

        auto pointLight = PointLight::create( 0xffffff, 2 );
        scene->add( pointLight );

        //materials
        auto cubeMaterial3 = MeshLambertMaterial::create();
        cubeMaterial3->color = 0xff6600;
        cubeMaterial3->envMap = reflectionCube;
        cubeMaterial3->combine = Combine::MixOperation;
        cubeMaterial3->reflectivity = 0.3;
        cubeMaterial3->side = Side::DoubleSide;

        auto cubeMaterial2 = MeshLambertMaterial::create();
        cubeMaterial2->color = 0xffee00;
        cubeMaterial2->envMap = refractionCube;
        cubeMaterial2->refractionRatio = 0.95;
        cubeMaterial2->side = Side::DoubleSide;

        auto cubeMaterial1 = MeshLambertMaterial::create();
        cubeMaterial1->color = 0xffffff;
        cubeMaterial1->envMap = reflectionCube;
        cubeMaterial1->side = Side::DoubleSide;


        resourceDir = rootDir.append(fileSeparator).append("asset").append(fileSeparator)
                .append("models").append(fileSeparator)
                .append("obj").append(fileSeparator)
                .append("walt").append(fileSeparator);
        std::thread thread1([&](const std::string& filepath){
                                Object3D::sptr objGroup;

                                OBJLoader loader;

                                objGroup = loader.load(resourceDir + filepath);

                                auto head = objGroup->children[0];
                                head->scale.set(12,12,12);
                                head->position.y = - 500;
                                head->material = cubeMaterial1;

                                auto head2G = head->geometry->clone();//head->clone(true);
                                auto head2 = Mesh::create(std::shared_ptr<Geometry>(head2G),cubeMaterial2);
                                head2->scale.set(12,12,12);
                                head2->position.y = - 500;
                                head2->position.x = - 800;

                                auto head3 = Mesh::create(std::shared_ptr<Geometry>(head2G),cubeMaterial3);
                                head3->scale.set(12,12,12);
                                head3->position.y = - 500;
                                head3->position.x =  800;

//
//                                auto head3 = head->clone(true);
//                                head3->position.x = 900;
//                                head3->material = cubeMaterial3;
                                scene->add(head);
                                scene->add(head2);
                                scene->add(head3);

                            }
                ,std::string("WaltHead.obj"));
        thread1.join();

        pCameraControl = std::make_shared<OrbitControl>(perspectiveCamera);
        pCameraControl->minDistance = 1000;
        pCameraControl->maxDistance = 2400;
        pCameraControl->enablePan = true;
        pCameraControl->enableDamping = true;
        controller = pCameraControl;


    }

    virtual void render() override;

    virtual void showControls() override;
};


#endif //THREE_CPP_GL_MATERIALS_CUBEMAP_H
