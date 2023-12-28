//
// Created by SeanR on 12/28/2023.
//

#ifndef THREE_CPP_GL_POSTPROCESSING_AFTER_IMAGE_H
#define THREE_CPP_GL_POSTPROCESSING_AFTER_IMAGE_H


#include "application_model.h"

#include "perspective_camera.h"
#include "effect_composer.h"
#include "orbit_control.h"

#include "box_geometry.h"
#include "shader_material.h"
#include "mesh_normal_material.h"

#include "ambient_light.h"
#include "directional_light.h"

#include "render_pass.h"
#include "shader_pass.h"
#include "shader_data.h"
#include "after_image_pass.h"

#include "group.h"
#include "timer.h"

#include <variant>
#include <unordered_map>


class GLPostProcessingAfterImage: public ApplicationBase{
    OrbitControl::sptr orbitControl;
    UniformValues::sptr uniforms;
    Timer timer;
    Mesh::sptr mesh;

    std::shared_ptr<threecpp::ShaderPass> effectSobel;

    std::shared_ptr<threecpp::EffectComposer> composer;

    bool enabled = true;
    float damp = 0.96;

public:
    GLPostProcessingAfterImage(int x, int y): ApplicationBase(x, y){}

    virtual void init() override {
        renderer->setClearColor(Color().setHex(0x000000));
        renderer->outputEncoding = TextureEncoding::sRGBEncoding;

        auto aspect = (float)screenX / screenY;
        camera = std::make_shared<PerspectiveCamera>( 70.f, aspect, 1.f, 1000.f );
        camera->position.set( 0, 0, 400 );
        //camera->lookAt(scene->position);

        scene = std::make_shared<Scene>();
        //scene->setBackgroundColor(Color().set(0x444444));

        auto geometry = BoxGeometry::create( 150, 150, 150, 2, 2, 2 );
        auto material = MeshNormalMaterial::create();
        mesh = Mesh::create( geometry, material );
        scene->add( mesh );

        //
        auto ambientLight = AmbientLight::create( 0xcccccc, 0.4 );
        scene->add( ambientLight );

        auto pointLight = PointLight::create( 0xffffff, 0.8 );
        pointLight->position.set( 0, 10, 25 );
        scene->add( pointLight );
        //scene->add( camera );

        // postprocessing
        composer = std::make_shared<threecpp::EffectComposer>( renderer, renderer->getRenderTarget() );
        auto renderPass = std::make_shared<threecpp::RenderPass>( scene, camera );
        composer->addPass( renderPass );

        auto afterimagePass = std::make_shared<threecpp::AfterImagePass>(screenX,screenY);
        composer->addPass( afterimagePass );
        uniforms = afterimagePass->uniforms;

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


#endif //THREE_CPP_GL_POSTPROCESSING_AFTER_IMAGE_H
