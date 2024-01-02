//
// Created by SeanR on 2023/12/30.
//

#ifndef THREE_CPP_GL_POSTPROCESSING_MASKING_H
#define THREE_CPP_GL_POSTPROCESSING_MASKING_H

#include "application_model.h"

#include "perspective_camera.h"
#include "effect_composer.h"
#include "orbit_control.h"

#include "box_geometry.h"
#include "torus_geometry.h"
#include "shader_material.h"
#include "mesh_normal_material.h"

#include "ambient_light.h"
#include "directional_light.h"

#include "render_pass.h"
#include "shader_pass.h"
#include "shader_data.h"
#include "clear_pass.h"
#include "mask_pass.h"
#include "texture_pass.h"

#include "texture_loader.h"

#include "group.h"
#include "timer.h"

#include <variant>
#include <unordered_map>


class GLPostProcessingMasking: public ApplicationBase{
    OrbitControl::sptr orbitControl;
    UniformValues::sptr uniforms;
    Timer timer;
    Mesh::sptr mesh;

    Mesh::sptr box;
    Mesh::sptr torus;

    std::shared_ptr<threecpp::ShaderPass> effectSobel;

    std::shared_ptr<threecpp::EffectComposer> composer;

    bool enabled = true;
    float damp = 0.96;

public:
    GLPostProcessingMasking(int x, int y): ApplicationBase(x, y){}

    virtual void init() override {
        renderer->setClearColor(Color().setHex( 0xe0e0e0 ));
        renderer->outputEncoding = TextureEncoding::sRGBEncoding;
        renderer->autoClear = false;

        auto aspect = (float)screenX / screenY;
        camera = std::make_shared<PerspectiveCamera>( 50.f, aspect, 1.f, 1000.f );
        camera->position.set( 0, 0, 10 );

        auto scene1 = Scene::create();
        auto scene2 = Scene::create();

        box = Mesh::create( BoxGeometry::create( 4, 4, 4 ) );
        scene1->add( box );

        torus = Mesh::create( TorusGeometry::create( 3, 1, 16, 32 ) );
        scene2->add( torus );

        auto clearPass = std::make_shared<threecpp::ClearPass>();

        auto clearMaskPass = std::make_shared<threecpp::ClearMaskPass>();

        auto maskPass1 = std::make_shared<threecpp::MaskPass>( scene1, camera );
        auto maskPass2 = std::make_shared<threecpp::MaskPass>( scene2, camera );

        // textures
        std::string rootDir = threecpp::getProjectPath();
        std::string fileSeparator = threecpp::getFileSeparator();
        std::string resourceDir = std::string(rootDir).append(fileSeparator).append("asset").append(fileSeparator)
                .append("textures").append(fileSeparator);

        auto texture1 = TextureLoader::load( resourceDir + "758px-Canestra_di_frutta_(Caravaggio).jpg" );
//        texture1->wrapS = Wrapping::RepeatWrapping;
//        texture1->wrapT = Wrapping::RepeatWrapping;
        texture1->minFilter = TextureFilter::LinearFilter;


        auto texture2 = TextureLoader::load( resourceDir + "2294472375_24a3b8ef46_o.jpg" );

        auto texturePass1 = std::make_shared<threecpp::TexturePass>( texture1 );
        auto texturePass2 = std::make_shared<threecpp::TexturePass>( texture2 );

        auto outputPass = std::make_shared<threecpp::ShaderPass>( ShaderData::getCopyShader() );

        GLRenderTargetParameter::sptr parameters = GLRenderTargetParameter::create();
        parameters->stencilBuffer = true;
        auto renderTarget = GLRenderTarget::create( screenX,screenY );
        renderTarget->texture = Texture::create();
        renderTarget->stencilBuffer = true;

        composer = std::make_shared<threecpp::EffectComposer>( renderer, renderTarget );
        composer->addPass( clearPass );
        composer->addPass( maskPass1 );
        composer->addPass( texturePass1 );
        composer->addPass( clearMaskPass );
        composer->addPass( maskPass2 );
        composer->addPass( texturePass2 );
        composer->addPass( clearMaskPass );
        composer->addPass( outputPass );


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

#endif //THREE_CPP_GL_POSTPROCESSING_MASKING_H
