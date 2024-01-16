//
// Created by SeanR on 2024/1/12.
//

#ifndef THREE_CPP_GL_DEMO_DECALS_H
#define THREE_CPP_GL_DEMO_DECALS_H

#include "application_model.h"

#include "orthographic_camera.h"

#include "orbit_control.h"
#include "directional_light.h"

#include "mesh_phong_material.h"
#include "mesh_normal_material.h"

#include "group.h"
#include "box_geometry.h"

#include "gltf_loader.h"
#include "texture_loader.h"

#include "line.h"
#include "line_basic_material.h"
#include "ray_caster.h"

#include "decal_geometry.h"

#include <thread>
#include <filesystem>


struct IntersectionResult{
    bool intersected;
    Vector3 point;
    Vector3 normal;

};

class GLDemoDecals: public ApplicationBase{
    Line::sptr line;
    std::shared_ptr<Raycaster> raycaster;
    std::vector<std::shared_ptr<IntersectionData>> intersects;

    Object3D::sptr lerrySmeithModel;
    Mesh::sptr mesh;
    Vector2 mouse;
    Mesh::sptr mouseHelper;

    IntersectionResult intersection{};

    Euler orientation;
    Vector3 position;
    Vector3 size{10,10,10};

    Material::sptr decalMaterial;
    std::vector<Mesh::sptr> decals;

    struct Parameter{
        int minScale = 10,maxScale =20;
        bool rotate = true;
    };
    Parameter params{};

public:
    GLDemoDecals(int x, int y):ApplicationBase(x,y){}

    virtual void init() override {
        renderer->setClearColor(Color().setHex(0x000000));
        renderer->shadowMap->enabled = true;
        //renderer->shadowMap->type = ShadowMapType::PCFSoftShadowMap;

        auto height = 100;
        auto aspect = (float)screenX / screenY;
        //camera = OrthographicCamera::create( - height * aspect, height * aspect, height, - height, 1000, 2500 );
        camera = std::make_shared<PerspectiveCamera>(45.0f, aspect , 1.f, 1000.0f);
        camera->position.set(0.0f, 0.0f, 120.0f);
        isPerspective = true;

        scene = std::make_shared<Scene>();
        scene->setBackgroundColor(Color().set(0x000000));

        auto ambientLight = AmbientLight::create(Color().setHex(0x443333),3.f);
        scene->add(ambientLight);

        auto dirLight1 = DirectionalLight::create( 0xffddcc, 1 );
        dirLight1->position.set( 1, 0.75, 0.5 );
        scene->add( dirLight1 );

        auto dirLight2 = DirectionalLight::create( 0xccccff, 1 );
        dirLight2->position.set( - 1, 0.75, - 0.5 );
        scene->add( dirLight2 );

        auto pointLight = PointLight::create(Color().setHex(0xffffff),2.0f);
        pointLight->position.set(0, 20, 50);
        pointLight->castShadow = true;
        scene->add(pointLight);

        auto geometry = BufferGeometry::create();
        geometry->setFromPoints({Vector3(), Vector3()} );

        line = Line::create( geometry, LineBasicMaterial::create() );
        scene->add( line );

        loadLeePerrySmith();

        Vector3 origin;
        Vector3 direction;
        raycaster = std::make_shared<Raycaster>(origin,direction);

        mouseHelper = Mesh::create( BoxGeometry::create( 1, 1, 10 ), MeshNormalMaterial::create() );
        mouseHelper->visible = true;
        scene->add( mouseHelper );

        std::shared_ptr<OrbitControl> oController = std::make_shared<OrbitControl>(camera);
        controller = oController;

        oController->rotateSpeed = 4.0f;
        oController->zoomSpeed = 3;
        oController->panSpeed = 3;
        oController->minDistance = 50;
        oController->maxDistance = 200;
//        tcontroller->enableZoom = false;
//        tcontroller->enablePan = false;
//        tcontroller->enableRotate = false;
        oController->dampingFactor = 0.3f;

        std::string dir = threecpp::getProjectPath();

        std::string relPath = threecpp::getFileSeparator().append("asset").append(threecpp::getFileSeparator()).append("textures")
                .append(threecpp::getFileSeparator()).append("decal").append(threecpp::getFileSeparator());

        Texture::sptr decalDiffuse = TextureLoader::load(dir + relPath +"decal-diffuse.png");
        Texture::sptr decalNormal = TextureLoader::load(dir + relPath +"decal-normal.jpg");

        decalMaterial = MeshPhongMaterial::create();
        decalMaterial->specular = 0x444444;
        decalMaterial->map = decalDiffuse;
        decalMaterial->normalMap = decalNormal;
        decalMaterial->normalScale = Vector2( 1, 1 );
        decalMaterial->shininess = 30;
        decalMaterial->transparent = true;
        decalMaterial->depthTest = true;
        decalMaterial->depthWrite = false;
        decalMaterial->polygonOffset = true;
        decalMaterial->polygonOffsetFactor = - 4;
        decalMaterial->wireframe = false;


    }

    virtual void render() override;

private:
    void loadLeePerrySmith(){
        std::string dir = threecpp::getProjectPath();

        GLTFLoader gltfLoader{};
        std::string relPath = threecpp::getFileSeparator().append("asset").append(threecpp::getFileSeparator()).append("models")
                .append(threecpp::getFileSeparator()).append("gltf").append(threecpp::getFileSeparator())
                .append("LeePerrySmith").append(threecpp::getFileSeparator());
        Group::sptr gltfGroup = gltfLoader.load(dir + relPath +"LeePerrySmith.glb");

        Texture::sptr map = TextureLoader::load(dir + relPath +"Map-COL.jpg");
        Texture::sptr specularMap = TextureLoader::load(dir + relPath +"Map-SPEC.jpg");
        Texture::sptr normalMap = TextureLoader::load(dir + relPath +"MInfinite-Level_02_Tangent_SmoothUV.jpg");

        Material::sptr material = MeshPhongMaterial::create();
        material->specular = 0x111111;
        material->map = map;
        material->specularMap = specularMap;
        material->normalMap = normalMap;
        material->shininess = 25.f;

        //Mesh::sptr lerrySmeithMesh = Mesh::create();


        lerrySmeithModel = gltfGroup->children[0]->children[0];
        lerrySmeithModel->material = material;
        mesh = std::dynamic_pointer_cast<Mesh>(lerrySmeithModel);
        mesh->scale.multiplyScalar(10);
//        gltfGroup->position.setY(-100);
//        gltfGroup->rotation.y(-math_number::PI/2);
        scene->add(mesh);
    }

    void checkIntersection( float x, float y ) {

        if ( !lerrySmeithModel ) return;

        mouse.x = ( (float)x * 2.0f / screenX ) * 2 - 1;
        mouse.y = - ( (float)y * 2.0f / screenY ) * 2 + 1;

        std::cout << "normalized mouse Pos.x = "<< mouse.x << " mouse Pos.y = " << mouse.y << std::endl;

        raycaster->setFromCamera( mouse, camera );
        auto intersects = raycaster->intersectObject( lerrySmeithModel, false );

        if ( intersects.size() > 0 ) {

            auto p = intersects[ 0 ]->point;
            mouseHelper->position.copy( p );
            intersection.point.copy( p );

            auto n = intersects[ 0 ]->face.normal.clone();
            n.transformDirection( lerrySmeithModel->matrixWorld );
            n.multiplyScalar( 10 );
            n.add( intersects[ 0 ]->point );

            intersection.normal.copy( intersects[ 0 ]->face.normal );
            mouseHelper->lookAt( n );

            auto lineBG = std::dynamic_pointer_cast<BufferGeometry>(line->geometry);
            auto positions = lineBG->getAttribute(AttributeName::position);
            positions->setXYZ( 0, p.x, p.y, p.z );
            positions->setXYZ( 1, n.x, n.y, n.z );
            positions->setNeedsUpdate(true);

            intersection.intersected = true;

            //intersects.length = 0;

        } else {

            intersection.intersected = false;
        }

    }

    void shoot() {

        position.copy( intersection.point );
        orientation.copy( mouseHelper->rotation );

        if ( params.rotate ) orientation.z(math::random() * 2 * math_number::PI);

        auto scale = params.minScale + math::random() * ( params.maxScale - params.minScale );
        size.set( scale, scale, scale );

        auto material = decalMaterial;//->cloneShared();
        material->color.setHex( math::random() * 0xffffff );

        BufferGeometry::sptr geometry = DecalGeometry::create( mesh, position, orientation, size );
        auto m = Mesh::create( geometry, material );

        decals.push_back( m );
        scene->add( m );

    }

    void removeDecals() {
        for(auto& d:decals){
            scene->remove( d );
        }

        decals.clear();
    }


};

#endif //THREE_CPP_GL_DEMO_DECALS_H
