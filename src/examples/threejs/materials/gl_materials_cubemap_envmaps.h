//
// Created by SeanR on 11/29/2023.
//

#ifndef THREE_CPP_GL_MATERIALS_ENVMAPS_H
#define THREE_CPP_GL_MATERIALS_ENVMAPS_H

#include "application_model.h"
#include "orbit_control.h"
#include "cube_texture_loader.h"
#include "obj_loader.h"

#include "ambient_light.h"
#include "point_light.h"

#include "icosahedron_geometry.h"
#include "sphere_geometry.h"

#include "mesh_standard_material.h"
#include "mesh_depth_material.h"
#include "mesh_normal_material.h"
#include "mesh_lambert_material.h"
#include "mesh.h"

#include <thread>

class GLMaterialsCubeMapEnvmaps: public ApplicationBase{
    PerspectiveCamera::sptr perspectiveCamera;
    OrbitControl::sptr pCameraControl;

public:
    GLMaterialsCubeMapEnvmaps(int x, int y): ApplicationBase(x, y){}

    virtual void init() override {
        renderer->setClearColor(Color().setHex(0x000000));
        //renderer->physicallyCorrectLights = true;
        renderer->outputEncoding = TextureEncoding::sRGBEncoding;
//        renderer->shadowMap->enabled = true;
//        renderer->shadowMap->type = ShadowMapType::PCFSoftShadowMap;
//        renderer->toneMapping = ToneMapping::ACESFilmicToneMapping;
        //renderer->setAnimationLoop( render );

        auto aspect = (float)screenX / screenY;
        perspectiveCamera = std::make_shared<PerspectiveCamera>(70.f, aspect , 1.f, 100000.f);
        perspectiveCamera->position.set( 0, 0, 1000 );
        //scene->add(camera);

        camera = perspectiveCamera;

        scene = std::make_shared<Scene>();
        scene->setBackgroundColor(Color().set(0x000000));

        std::string rootDir = threecpp::getProjectPath();
        std::string fileSeparator = threecpp::getFileSeparator();
        std::string resourceDir = std::string(rootDir).append(fileSeparator).append("asset").append(fileSeparator)
                .append("textures").append(fileSeparator)
                .append("cube").append(fileSeparator)
                .append("Bridge2").append(fileSeparator);

        auto format = ".jpg";
        auto fileurls = {
                resourceDir + "posx" + format, resourceDir + "negx" + format,
                resourceDir + "posy" + format, resourceDir + "negy" + format,
                resourceDir + "posz" + format, resourceDir + "negz" + format
        };

        auto texCube = CubeTextureLoader::load( fileurls );
        texCube->setNeedsUpdate(true);
        texCube->format = PixelFormat::RGBFormat;

        resourceDir = std::string(rootDir).append(fileSeparator).append("asset").append(fileSeparator)
                .append("textures").append(fileSeparator);
        auto textureEquirec = TextureLoader::load(resourceDir + "2294472375_24a3b8ef46_o.jpg" );
        textureEquirec->mapping = TextureMapping::EquirectangularReflectionMapping;
        //textureEquirec->encoding = TextureEncoding::sRGBEncoding;

//        scene->setBackgroundCubeTexture(texCube);
        scene->setBackgroundTexture(textureEquirec);

        //
        auto geometry = IcosahedronBufferGeometry::create( 400, 5 );
        //auto geometry = SphereGeometry::create( 400, 15 );
        Material::sptr sphereMaterial = MeshBasicMaterial::create();
        sphereMaterial->envMap = texCube;
//        sphereMaterial->envMap = textureEquirec;

        Mesh::sptr sphereMesh = Mesh::create( geometry, sphereMaterial );
        scene->add( sphereMesh );

        pCameraControl = std::make_shared<OrbitControl>(perspectiveCamera);
        pCameraControl->minDistance = 500;
        pCameraControl->maxDistance = 2500;
        pCameraControl->enablePan = true;
        pCameraControl->enableDamping = true;
        controller = pCameraControl;


    }

    virtual void render() override;

    virtual void showControls() override;
};


#endif //THREE_CPP_GL_MATERIALS_ENVMAPS_H
