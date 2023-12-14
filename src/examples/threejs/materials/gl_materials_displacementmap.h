//
// Created by SeanR on 12/12/2023.
//

#ifndef THREE_CPP_GL_MATERIALS_DISPLACEMENTMAP_H
#define THREE_CPP_GL_MATERIALS_DISPLACEMENTMAP_H

#include "application_model.h"
#include "orbit_control.h"
#include "cube_texture_loader.h"
#include "obj_loader.h"

#include "ambient_light.h"
#include "point_light.h"

#include "mesh_standard_material.h"
#include "mesh_lambert_material.h"
#include "mesh.h"

#include <thread>

class GLMaterialsDisplacementMap: public ApplicationBase{
    OrthographicCamera::sptr orthoCamera;
    OrbitControl::sptr pCameraControl;
    Material::sptr material;
    Light::sptr ambientLight,pointLight;

    struct Settings{
        float roughness = 0.4;
        float metalness = 1.0;
        float aoMapIntensity = 1.0f;
        float envMapIntensity = 1.0;
        float displacementScale= 2.436143;
        float normalScale = 1.0;
        float ambientIntensity = 0.2;
    };

//    float roughness = 0.4;
//    float metalness = 1.0;
//    float aoMapIntensity = 1.0f;
//    float envMapIntensity = 1.0;
//    float displacementScale= 2.436143;
//    float normalScale = 1.0;
//    float ambientIntensity = 0.2;

    float height = 500;
    float r = 0.0;

    Settings setting{};

public:
    GLMaterialsDisplacementMap(int x, int y):ApplicationBase(x,y){}

    virtual void init() override {
        renderer->setClearColor(Color().setHex(0x000000));
        //renderer->physicallyCorrectLights = true;
        renderer->outputEncoding = TextureEncoding::sRGBEncoding;
//        renderer->shadowMap->enabled = true;
//        renderer->shadowMap->type = ShadowMapType::PCFSoftShadowMap;
//        renderer->toneMapping = ToneMapping::ACESFilmicToneMapping;
        //renderer->setAnimationLoop( render );

        auto aspect = (float)screenX / screenY;
        orthoCamera = std::make_shared<OrthographicCamera>( - height * aspect, height * aspect, height, - height, 1, 10000 );
        orthoCamera->position.set( 0, 0, 1500 );
        //scene->add(camera);

        camera = orthoCamera;

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

//        scene->setBackgroundCubeTexture(reflectionCube);

        // textures
        resourceDir = std::string(rootDir).append(fileSeparator).append("asset").append(fileSeparator)
                .append("models").append(fileSeparator)
                .append("obj").append(fileSeparator)
                .append("ninja").append(fileSeparator);

        auto normalMap = TextureLoader::load( resourceDir + "normal.png" );
        auto aoMap = TextureLoader::load( resourceDir + "ao.jpg" );
        auto displacementMap = TextureLoader::load( resourceDir + "displacement.jpg" );

        // material
        material = MeshStandardMaterial::create();
        material->color = 0x888888;
        material->roughness = setting.roughness,
        material->metalness = setting.metalness,
        material->normalMap = normalMap,
        material->normalScale = Vector2( 1, - 1 ); // why does the normal map require negation in this case?
        material->aoMap = aoMap;
        material->aoMapIntensity = setting.aoMapIntensity;
        material->displacementMap = displacementMap;
        material->displacementScale = setting.displacementScale,
        material->displacementBias = - 0.428408; // from original model
        material->envMap = reflectionCube;
        material->envMapIntensity = setting.envMapIntensity;
        material->side = Side::DoubleSide;


        // lights
        ambientLight = AmbientLight::create( 0xffffff,setting.ambientIntensity );
        scene->add( ambientLight );

        pointLight = PointLight::create( 0xff0000, 0.5 );
        pointLight->position.z = 2500.f;
        scene->add( pointLight );

        auto pointLight2 = PointLight::create( 0xff6666, 1.f );
        scene->add( pointLight2 );

        auto pointLight3 = PointLight::create( 0x0000ff, 0.5 );
        pointLight3->position.set(-1000.f, 0, 1000.f);
        scene->add( pointLight3 );


        resourceDir = rootDir.append(fileSeparator).append("asset").append(fileSeparator)
                .append("models").append(fileSeparator)
                .append("obj").append(fileSeparator)
                .append("ninja").append(fileSeparator);
        std::thread thread1([&](const std::string& filepath){
                                Object3D::sptr objGroup;

                                OBJLoader loader;

                                objGroup = loader.load(resourceDir + filepath);

                                auto headG = objGroup->children[0]->geometry;
                                headG->uvs2 = headG->uvs;
                                headG->center();

                                auto mesh = Mesh::create(headG,material);
                                mesh->scale.multiplyScalar(25);
                                scene->add(mesh);

                            }
                ,std::string("ninjaHead_Low.obj"));
        thread1.join();

        pCameraControl = std::make_shared<OrbitControl>(camera);
        pCameraControl->enablePan = true;
        pCameraControl->enableDamping = true;
        controller = pCameraControl;


    }

    virtual void render() override;

    virtual void showControls() override;
};


#endif //THREE_CPP_GL_MATERIALS_DISPLACEMENTMAP_H
