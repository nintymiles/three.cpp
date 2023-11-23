//
// Created by SeanR on 11/23/2023.
//

#ifndef THREE_CPP_GL_LIGHTS_SPOTLIGHT_H
#define THREE_CPP_GL_LIGHTS_SPOTLIGHT_H

#include "application_model.h"
#include "orbit_control.h"
#include "ambient_light.h"
#include "spot_light.h"

#include "plane_geometry.h"
#include "mesh.h"
#include "mesh_lambert_material.h"

#include "texture_loader.h"
#include "texture.h"

#include "obj_loader.h"
#include "trackball_control.h"

#include <thread>

class GLLightsSpotlight: public ApplicationBase{
    SpotLight::sptr spotLight;

public:
    GLLightsSpotlight(int x, int y):ApplicationBase(x,y){}

    virtual void init() override {
        renderer->setClearColor(Color().setHex(0x000000));
        //renderer->physicallyCorrectLights = true;
        renderer->outputEncoding = TextureEncoding::sRGBEncoding;
        renderer->shadowMap->enabled = true;
        renderer->shadowMap->type = ShadowMapType::PCFSoftShadowMap;
        renderer->toneMapping = ToneMapping::ACESFilmicToneMapping;
        //renderer->setAnimationLoop( render );

        camera = std::make_shared<PerspectiveCamera>(35.0f, screenX/screenY , .1f, 1000.0f);
        camera->position.set( 76, 50, 10 );
        camera->rotation.set( - 1.29, 1.15, 1.26 );

        scene = std::make_shared<Scene>();
        scene->setBackgroundColor(Color().set(0x000000));

        auto ambientLight = AmbientLight::create(Color().setHex(0xcccccc),.8f);
        scene->add(ambientLight);

        std::vector<std::string> filenames = {"disturb.jpg", "colors.png", "uv_grid_opengl.jpg" };

        std::string rootDir = threecpp::getProjectPath();
        std::string fileSeparator = threecpp::getFileSeparator();
        std::string resourceDir = rootDir.append(fileSeparator).append("asset").append(fileSeparator).append("textures").append(fileSeparator);

        TextureLoader textureLoader = TextureLoader();
        std::unordered_map<std::string,Texture::sptr> textureMap{};
        for(auto filename:filenames){
            auto texture = textureLoader.load( resourceDir + filename );
            texture->minFilter = TextureFilter::LinearFilter;
            texture->magFilter = TextureFilter::LinearFilter;
            texture->encoding = TextureEncoding::sRGBEncoding;

            textureMap[filename] = texture;
        }

        spotLight = SpotLight::create( 0xffffff, 10 );
        spotLight->position.set( 25, 50, 25 );
        spotLight->angle = math_number::PI / 6;
        spotLight->penumbra = 1;
        spotLight->decay = 2;
        spotLight->distance = 100;
        spotLight->map = textureMap["disturb.jpg"];

        spotLight->castShadow = true;
        spotLight->shadow->shadowMapSize.x = 1024;
        spotLight->shadow->shadowMapSize.y = 1024;
        spotLight->shadow->camera->_near = 10;
        spotLight->shadow->camera->_far = 200;
        //spotLight->shadow->focus = 1;
        scene->add( spotLight );

        PlaneGeometry::sptr geometry = PlaneGeometry::create( 1000, 1000 );
        //MeshLambertMaterial::sptr material = MeshLambertMaterial::create( 0x808080 );
        auto planeMaterial = std::make_shared<MeshLambertMaterial>();
        planeMaterial->color = Color().setHex(0x808080);

        Mesh::sptr mesh = Mesh::create( geometry, planeMaterial );
        mesh->position.set( 0, - 1, 0 );
        //mesh->rotation.setX( - math_number::PI / 2 );
        mesh->receiveShadow = true;
        scene->add( mesh );

        std::string dir = threecpp::getProjectPath();
        std::thread thread1([&](const std::string& filepath){
                Group::sptr objGroup;

                OBJLoader loader;
                std::string relativeDir = threecpp::getFileSeparator().append("asset").append(threecpp::getFileSeparator()).append("models")
                        .append(threecpp::getFileSeparator()).append("obj").append(threecpp::getFileSeparator());
                objGroup = loader.load(dir + relativeDir + filepath);

                objGroup->scale.multiplyScalar( 0.1 );
                objGroup->position.setZ(30);
                objGroup->position.setY(0);
                objGroup->position.setX(0);
                objGroup->rotation.setY( - math_number::PI  );
                objGroup->rotation.setZ( math_number::PI/2  );

                scene->add(objGroup);
                            }
                ,std::string("lucy100k.obj"));
        thread1.join();

        std::shared_ptr<OrbitControl> oController = std::make_shared<OrbitControl>(camera, scene);
        oController->minDistance = 20;
        oController->maxDistance = 500;
        oController->target.set( 0, 18, 0 );
        oController->update();
        controller = oController;

        Vector4 screen = Vector4(0, 0, screenX, screenY);
        //controller = std::make_shared<TrackballControls>(camera, screen);
        std::shared_ptr<TrackballControls> tcontroller = std::make_shared<TrackballControls>(camera, screen);
        controller = tcontroller;

        tcontroller->staticMoving = false;
        tcontroller->rotateSpeed = 2.0f;
        tcontroller->zoomSpeed = 3;
        tcontroller->panSpeed = 3;
        tcontroller->noZoom = false;
        tcontroller->noPan = false;
        tcontroller->noRotate = false;
        tcontroller->dynamicDampingFactor = 0.3f;

    }
};


#endif //THREE_CPP_GL_LIGHTS_SPOTLIGHT_H
