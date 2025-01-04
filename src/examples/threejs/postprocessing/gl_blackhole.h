//
// Created by SeanR on 2023/12/19.
//

#ifndef THREE_CPP_GL_BLACKHOLE_H
#define THREE_CPP_GL_BLACKHOLE_H

#include "application_model.h"

#include "perspective_camera.h"
#include "effect_composer.h"
#include "orbit_control.h"

#include "plane_geometry.h"
#include "mesh_phong_material.h"
#include "black_hole_shader_data.h"

#include "ambient_light.h"
#include "directional_light.h"

#include "render_pass.h"
#include "shader_pass.h"
#include "shader_data.h"


class GLBlackHole: public ApplicationBase{
    OrbitControl::sptr orbitControl;
    UniformValues::sptr uniforms;
    Timer timer;

    std::shared_ptr<threecpp::EffectComposer> composer;
    Object3D::sptr object;

public:
    GLBlackHole(int x, int y): ApplicationBase(x, y){}

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

        auto geometry = PlaneGeometry::create( 2, 2 );

        ShaderMaterial::sptr shaderMaterial = BlackholeShaderData::blackHoleMainShader();
        Mesh::sptr planeMesh = Mesh::create( geometry, shaderMaterial );
        scene->add( planeMesh );

        scene->add( AmbientLight::create( 0x222222 ) );

        auto light = DirectionalLight::create( 0xffffff );
        light->position.set( 1, 1, 1 );
        scene->add( light );

        // postprocessing
        composer = std::make_shared<threecpp::EffectComposer>( renderer, renderer->getRenderTarget() );
        auto renderPass = std::make_shared<threecpp::RenderPass>( scene, camera );
        composer->addPass( renderPass );

//        auto effect1 = std::make_shared<threecpp::ShaderPass>( ShaderData::getDotScreenShader() );
//        effect1->uniforms->set("scale",4.f);
//        composer->addPass( effect1 );
//
//        auto effect2 = std::make_shared<threecpp::ShaderPass>( ShaderData::getRGBShiftShader() );
//        effect2->uniforms->set("amount",0.0015f);
//        composer->addPass( effect2 );

        orbitControl = std::make_shared<OrbitControl>(camera);
        orbitControl->enablePan = true;
        orbitControl->enableDamping = true;
        controller = orbitControl;


    }

    virtual void render() override;

    virtual void showControls() override;
};


#endif //THREE_CPP_GL_BLACKHOLE_H
