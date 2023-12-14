//
// Created by SeanR on 12/14/2023.
//

#ifndef THREE_CPP_GL_MATERIALS_CUBEMAP_REFRACTION_H
#define THREE_CPP_GL_MATERIALS_CUBEMAP_REFRACTION_H

#include "application_model.h"
#include "orbit_control.h"
#include "cube_texture_loader.h"
#include "obj_loader.h"

#include "ambient_light.h"
#include "point_light.h"

#include "mesh_phong_material.h"
#include "sphere_geometry.h"

#include "mesh.h"

#include <thread>

class GLMaterialsCubeMapRefraction: public ApplicationBase{
    PerspectiveCamera::sptr perspectiveCamera;
    OrbitControl::sptr pCameraControl;
    PointLight::sptr pointLight;

public:
    GLMaterialsCubeMapRefraction(int x, int y):ApplicationBase(x,y){}

    virtual void init() override {
        renderer->setClearColor(Color().setHex(0x000000));
        //renderer->physicallyCorrectLights = true;
        renderer->outputEncoding = TextureEncoding::sRGBEncoding;
        renderer->shadowMap->enabled = true;
        renderer->shadowMap->type = ShadowMapType::PCFSoftShadowMap;
        renderer->toneMapping = ToneMapping::ACESFilmicToneMapping;
        //renderer->setAnimationLoop( render );

        auto aspect = (float)screenX / screenY;
        perspectiveCamera = std::make_shared<PerspectiveCamera>(50.f, aspect , 1.f, 100000.f);
        perspectiveCamera->position.set( 0, 0, -4000 );
        //scene->add(camera);

        camera = perspectiveCamera;

        scene = std::make_shared<Scene>();
        scene->setBackgroundColor(Color().set(0x000000));

        std::string rootDir = threecpp::getProjectPath();
        std::string fileSeparator = threecpp::getFileSeparator();
        std::string resourceDir = std::string(rootDir).append(fileSeparator).append("asset").append(fileSeparator)
                .append("textures").append(fileSeparator)
                .append("cube").append(fileSeparator)
                .append("Park3Med").append(fileSeparator);

        auto format = ".jpg";
        auto fileurls = {
                resourceDir + "px" + format, resourceDir + "nx" + format,
                resourceDir + "py" + format, resourceDir + "ny" + format,
                resourceDir + "pz" + format, resourceDir + "nz" + format
        };


        auto refractionCube = CubeTextureLoader::load( fileurls );
        refractionCube->mapping = TextureMapping::CubeRefractionMapping;
        refractionCube->setNeedsUpdate(true);
        refractionCube->format = PixelFormat::RGBFormat;

        scene->setBackgroundCubeTexture(refractionCube);


        // lights
        auto ambientLight = AmbientLight::create( 0xffffff );
        scene->add( ambientLight );

        pointLight = PointLight::create( 0xffffff, 2 );
        scene->add( pointLight );

        // light representation
        auto sphere = SphereGeometry::create( 100, 16, 8 );

        auto mesh = Mesh::create( sphere, MeshBasicMaterial::create( 0xffffff ) );
        mesh->scale.set( 0.05, 0.05, 0.05 );
        pointLight->add( mesh );

        //materials
        auto cubeMaterial3 = MeshPhongMaterial::create();
        cubeMaterial3->color = 0xccddff;
        cubeMaterial3->envMap = refractionCube;
        cubeMaterial3->refractionRatio = 0.98;
        cubeMaterial3->side = Side::DoubleSide;

        auto cubeMaterial2 = MeshPhongMaterial::create();
        cubeMaterial2->color = 0xccfffd;
        cubeMaterial2->envMap = refractionCube;
        cubeMaterial2->refractionRatio = 0.985;
        cubeMaterial2->side = Side::DoubleSide;

        auto cubeMaterial1 = MeshPhongMaterial::create();
        cubeMaterial1->color = 0xffffff;
        cubeMaterial1->envMap = refractionCube;
        cubeMaterial1->refractionRatio = 0.98;
        cubeMaterial1->side = Side::DoubleSide;


        resourceDir = rootDir.append(fileSeparator).append("asset").append(fileSeparator)
                .append("models").append(fileSeparator)
                .append("obj").append(fileSeparator);
        std::thread thread1([&](const std::string& filepath){
                                Object3D::sptr objGroup;

                                OBJLoader loader;

                                objGroup = loader.load(resourceDir + filepath);

                                auto obj = objGroup->children[0];
                                auto objGeo = obj->geometry;
                                objGeo->center();
                                //objGeo->computeVertexNormals();

                                float s = .8f;

                                auto obj1 = Mesh::create(std::shared_ptr<Geometry>(objGeo),cubeMaterial1);
                                obj1->scale.set(s,s,s);
                                obj1->rotation.setX( math_number::PI * 0.5 );

                                auto obj2 = Mesh::create(std::shared_ptr<Geometry>(objGeo),cubeMaterial2);
                                obj2->scale.set(s,s,s);
                                obj2->position.x = - 1200;
                                obj2->rotation.setX( math_number::PI * 0.5 );

                                auto obj3 = Mesh::create(std::shared_ptr<Geometry>(objGeo),cubeMaterial3);
                                obj3->scale.set(s,s,s);
                                obj3->position.x = 1200;
                                obj3->rotation.setX( math_number::PI * 0.5 );

                                scene->add(obj1);
                                scene->add(obj2);
                                scene->add(obj3);

                            }
                ,std::string("lucy100k.obj"));
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


#endif //THREE_CPP_GL_MATERIALS_CUBEMAP_REFRACTION_H
