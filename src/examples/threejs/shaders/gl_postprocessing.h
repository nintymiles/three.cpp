//
// Created by SeanR on 2023/12/19.
//

#ifndef THREE_CPP_GL_POSTPROCESSING_H
#define THREE_CPP_GL_POSTPROCESSING_H

#include "application_model.h"

#include "perspective_camera.h"
#include "effect_composer.h"
#include "orbit_control.h"

#include "sphere_geometry.h"
#include "mesh_phong_material.h"

#include "ambient_light.h"
#include "directional_light.h"

#include "render_pass.h"
#include "shader_pass.h"
#include "shader_data.h"


class GLPostprocessing: public ApplicationBase{
    OrbitControl::sptr pCameraControl;
    UniformValues::sptr uniforms;
    Timer timer;

    std::shared_ptr<threecpp::EffectComposer> composer;
    Object3D::sptr object;

public:
    GLPostprocessing(int x, int y): ApplicationBase(x, y){}

    virtual void init() override {
        renderer->setClearColor(Color().setHex(0x000000));
        renderer->outputEncoding = TextureEncoding::sRGBEncoding;

        auto aspect = (float)screenX / screenY;
        camera = std::make_shared<PerspectiveCamera>( 70, aspect, 1, 1000 );
        camera->position.set( 0, 0, 400 );


        scene = std::make_shared<Scene>();
        scene->setBackgroundColor(Color().set(0x000000));
        scene->fog = Fog::create( 0x000000, 1, 1000 );

        object = Object3D::create();
        scene->add( object );

        auto geometry = SphereGeometry::create( 1, 4, 4 );
        auto material = MeshPhongMaterial::create(0xffffff );
        material->flatShading = true;

        for ( int i = 0; i < 100; i ++ ) {
            auto mesh = Mesh::create( geometry, material );
            mesh->position.set( math::random() - 0.5, math::random() - 0.5, math::random() - 0.5 ).normalize();
            mesh->position.multiplyScalar( math::random() * 400 );
            mesh->rotation.set( math::random() * 2, math::random() * 2, math::random() * 2 );
            mesh->scale.set(math::random() * 50,math::random() * 50,math::random() * 50);
            object->add( mesh );

        }

        scene->add( AmbientLight::create( 0x222222 ) );

        auto light = DirectionalLight::create( 0xffffff );
        light->position.set( 1, 1, 1 );
        scene->add( light );

        // postprocessing
        composer = std::make_shared<threecpp::EffectComposer>( renderer, renderer->getRenderTarget() );
        auto renderPass = std::make_shared<threecpp::RenderPass>( scene, camera );
        composer->addPass( renderPass );

        auto effect1 = std::make_shared<threecpp::ShaderPass>( ShaderData::getDotScreenShader() );
        effect1->uniforms->set("scale",4.f);
        composer->addPass( effect1 );

        auto effect2 = std::make_shared<threecpp::ShaderPass>( ShaderData::getRGBShiftShader() );
        effect2->uniforms->set("amount",0.0015f);
        composer->addPass( effect2 );


    }

    virtual void render() override;

    virtual void showControls() override;
};


#endif //THREE_CPP_GL_POSTPROCESSING_H
