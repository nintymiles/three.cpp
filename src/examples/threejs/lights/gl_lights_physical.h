//
// Created by seanren on 11/15/2023.
//

#ifndef THREE_CPP_GL_LIGHTS_PHYSICAL_H
#define THREE_CPP_GL_LIGHTS_PHYSICAL_H

#include "application_model.h"

#include "mesh_standard_material.h"
#include "sphere_geometry.h"
#include "plane_geometry.h"
#include "box_geometry.h"

#include "hemisphere_light.h"

#include "texture_loader.h"

#include "timer.h"
#include "number.h"
#include "trackball_control.h"

#include <thread>
#include <filesystem>

class GLLightsPhysical: public ApplicationBase{
    PointLight::sptr bulbLight;
    Mesh::sptr ballMesh;
    HemisphereLight::sptr hemiLight;
    MeshStandardMaterial::sptr ballMat, cubeMat, floorMat,bulbMat;

    bool previousShadowMap = false;

    std::vector<std::pair<std::string,int>> bulbLuminousPowers;
    //std::vector<std::string> bulbLuminousPowerNames;
    std::vector<std::pair<std::string,float>> hemiLuminousIrradiances;
    //std::vector<std::string> hemiLuminousIrradianceNames;

    bool shadows = true;
    float exposure = 0.68;
//    std::string bulbPower;//: Object.keys( bulbLuminousPowers )[ 4 ],
//    std::string hemiIrradiance;//: Object.keys( hemiLuminousIrradiances )[ 0 ]
    int hemiIrradianceSelIdx = 4;
    int bulbPowerSelIdx = 0;

public:
    GLLightsPhysical(int x, int y):ApplicationBase(x,y){}

    virtual void init() override {
        renderer->setClearColor(Color().setHex(0x000000));
        renderer->physicallyCorrectLights = true;
        renderer->outputEncoding = TextureEncoding::sRGBEncoding;
        renderer->shadowMap->enabled = true;
        //renderer->shadowMap->type = ShadowMapType::PCFSoftShadowMap;
        renderer->toneMapping = ToneMapping::ReinhardToneMapping;

        camera = std::make_shared<PerspectiveCamera>(50.0f, screenX/screenY , .1f, 100.0f);
        camera->position.set(-4.f, 2.f, 4.f);

        scene = std::make_shared<Scene>();
        scene->setBackgroundColor(Color().set(0x000000));

        SphereGeometry::sptr bulbGeometry = SphereGeometry::create( 0.02, 16, 8 );
        bulbLight = PointLight::create( 0xffee88, 1, 100, 2 );

        bulbMat = MeshStandardMaterial::create();
        bulbMat->emissive = 0xffffee;
        bulbMat->emissiveIntensity = 1;
        bulbMat->color = 0x000000;

        bulbLight->add(Mesh::create( bulbGeometry, bulbMat ));
        bulbLight->position.set( 0, 2, 0 );
        bulbLight->castShadow = true;
        scene->add( bulbLight );

        hemiLight = HemisphereLight::create( 0xddeeff, 0x0f0e0d, 0.02 );
        scene->add( hemiLight );

        floorMat = MeshStandardMaterial::create();
        floorMat->roughness=0.8;
        floorMat->color=0xffffff;
        floorMat->metalness=0.2;
        floorMat->bumpScale=0.0005;

        std::string rootDir = threecpp::getProjectPath();
        std::string fileSeparator = threecpp::getFileSeparator();
        std::string resourceDir = rootDir.append(fileSeparator).append("asset").append(fileSeparator).append("textures").append(fileSeparator);

        TextureLoader textureLoader = TextureLoader();
        auto hwDiffMap = textureLoader.load( resourceDir + "hardwood2_diffuse.jpg");
        if(hwDiffMap){
            hwDiffMap->wrapS = Wrapping::RepeatWrapping;
            hwDiffMap->wrapT = Wrapping::RepeatWrapping;
            hwDiffMap->anisotropy = 4;
            hwDiffMap->repeat.set( 10, 24 );
            //hwDiffMap->encoding = TextureEncoding::sRGBEncoding;
            floorMat->map = hwDiffMap;
            floorMat->needsUpdate = true;
        }

        auto hwBumpMap = textureLoader.load( resourceDir + "hardwood2_bump.jpg");
        if(hwBumpMap){
            hwBumpMap->wrapS = Wrapping::RepeatWrapping;
            hwBumpMap->wrapT = Wrapping::RepeatWrapping;
            hwBumpMap->anisotropy = 4;
            hwBumpMap->repeat.set( 10, 24 );
            //hwBumpMap->type = TextureDataType::HalfFloatType;
            //hwBumpMap->encoding = TextureEncoding::sRGBEncoding;
            floorMat->bumpMap = hwBumpMap;
            floorMat->needsUpdate = true;
        }

        auto hwRoughMap = textureLoader.load( resourceDir + "hardwood2_roughness.jpg");
        if(hwRoughMap){
            hwRoughMap->wrapS = Wrapping::RepeatWrapping;
            hwRoughMap->wrapT = Wrapping::RepeatWrapping;
            hwRoughMap->anisotropy = 4;
            hwRoughMap->repeat.set( 10, 24 );
            floorMat->roughnessMap = hwRoughMap;
            floorMat->needsUpdate = true;
        }

        cubeMat = MeshStandardMaterial::create();
        cubeMat->roughness = 0.7;
        cubeMat->color = 0xffffff;
        cubeMat->bumpScale = 0.002;
        cubeMat->metalness = 0.2;

        auto bDiffMap = textureLoader.load( resourceDir + "brick_diffuse.jpg");
        if(bDiffMap){
            bDiffMap->wrapS = Wrapping::RepeatWrapping;
            bDiffMap->wrapT = Wrapping::RepeatWrapping;
            bDiffMap->anisotropy = 4;
            bDiffMap->repeat.set( 1, 1 );
            cubeMat->map = bDiffMap;
            cubeMat->needsUpdate = true;
        }

        auto bBumpMap = textureLoader.load( resourceDir + "brick_bump.jpg");
        if(bBumpMap){
            bBumpMap->wrapS = Wrapping::RepeatWrapping;
            bBumpMap->wrapT = Wrapping::RepeatWrapping;
            bBumpMap->anisotropy = 4;
            bBumpMap->repeat.set( 1, 1 );
//            bBumpMap->type = TextureDataType::UnsignedByteType;
//            bBumpMap->format = PixelFormat::RGBFormat;
            cubeMat->bumpMap = bBumpMap;
            cubeMat->needsUpdate = true;
        }

        ballMat = MeshStandardMaterial::create();
        ballMat->color = 0xffffff;
        ballMat->roughness =  0.5;
        ballMat->metalness = 1.0;

        auto eAtmosMap = textureLoader.load(resourceDir + "planets" + fileSeparator + "earth_atmos_2048.jpg");
        if(eAtmosMap){
            eAtmosMap->anisotropy = 4;
            //eAtmosMap->encoding = TextureEncoding::sRGBEncoding;
            ballMat->map = eAtmosMap;
            ballMat->needsUpdate = true;
        }

        auto eSpecMap = textureLoader.load(resourceDir + "planets" + fileSeparator + "earth_specular_2048.jpg");
        if(eSpecMap){
            eSpecMap->anisotropy = 4;
            //eSpecMap->encoding = TextureEncoding::sRGBEncoding;
            ballMat->metalnessMap = eSpecMap;
            ballMat->needsUpdate = true;
        }

        PlaneGeometry::sptr floorGeometry = PlaneGeometry::create( 20, 20 );
        floorGeometry->mergeVertices();
        Mesh::sptr floorMesh = Mesh::create(floorGeometry, floorMat );
        floorMesh->receiveShadow = true;
        floorMesh->rotation.setX(- math_number::PI/2.0);
        scene->add( floorMesh );

        SphereGeometry::sptr ballGeometry = SphereGeometry::create(0.25, 32, 32);
        //Mesh::sptr ballMesh = Mesh::create(ballGeometry, ballMat);
        ballMesh = Mesh::create(ballGeometry, ballMat);
        ballMesh->position.set( 1, 0.375, 1 );
        ballMesh->scale.set(1.5,1.5,1.5);
        ballMesh->rotation.setY(math_number::PI);
        ballMesh->castShadow = true;
        scene->add( ballMesh );

        BoxGeometry::sptr boxGeometry = BoxGeometry::create(0.5, 0.5, 0.5);
        Mesh::sptr boxMesh = Mesh::create(boxGeometry, cubeMat);
        boxMesh->position.set( - 0.5, 0.25, - 1 );
        boxMesh->castShadow = true;
        scene->add( boxMesh );

        Mesh::sptr boxMesh2 = Mesh::create(boxGeometry, cubeMat);
        boxMesh2->position.set( 0, 0.25, - 5 );
        boxMesh2->castShadow = true;
        scene->add( boxMesh2 );

        Mesh::sptr boxMesh3 = Mesh::create(boxGeometry, cubeMat);
        boxMesh3->position.set( 7, 0.25, 0 );
        boxMesh3->castShadow = true;
        scene->add( boxMesh3 );

        // ref for lumens: http://www.power-sure.com/lumens.htm
        bulbLuminousPowers = {
                {"110000 lm (1000W)", 110000},
                {"3500 lm (300W)", 3500},
                {"1700 lm (100W)", 1700},
                {"800 lm (60W)", 800},
                {"400 lm (40W)", 400},
                {"180 lm (25W)", 180},
                {"20 lm (4W)", 20},
                {"Off", 0}
        };
//        for(auto item:bulbLuminousPowers)
//            bulbLuminousPowerNames.push_back(item.first);
//
//        bulbPower = bulbLuminousPowerNames[4];
        bulbPowerSelIdx = 4;

        // ref for solar irradiances: https://en.wikipedia.org/wiki/Lux
        hemiLuminousIrradiances = {
                {"0.0001 lx (Moonless Night)", 0.0001},
                {"0.002 lx (Night Airglow)", 0.002},
                {"0.5 lx (Full Moon)", 0.5},
                {"3.4 lx (City Twilight)", 3.4},
                {"50 lx (Living Room)", 50},
                {"100 lx (Very Overcast)", 100},
                {"350 lx (Office Room)", 350},
                {"400 lx (Sunrise/Sunset)", 400},
                {"1000 lx (Overcast)", 1000},
                {"18000 lx (Daylight)", 18000},
                {"50000 lx (Direct Sun)", 50000}
        };
//        for(auto item:hemiLuminousIrradiances)
//            hemiLuminousIrradianceNames.push_back(item.first);
//
//        hemiIrradiance = hemiLuminousIrradianceNames[0];
        hemiIrradianceSelIdx = 0;

        Vector4 screen = Vector4(0, 0, screenX, screenY);
        //controller = std::make_shared<TrackballControls>(camera, screen);
        std::shared_ptr<TrackballControls> tcontroller = std::make_shared<TrackballControls>(camera, screen);
        controller = tcontroller;

        tcontroller->staticMoving = false;
        tcontroller->rotateSpeed = 4.0f;
        tcontroller->zoomSpeed = 3;
        tcontroller->panSpeed = 3;
        tcontroller->noZoom = true;
        tcontroller->noPan = true;
        tcontroller->noRotate = false;
        tcontroller->dynamicDampingFactor = 0.3f;

    }

    virtual void render() override;

    virtual void showControls() override;

};


#endif //THREE_CPP_GL_LIGHTS_PHYSICAL_H
