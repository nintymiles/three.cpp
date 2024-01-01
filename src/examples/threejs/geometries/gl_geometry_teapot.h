//
// Created by SeanR on 2024/1/1.
//

#ifndef THREE_CPP_GL_GEOMETRY_TEAPOT_H
#define THREE_CPP_GL_GEOMETRY_TEAPOT_H

#include "application_model.h"
#include "orbit_control.h"

#include "material.h"
#include "mesh_lambert_material.h"
#include "mesh_phong_material.h"
#include "mesh_normal_material.h"
#include "mesh_basic_material.h"
#include "mesh_depth_material.h"

#include "mesh.h"
#include "teapot_geometry.h"

#include "ambient_light.h"
#include "point_light.h"

#include "grid_helper.h"

#include "texture_loader.h"
#include "cube_texture_loader.h"

class GLGeometryTeapot : public ApplicationBase{
    PerspectiveCamera::sptr perspectiveCamera;
    OrbitControl::sptr cameraControl;

    AmbientLight::sptr ambientLight;
    Light::sptr light;

    Mesh::sptr teapot;

    CubeTexture::sptr textureCube;
    Texture::sptr textureMap;

    std::vector<Object3D::sptr> objects;

    PointLight::sptr pointLight;

    std::vector<std::string> shadingVec = {"wireframe", "flat", "smooth", "glossy", "textured", "reflective"};
    std::vector<int> tessVec = {2, 3, 4, 5, 6, 8, 10, 15, 20, 30, 40, 50};
    int selTess=8,selShading=0;

    struct EffectController{
        int newTess = 15;
        bool bottom = true;
        bool lid = true;
        bool body = true;
        bool fitLid = true;
        bool nonblinn = true;
        std::string newShading = "textured";
    };
    EffectController effectController{};

    int tess = - 1;	// force initialization
    bool bBottom;
    bool bLid;
    bool bBody;
    bool bFitLid;
    bool bNonBlinn;
    std::string shading;
    const int teapotSize = 300;

    std::map<std::string,Material::sptr> materials{};


public:
    GLGeometryTeapot(int x, int y):ApplicationBase(x,y){}

    virtual void init() override {
        renderer->setClearColor(Color().setHex(0x000000));
        //renderer->physicallyCorrectLights = true;
        renderer->outputEncoding = TextureEncoding::sRGBEncoding;

        auto aspect = (float)screenX / screenY;
        perspectiveCamera = std::make_shared<PerspectiveCamera>(45.f, aspect , 1.f, 80000.f);
        perspectiveCamera->position.set( -600, 550, 1300 );
        //scene->add(camera);

        camera = perspectiveCamera;

        scene = std::make_shared<Scene>();
        scene->setBackgroundColor(Color().set(0xAA0000));


        // Lights
        ambientLight = AmbientLight::create( 0x333333 );
        scene->add( ambientLight );

        light = DirectionalLight::create( 0xFFFFFF, 1.0 );
        light->position.set( 0.32, 0.39, 0.7 );
        scene->add (light);

        std::string rootDir = threecpp::getProjectPath();
        std::string fileSeparator = threecpp::getFileSeparator();
        std::string resourceDir = std::string(rootDir).append(fileSeparator).append("asset").append(fileSeparator)
                .append("textures").append(fileSeparator);
        // TEXTURE MAP
        textureMap = TextureLoader::load( resourceDir + "uv_grid_opengl.jpg" );
        textureMap->wrapS = Wrapping::RepeatWrapping;
        textureMap->wrapT = Wrapping::RepeatWrapping;
        textureMap->anisotropy = 16;
//        textureMap->encoding = TextureEncoding::sRGBEncoding;

        resourceDir = resourceDir.append(fileSeparator).append("cube").append(fileSeparator).append("pisa").append(fileSeparator);
        auto format = ".jpg";
        auto fileurls = {
                resourceDir + "px" + format, resourceDir + "nx" + format,
                resourceDir + "py" + format, resourceDir + "ny" + format,
                resourceDir + "pz" + format, resourceDir + "nz" + format
        };

        textureCube = CubeTextureLoader::load( fileurls );

        cameraControl = std::make_shared<OrbitControl>(perspectiveCamera);
        cameraControl->minDistance = 1000;
        cameraControl->maxDistance = 2400;
        cameraControl->enablePan = true;
        cameraControl->enableDamping = true;
        controller = cameraControl;

        initMaterials();
        //initData();

    }

    virtual void render() override;

    virtual void showControls() override;

private:

    // Whenever the teapot changes, the scene is rebuilt from scratch (not much to it).
    void createNewTeapot() {

        if ( teapot ) {
            //teapot.geometry.dispose();
            scene->remove( teapot );
        }

        auto geometry = TeapotGeometry::create( teapotSize,
                                             tess,
                                             effectController.bottom,
                                             effectController.lid,
                                             effectController.body,
                                             effectController.fitLid,
                                             !effectController.nonblinn );

        teapot = Mesh::create( geometry, materials[ shading ] );

        scene->add( teapot );

    }

    void initMaterials(){
        auto wireframeMat = MeshBasicMaterial::create();
        wireframeMat->wireframe = true;
        materials[ "wireframe" ] = wireframeMat;
        auto phongMat = MeshPhongMaterial::create( 0x000000 );
        phongMat->flatShading = true;
        phongMat->side = Side::DoubleSide;
        materials[ "flat" ] = phongMat;

        auto smoothMat = MeshLambertMaterial::create();
        smoothMat->side = Side::DoubleSide;
        materials[ "smooth" ] =  smoothMat;

        auto phongMat2 = MeshPhongMaterial::create();
        phongMat2->side = Side::DoubleSide;
        materials[ "glossy" ] = phongMat2;

        auto texMat = MeshPhongMaterial::create();
        texMat->side = Side::DoubleSide;
        texMat->map = textureMap;
        materials[ "textured" ] = texMat;

        auto texCubeMat = MeshPhongMaterial::create();
        texCubeMat->side = Side::DoubleSide;
        texCubeMat->envMap = textureCube;
        materials[ "reflective" ] = texCubeMat;

    };

    void initData(){
        tessVec = {2, 3, 4, 5, 6, 8, 10, 15, 20, 30, 40, 50};
        shadingVec = {"wireframe", "flat", "smooth", "glossy", "textured", "reflective"};
    }

};

#endif //THREE_CPP_GL_GEOMETRY_TEAPOT_H
