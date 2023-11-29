//
// Created by SeanR on 11/29/2023.
//

#ifndef THREE_CPP_GL_MATERIALS_CHANNELS_H
#define THREE_CPP_GL_MATERIALS_CHANNELS_H

#include "application_model.h"
#include "orbit_control.h"
#include "texture_loader.h"
#include "obj_loader.h"

#include "ambient_light.h"
#include "point_light.h"

#include "mesh_standard_material.h"

#include <thread>

class GLMaterialsChannels: public ApplicationBase{
public:
    GLMaterialsChannels(int x, int y):ApplicationBase(x,y){}

    virtual void init() override {
        renderer->setClearColor(Color().setHex(0x000000));
        //renderer->physicallyCorrectLights = true;
        renderer->outputEncoding = TextureEncoding::sRGBEncoding;
        renderer->shadowMap->enabled = true;
        renderer->shadowMap->type = ShadowMapType::PCFSoftShadowMap;
        renderer->toneMapping = ToneMapping::ACESFilmicToneMapping;
        //renderer->setAnimationLoop( render );

        camera = std::make_shared<PerspectiveCamera>(45.f, screenX/screenY , 500.f, 3000.f);
        camera->position.set( 0, 0, 1500 );
        //scene->add(camera);

        scene = std::make_shared<Scene>();
        scene->setBackgroundColor(Color().set(0x000000));

        // lights
        auto ambientLight = AmbientLight::create( 0xffffff, 0.1 );
        scene->add( ambientLight );

        auto pointLight = PointLight::create( 0xff0000, 0.5 );
        pointLight->position.z = 2500;
        scene->add( pointLight );

        auto pointLight2 = PointLight::create( 0xff6666, 1 );
        camera->add( pointLight2 );

        auto pointLight3 = PointLight::create( 0x0000ff, 0.5 );
        pointLight3->position.x = - 1000;
        pointLight3->position.z = 1000;
        scene->add( pointLight3 );

        std::string rootDir = threecpp::getProjectPath();
        std::string fileSeparator = threecpp::getFileSeparator();
        std::string resourceDir = rootDir.append(fileSeparator).append("asset").append(fileSeparator)
                .append("models").append(fileSeparator)
                .append("obj").append(fileSeparator)
                .append("ninja").append(fileSeparator);

        // textures
        auto textureLoader = TextureLoader();
        auto normalMap = textureLoader.load( resourceDir + "normal.png" );
        auto aoMap = textureLoader.load( resourceDir + "ao.jpg ");
        auto displacementMap = textureLoader.load( resourceDir + "displacement.jpg" );

        // material
        auto materialStandard = MeshStandardMaterial::create();
        materialStandard->color = 0xffffff;
        materialStandard->metalness = 0.5;
        materialStandard->roughness = 0.6;

        materialStandard->displacementMap = displacementMap;

//        materialStandard->displacementScale = SCALE;
//                                                                   displacementBias: BIAS,

        materialStandard->aoMap = aoMap,
        materialStandard->normalMap = normalMap;
        materialStandard-> normalScale = Vector2( 1, - 1 );

                //flatShading: true,
        materialStandard->side = Side::DoubleSide;

        std::thread thread1([&](const std::string& filepath){
                Group::sptr objGroup;

                OBJLoader loader;

                objGroup = loader.load(resourceDir + filepath);

//                const geometry = group.children[ 0 ].geometry;
//                geometry.attributes.uv2 = geometry.attributes.uv;
//                geometry.center();

                objGroup->scale.multiplyScalar( 25 );

                objGroup->castShadow = true;
                objGroup->receiveShadow = true;

                scene->add(objGroup);
                            }
                ,std::string("ninjaHead_Low.obj"));
        thread1.join();

        std::shared_ptr<OrbitControl> oController = std::make_shared<OrbitControl>(camera);
        oController->minDistance = 20;
        oController->maxDistance = 500;
        oController->target.set( 0, 18, 0 );
        oController->update();
        controller = oController;


    }
};


#endif //THREE_CPP_GL_MATERIALS_CHANNELS_H
