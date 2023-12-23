//
// Created by SeanR on 12/23/2023.
//

#ifndef THREE_CPP_GL_POSTPROCESSING_SOBEL_H
#define THREE_CPP_GL_POSTPROCESSING_SOBEL_H


#include "application_model.h"

#include "perspective_camera.h"
#include "effect_composer.h"
#include "orbit_control.h"

#include "torus_knot_geometry.h"
#include "shader_material.h"
#include "mesh_phong_material.h"

#include "ambient_light.h"
#include "directional_light.h"

#include "render_pass.h"
#include "shader_pass.h"
#include "shader_data.h"
#include "half_tone_pass.h"

#include "group.h"
#include "timer.h"

#include <variant>
#include <unordered_map>


class GLPostProcessingSobel: public ApplicationBase{
    OrbitControl::sptr orbitControl;
    UniformValues::sptr uniforms;
    Timer timer;

    std::shared_ptr<threecpp::ShaderPass> effectSobel;

    std::shared_ptr<threecpp::EffectComposer> composer;

    bool postprocessing = true;

public:
    GLPostProcessingSobel(int x, int y): ApplicationBase(x, y){}

    virtual void init() override {
        renderer->setClearColor(Color().setHex(0x000000));
        renderer->outputEncoding = TextureEncoding::sRGBEncoding;

        auto aspect = (float)screenX / screenY;
        camera = std::make_shared<PerspectiveCamera>( 70.f, aspect, .1f, 200.f );
        camera->position.set( 0, 10, 25 );
        //camera->lookAt(scene->position);

        scene = std::make_shared<Scene>();
        //scene->setBackgroundColor(Color().set(0x444444));

        auto geometry = TorusKnotGeometry::create( 8, 3, 256, 32, 2, 3 );
        auto material = MeshPhongMaterial::create( 0xffff00 );

        auto mesh = Mesh::create( geometry, material );
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

        // color to grayscale conversion
        auto effectGrayScale = std::make_shared<threecpp::ShaderPass>( ShaderData::getLuminosityShader() );
        composer->addPass( effectGrayScale );

        // you might want to use a gaussian blur filter before
        // the next pass to improve the result of the Sobel operator

        // Sobel operator
        effectSobel = std::make_shared<threecpp::ShaderPass>( ShaderData::getSobelShader() );
        effectSobel->uniforms->set("resolution",Vector2(screenX,screenY));
        composer->addPass( effectSobel );

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


#endif //THREE_CPP_GL_POSTPROCESSING_SOBEL_H
