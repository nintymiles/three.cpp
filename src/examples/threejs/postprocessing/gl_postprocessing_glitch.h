//
// Created by SeanR on 1/3/2024.
//

#ifndef THREE_CPP_GL_POSTPROCESSING_GLITCH_H
#define THREE_CPP_GL_POSTPROCESSING_GLITCH_H

#include "application_model.h"

#include "perspective_camera.h"
#include "effect_composer.h"
#include "orbit_control.h"

#include "sphere_geometry.h"
#include "box_geometry.h"
#include "shader_material.h"
#include "mesh_phong_material.h"

#include "ambient_light.h"
#include "directional_light.h"

#include "render_pass.h"
#include "shader_pass.h"
#include "shader_data.h"

#include "glitch_pass.h"

#include "group.h"
#include "timer.h"

#include <variant>
#include <unordered_map>


class GLPostProcessingGlitch: public ApplicationBase{
    OrbitControl::sptr orbitControl;
    UniformValues::sptr uniforms;
    Timer timer;

    Object3D::sptr object;
    Light::sptr light;

    std::shared_ptr<threecpp::EffectComposer> composer;
    std::shared_ptr<threecpp::GlitchPass> glitchPass;

    bool wildGlitch = false;


public:
    GLPostProcessingGlitch(int x, int y): ApplicationBase(x, y){}

    virtual void init() override {
        renderer->setClearColor(Color().setHex(0x000000));
        renderer->outputEncoding = TextureEncoding::sRGBEncoding;

        auto aspect = (float)screenX / screenY;
        camera = std::make_shared<PerspectiveCamera>( 70, aspect, 1, 1000 );
        camera->position.set( 0, 0, 400 );

        scene = std::make_shared<Scene>();
        scene->setBackgroundColor(Color().set(0x444444));
        scene->fog = Fog::create( 0x000000, 1, 1000 );

        object = Object3D::create();
        scene->add( object );

        auto geometry = SphereGeometry::create( 1, 4, 4 );

        for ( int i = 0; i < 100; i ++ ) {
            auto material = MeshPhongMaterial::create( 0xffffff * math::random() );
            material->flatShading = true;

            auto mesh = Mesh::create( geometry, material );
            mesh->position.set( math::random() - 0.5, math::random() - 0.5, math::random() - 0.5 ).normalize();
            mesh->position.multiplyScalar( math::random() * 400 );
            mesh->rotation.set( math::random() * 2, math::random() * 2, math::random() * 2 );
            mesh->scale = Vector3(math::random() * 50);
            object->add( mesh );

        }

        scene->add( AmbientLight::create( 0x222222 ) );

        light = DirectionalLight::create( 0xffffff );
        light->position.set( 1, 1, 1 );
        scene->add( light );

        // postprocessing
        composer = std::make_shared<threecpp::EffectComposer>( renderer, renderer->getRenderTarget() );
        composer->addPass( std::make_shared<threecpp::RenderPass>( scene, camera ) );

        glitchPass = std::make_shared<threecpp::GlitchPass>();
        composer->addPass( glitchPass );

        orbitControl = std::make_shared<OrbitControl>(camera);
        orbitControl->target.set( 0, 0, 0 );
        orbitControl->update();
        orbitControl->enablePan = true;
        orbitControl->enableDamping = true;
        controller = orbitControl;

        timer = Timer();


    }

    virtual void render() override;

    virtual void showControls() override;
};

#endif //THREE_CPP_GL_POSTPROCESSING_GLITCH_H
