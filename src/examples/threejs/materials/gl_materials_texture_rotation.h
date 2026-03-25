//
// Created by SeanR on 03/25/2026.
//

#ifndef THREE_CPP_GL_MATERIALS_TEXTURES_ROTATION_H
#define THREE_CPP_GL_MATERIALS_TEXTURES_ROTATION_H

#include "application_model.h"
#include "orbit_control.h"
#include "trackball_control.h"
#include "texture_loader.h"
#include "obj_loader.h"

#include "box_geometry.h"
#include "ambient_light.h"
#include "point_light.h"

#include "mesh_basic_material.h"
#include "mesh_depth_material.h"
#include "mesh_normal_material.h"
#include "mesh.h"


#include <thread>

class GLMaterialsTextureRotation: public ApplicationBase{
    PerspectiveCamera::sptr perspectiveCamera;
    OrbitControl::sptr pCameraControl;
    Mesh::sptr mesh;


    struct APIData{
        float offsetX=0,offsetY=0,repeatX=.25,repeatY=.25,rotation = math_number::PI/4,centerX=.5,centerY=.5;
    };

    APIData API{};

    void updateUvTransform(){
        auto texture = mesh->material->map;

        if ( texture->matrixAutoUpdate == true ) {

            texture->offset.set( API.offsetX, API.offsetY );
            texture->repeat.set( API.repeatX, API.repeatY );
            texture->center.set( API.centerX, API.centerY );
            texture->rotation = API.rotation; // rotation is around [ 0.5, 0.5 ]

        } else {
            // one way...
            //texture.matrix.setUvTransform( API.offsetX, API.offsetY, API.repeatX, API.repeatY, API.rotation, API.centerX, API.centerY );

            // another way...
            texture->matrix
                    .identity()
                    .translate( - API.centerX, - API.centerY )
                    .rotate( API.rotation )					// I don't understand how rotation can preceed scale, but it seems to be required...
                    .scale( API.repeatX, API.repeatY )
                    .translate( API.centerX, API.centerY )
                    .translate( API.offsetX, API.offsetY );

        }

    }

public:
    GLMaterialsTextureRotation(int x, int y):ApplicationBase(x,y){}

    virtual void init() override {
        renderer->setClearColor(Color().setHex(0x000000));
        //renderer->physicallyCorrectLights = true;
        renderer->outputEncoding = TextureEncoding::sRGBEncoding;
        renderer->shadowMap->enabled = true;
        renderer->shadowMap->type = ShadowMapType::PCFSoftShadowMap;
        renderer->toneMapping = ToneMapping::ACESFilmicToneMapping;
        //renderer->setAnimationLoop( render );

        auto aspect = (float)screenX / screenY;
        perspectiveCamera = std::make_shared<PerspectiveCamera>(40.f, aspect , 1.f, 1000.f);
        perspectiveCamera->position.set( 10, 15, 25 );

        camera = perspectiveCamera;
//scene->add(camera);


        scene = std::make_shared<Scene>();
//        scene->setBackgroundColor(Color().set(0x000000));

        auto geometry = BoxGeometry::create(10,10,10);

        std::string rootDir = threecpp::getProjectPath();
        std::string fileSeparator = threecpp::getFileSeparator();
        std::string resourceDir = rootDir.append(fileSeparator).append("asset").append(fileSeparator)
                .append("textures").append(fileSeparator);

        // textures
        auto textureLoader = TextureLoader();
        auto uvMap = textureLoader.load( resourceDir + "uv_grid_opengl.jpg" );
        uvMap->wrapS = uvMap->wrapT = Wrapping::RepeatWrapping;
        //todo:fix this;
        uvMap->anisotropy = renderer->capabilities->getMaxAnisotropy();

        // material
        MeshNormalMaterial::sptr material = MeshBasicMaterial::create();
        material->map=uvMap;
        mesh = Mesh::create(geometry,material);

        scene->add(mesh);


        pCameraControl = std::make_shared<OrbitControl>(perspectiveCamera);
        pCameraControl->minDistance = 20;
        pCameraControl->maxDistance = 50;
        pCameraControl->enablePan = true;
        pCameraControl->enableDamping = true;
        pCameraControl->maxPolarAngle = math_number::PI/2;
        controller = pCameraControl;



        updateUvTransform();

    }

    virtual void render() override;

    virtual void showControls() override;
};


#endif //THREE_CPP_GL_MATERIALS_TEXTURES_ROTATION_H
