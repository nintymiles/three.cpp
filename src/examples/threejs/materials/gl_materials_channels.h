//
// Created by SeanR on 11/29/2023.
//

#ifndef THREE_CPP_GL_MATERIALS_CHANNELS_H
#define THREE_CPP_GL_MATERIALS_CHANNELS_H

#include "application_model.h"
#include "orbit_control.h"
#include "trackball_control.h"
#include "texture_loader.h"
#include "obj_loader.h"

#include "ambient_light.h"
#include "point_light.h"

#include "mesh_standard_material.h"
#include "mesh_depth_material.h"
#include "mesh_normal_material.h"
#include "mesh.h"

#include <thread>

class GLMaterialsChannels: public ApplicationBase{
    PerspectiveCamera::sptr perspectiveCamera;
    OrthographicCamera::sptr orthoCamera;
    OrbitControl::sptr pCameraControl,oCameraControl;
    Mesh::sptr headMesh;

    MeshNormalMaterial::sptr materialStandard;
    MeshNormalMaterial::sptr materialNormal;
    MeshDepthMaterial::sptr materialDepth,materialDepthRGBA;
    std::vector<std::string> materialVec,cameraVec,controlVec,sideVec;
    int selMaterial=1,selCamera=0,selSide=0;
    const float SCALE = 2.436143; // from original model
    const float BIAS = - 0.428408; // from original model

    void initComboData(){
        materialVec = { "standard", "normal", "depthBasic", "depthRGBA" };
        cameraVec = { "perspective", "ortho" };
        sideVec = { "front", "back", "double" };
    }
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

        auto aspect = (float)screenX / screenY;
        perspectiveCamera = std::make_shared<PerspectiveCamera>(45.f, aspect , 500.f, 3000.f);
        perspectiveCamera->position.set( 0, 0, 1500 );
        //scene->add(camera);

        camera = perspectiveCamera;


        auto height = 500;
        orthoCamera = OrthographicCamera::create( - height * aspect, height * aspect, height, - height, 1000, 2500 );
        orthoCamera->position.set( 0, 0, 1500 );
        //scene->add( orthoCamera );
//        camera = orthoCamera;

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
        materialStandard = MeshStandardMaterial::create();
        materialStandard->color = 0xffffff;
        materialStandard->metalness = 0.5;
        materialStandard->roughness = 0.6;

        materialStandard->displacementMap = displacementMap;
        materialStandard->displacementScale = SCALE;
        materialStandard->displacementBias = BIAS;
        materialStandard->aoMap = aoMap,
        materialStandard->normalMap = normalMap;
        materialStandard-> normalScale = Vector2( 1, - 1 );
        //flatShading: true,
        materialStandard->side = Side::DoubleSide;

        materialDepth = MeshDepthMaterial::create();
        materialDepth->depthPacking = DepthPackingStrategies::BasicDepthPacking;
        materialDepth->displacementMap = displacementMap;
        materialDepth->displacementScale = SCALE,
        materialDepth->displacementBias = BIAS,
        materialDepth->side = Side::DoubleSide;

        materialDepthRGBA = MeshDepthMaterial::create();
        materialDepthRGBA->depthPacking = DepthPackingStrategies::RGBADepthPacking;
        materialDepthRGBA->displacementMap = displacementMap;
        materialDepthRGBA->displacementScale = SCALE,
        materialDepthRGBA->displacementBias = BIAS,
        materialDepthRGBA->side = Side::DoubleSide;

        materialNormal = MeshNormalMaterial::create();
        materialNormal -> displacementMap = displacementMap;
        materialNormal -> displacementScale = SCALE;
        materialNormal -> displacementBias = BIAS;
        materialNormal -> normalMap = normalMap,
        materialNormal -> normalScale = Vector2( 1, - 1 ),
                //flatShading: true,
        materialNormal->side = Side::DoubleSide;;

        std::thread thread1([&](const std::string& filepath){
                Object3D::sptr objGroup;

                OBJLoader loader;

                objGroup = loader.load(resourceDir + filepath);

                auto geometry = objGroup->children[0]->geometry;
                geometry->uvs2 = geometry->uvs;
                //geometry-> = geometry->uv;
                geometry->center();

                headMesh = Mesh::create(objGroup->children[0]->geometry,materialStandard);
                headMesh->scale.multiplyScalar( 25 );

                headMesh->castShadow = true;
                headMesh->receiveShadow = true;

                scene->add(headMesh);
                }
                ,std::string("ninjaHead_Low.obj"));
        thread1.join();

        pCameraControl = std::make_shared<OrbitControl>(perspectiveCamera);
        pCameraControl->minDistance = 1000;
        pCameraControl->maxDistance = 2400;
        pCameraControl->enablePan = true;
        pCameraControl->enableDamping = true;
        controller = pCameraControl;

        oCameraControl = std::make_shared<OrbitControl>(orthoCamera);
        oCameraControl->minZoom = 0.5;
        oCameraControl->maxZoom = 1.5;
        oCameraControl->enablePan = true;
        oCameraControl->enableDamping = true;
        //controller = oCameraControl;

//        Vector4 screen = Vector4(0, 0, screenX, screenY);
//        std::shared_ptr<TrackballControls> tcontroller = std::make_shared<TrackballControls>(camera, screen);
//        controller = tcontroller;
//
//        tcontroller->staticMoving = false;
//        tcontroller->rotateSpeed = 4.0f;
//        tcontroller->zoomSpeed = 3;
//        tcontroller->panSpeed = 3;
//        tcontroller->noZoom = true;
//        tcontroller->noPan = true;
//        tcontroller->noRotate = false;
//        tcontroller->dynamicDampingFactor = 0.3f;

        initComboData();

    }

    virtual void render() override;

    virtual void showControls() override;
};


#endif //THREE_CPP_GL_MATERIALS_CHANNELS_H
