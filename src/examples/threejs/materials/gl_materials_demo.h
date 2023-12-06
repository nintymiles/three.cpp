//
// Created by SeanR on 2023/12/6.
//

#ifndef THREE_CPP_GL_MATERIALS_DEMO_H
#define THREE_CPP_GL_MATERIALS_DEMO_H

#include "application_model.h"
#include "orbit_control.h"

#include "material.h"
#include "mesh_lambert_material.h"
#include "mesh_phong_material.h"
#include "mesh_normal_material.h"
#include "mesh_basic_material.h"
#include "mesh_depth_material.h"

#include "mesh.h"
#include "sphere_geometry.h"

#include "ambient_light.h"
#include "point_light.h"

#include "grid_helper.h"

class GLMaterialsDemo : public ApplicationBase{
    PerspectiveCamera::sptr perspectiveCamera;
    OrbitControl::sptr pCameraControl,oCameraControl;
    Mesh::sptr headMesh;

    std::vector<Material::sptr> materials;
    std::vector<Object3D::sptr> objects;

    PointLight::sptr pointLight;

    std::vector<std::string> materialVec,cameraVec,controlVec,sideVec;
    int selMaterial=1,selCamera=0,selSide=0;
    const float SCALE = 2.436143; // from original model
    const float BIAS = - 0.428408; // from original model

    void initMaterials(){
        // Materials
//        const texture = new THREE.Texture( generateTexture() );
//        texture.needsUpdate = true;

        MeshLambertMaterial::sptr transLamMaterial = MeshLambertMaterial::create();
        transLamMaterial->transparent = true;
//        transLamMaterial->map = texutre;
        materials.push_back( transLamMaterial );

        materials.push_back( MeshLambertMaterial::create(0xdddddd) );

        MeshPhongMaterial::sptr phoneMaterial = MeshPhongMaterial::create();
        phoneMaterial->color = 0xdddddd;
        phoneMaterial->specular = 0x009900;
        phoneMaterial->shininess = 30;
        phoneMaterial->flatShading = true;
        materials.push_back( phoneMaterial );

        materials.push_back( MeshNormalMaterial::create() );

        MeshBasicMaterial::sptr basicMaterial = MeshBasicMaterial::create();
        basicMaterial->color = 0xffaa00;
        basicMaterial->transparent = true;
        basicMaterial->blending = Blending::AdditiveBlending;
        materials.push_back( basicMaterial );

        materials.push_back( MeshLambertMaterial::create( 0xdddddd ) );

        MeshPhongMaterial::sptr phoneMaterial2 = MeshPhongMaterial::create();
        phoneMaterial2->color = 0xdddddd;
        phoneMaterial2->specular = 0x009900;
        phoneMaterial2->shininess = 30;
        //phoneMaterial2->map = texture;
        phoneMaterial2->transparent = true;
        materials.push_back( phoneMaterial2 );

        MeshNormalMaterial::sptr normalMaterial = MeshNormalMaterial::create();
        normalMaterial->flatShading = true;
        materials.push_back( normalMaterial );

        MeshBasicMaterial::sptr basicMaterial2 = MeshBasicMaterial::create(0xffaa00);
        basicMaterial2->wireframe = true;
        materials.push_back( basicMaterial2 );

        materials.push_back( MeshDepthMaterial::create() );

        MeshLambertMaterial::sptr lambertMaterial = MeshLambertMaterial::create(0x666666);
        lambertMaterial->emissive = 0xff0000;
        materials.push_back( lambertMaterial );

        MeshPhongMaterial::sptr phoneMaterial3 = MeshPhongMaterial::create();
        phoneMaterial3->color = 0x000000;
        phoneMaterial3->specular = 0x666666;
        phoneMaterial3->emissive = 0xff0000;
        phoneMaterial3->shininess = 10;
        phoneMaterial3->opacity = 0.9f;
        phoneMaterial3->transparent = true;
        materials.push_back( phoneMaterial3 );

        MeshBasicMaterial::sptr basicMaterial3 = MeshBasicMaterial::create(0xffaa00);
        basicMaterial3->transparent = true;
        materials.push_back( basicMaterial3 );
    }

    void addMesh( BufferGeometry::sptr geometry, Material::sptr material ) {
        auto mesh = Mesh::create( geometry, material );

        mesh->position.x = ( objects.size() % 4 ) * 200.0f - 400;
        mesh->position.z = math::floor( objects.size() / 4 ) * 200.0f - 200;

        mesh->rotation.setX( math::random() * 200.f - 100 );
        mesh->rotation.setY( math::random() * 200.f - 100 );
        mesh->rotation.setZ( math::random() * 200.f - 100 );

        objects.push_back( mesh );

        scene->add( mesh );
    }

public:
    GLMaterialsDemo(int x, int y):ApplicationBase(x,y){}

    virtual void init() override {
        renderer->setClearColor(Color().setHex(0x000000));
        //renderer->physicallyCorrectLights = true;
        renderer->outputEncoding = TextureEncoding::sRGBEncoding;
        renderer->shadowMap->enabled = true;
        renderer->shadowMap->type = ShadowMapType::PCFSoftShadowMap;
        renderer->toneMapping = ToneMapping::ACESFilmicToneMapping;
        //renderer->setAnimationLoop( render );

        auto aspect = (float)screenX / screenY;
        perspectiveCamera = std::make_shared<PerspectiveCamera>(45.f, aspect , 1.f, 2000.f);
        perspectiveCamera->position.set( 0, 200, 800 );
        //scene->add(camera);

        camera = perspectiveCamera;

        scene = std::make_shared<Scene>();
        scene->setBackgroundColor(Color().set(0x000000));

        // Grid
        auto grid = GridHelper::create( 1000, 40, 0x303030, 0x303030 );
        grid->position.y = - 75;
        scene->add( grid );

        initMaterials();

        // Spheres geometry
        auto geometry = SphereGeometry::create( 70, 32, 16 );

        for ( int i = 0, l = materials.size(); i < l; i ++ ) {
            addMesh( geometry, materials[ i ] );
        }

        // Lights
        scene->add( AmbientLight::create( 0x111111,10 ) );

        auto directionalLight = DirectionalLight::create( 0xffffff, 0.125 );

        directionalLight->position.x = math::random() - 0.5;
        directionalLight->position.y = math::random() - 0.5;
        directionalLight->position.z = math::random() - 0.5;
        directionalLight->position.normalize();

        scene->add( directionalLight );

        pointLight = PointLight::create( 0xffffff, 1 );
        scene->add( pointLight );

        pointLight->add( Mesh::create( SphereGeometry::create( 4, 8, 8 ), MeshBasicMaterial::create( 0xffffff ) ) );

        pCameraControl = std::make_shared<OrbitControl>(perspectiveCamera);
        pCameraControl->minDistance = 1000;
        pCameraControl->maxDistance = 2400;
        pCameraControl->enablePan = true;
        pCameraControl->enableDamping = true;
        controller = pCameraControl;


        //initComboData();

    }

    virtual void render() override;

    virtual void showControls() override;

};


#endif //THREE_CPP_GL_MATERIALS_DEMO_H
