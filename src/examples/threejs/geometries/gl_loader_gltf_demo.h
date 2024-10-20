//
// Created by SeanR on 2024/01/03.
//

#ifndef THREE_CPP_GL_LOADER_GLTF_DEMO_H
#define THREE_CPP_GL_LOADER_GLTF_DEMO_H

#include "application_model.h"
#include "group.h"

#include "orthographic_camera.h"

#include "trackball_control.h"

#include "box_geometry.h"

#include "gltf_loader.h"

#include "animation_mixer.h"
#include "timer.h"

#include <thread>
#include <filesystem>

class GLLoaderGLTFDemo: public ApplicationBase{
    Group::sptr objGroup;
    MeshBasicMaterial::sptr meshMaterial;

    AnimationMixer mixer;
    Timer timer;

public:
    GLLoaderGLTFDemo(int x, int y):ApplicationBase(x,y){}

    virtual void init() override {
        renderer->setClearColor(Color().setHex(0x000000));
        renderer->shadowMap->enabled = true;
        //renderer->shadowMap->type = ShadowMapType::PCFSoftShadowMap;

        auto height = 100;
        auto aspect = (float)screenX / screenY;
        //camera = OrthographicCamera::create( - height * aspect, height * aspect, height, - height, 1000, 2500 );
        camera = std::make_shared<PerspectiveCamera>(45.0f, aspect , 1.f, 2000.0f);
        camera->position.set(0.0f, 0.0f, 250.0f);
        //camera->lookAt(Vector3());
        isPerspective = true;

        scene = std::make_shared<Scene>();
        scene->setBackgroundColor(Color().set(0x000000));

        auto ambientLight = AmbientLight::create(Color().setHex(0xcccccc),3.f);
//        ambientLight->position.set(0, 0, 10);
        scene->add(ambientLight);

        auto pointLight = PointLight::create(Color().setHex(0xffffff),2.0f);
        pointLight->position.set(0, 20, 50);
        pointLight->castShadow = true;
        scene->add(pointLight);

        meshMaterial = MeshBasicMaterial::create();
        meshMaterial->color.set(0x7777ff);
        meshMaterial->name = "Basic Material";
        meshMaterial->flatShading = true;
        meshMaterial->opacity = 0.01f;
        meshMaterial->fog = true;
        meshMaterial->colorWrite = true;

        std::string dir = threecpp::getProjectPath();

        GLTFLoader gltfLoader{};
        std::string relPath = threecpp::getFileSeparator().append("asset").append(threecpp::getFileSeparator()).append("models")
                .append(threecpp::getFileSeparator()).append("gltf").append(threecpp::getFileSeparator());
//                .append("DamagedHelmet").append(threecpp::getFileSeparator());
//        Group::sptr gltfGroup = gltfLoader.load(dir + relPath +"DamagedHelmet.gltf");
//        Group::sptr gltfGroup = gltfLoader.load(dir + relPath +"LittlestTokyo.glb");
//        Group::sptr gltfGroup = gltfLoader.load(dir + relPath +"Flamingo.glb");
//        Geometry::sptr geometry1 = gltfGroup->children[0]->children[0]->children[0]->geometry;
//        Mesh::sptr mesh = Mesh::create(geometry1,MeshBasicMaterial::create(0xff0000));
//        gltfGroup->scale.multiplyScalar(100);
//        gltfGroup->position.setY(-100);
//        gltfGroup->rotation.y(-math_number::PI/2);

        GLTFModel gltfModel = gltfLoader.loadModel(dir + relPath +"Flamingo.glb");

        scene->add(gltfModel.scene);

        mixer = AnimationMixer(gltfModel.scene);
        mixer.clipAction(gltfModel.animations[0],nullptr,AnimationBlendMode::NormalAnimationBlendMode);

//        BoxGeometry::sptr box = BoxGeometry::create( 1, 1, 1, 1, 1, 1 );
//        scene->add(Mesh::create(box,MeshBasicMaterial::create(0xff0000)));

        Vector4 screen = Vector4(0, 0, screenX, screenY);
        std::shared_ptr<TrackballControls> tcontroller = std::make_shared<TrackballControls>(camera, screen);
        controller = tcontroller;

        tcontroller->staticMoving = false;
        tcontroller->rotateSpeed = 4.0f;
        tcontroller->zoomSpeed = 3;
        tcontroller->panSpeed = 3;
        tcontroller->noZoom = false;
        tcontroller->noPan = false;
        tcontroller->noRotate = false;
        tcontroller->dynamicDampingFactor = 0.3f;

        timer = Timer();
    }

    virtual void render() override;


};


#endif //THREE_CPP_GL_LOADER_GLTF_DEMO_H
