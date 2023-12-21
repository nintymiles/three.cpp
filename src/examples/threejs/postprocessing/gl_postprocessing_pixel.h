//
// Created by ultraman on 12/21/2023.
//

#ifndef THREE_CPP_GL_POSTPROCESSING_PIXEL_H
#define THREE_CPP_GL_POSTPROCESSING_PIXEL_H


#include "application_model.h"

#include "perspective_camera.h"
#include "effect_composer.h"
#include "trackball_control.h"
#include "orbit_control.h"

#include "sphere_geometry.h"
#include "box_geometry.h"
#include "cone_geometry.h"
#include "tetrahedron_geometry.h"
#include "torus_knot_geometry.h"

#include "mesh_phong_material.h"

#include "directional_light.h"
#include "hemisphere_light.h"

#include "render_pass.h"
#include "shader_pass.h"
#include "shader_data.h"

#include "group.h"


class GLPostprocessingPixel: public ApplicationBase{
    TrackballControls::sptr tControl;
    OrbitControl::sptr oControl;
    UniformValues::sptr uniforms;
    Timer timer;
    std::vector<BufferGeometry::sptr> geometries;

    std::shared_ptr<threecpp::EffectComposer> composer;
    Object3D::sptr object;

public:
    GLPostprocessingPixel(int x, int y): ApplicationBase(x, y){}

    virtual void init() override {
        renderer->setClearColor(Color().setHex(0x000000));
        renderer->outputEncoding = TextureEncoding::sRGBEncoding;

        auto aspect = (float)screenX / screenY;
        camera = std::make_shared<PerspectiveCamera>( 75, aspect, 1, 10000 );
        camera->position.set( 0, 0, 30 );

        scene = std::make_shared<Scene>();
        scene->setBackgroundColor(Color().set(0x000000));
        scene->fog = Fog::create( 0x000000, 1, 1000 );

        object = Object3D::create();
        scene->add( object );
        scene->setBackgroundColor(Color().set(0xffffff));

        auto hemisphereLight = HemisphereLight::create( 0xfceafc, 0x000000, .8 );
        scene->add( hemisphereLight );

        auto dirLight = DirectionalLight::create( 0xffffff, .5 );
        dirLight->position.set( 150, 75, 150 );
        scene->add( dirLight );

        auto dirLight2 = DirectionalLight::create( 0xffffff, .2 );
        dirLight2->position.set( - 150, 75, - 150 );
        scene->add( dirLight2 );

        auto dirLight3 = DirectionalLight::create( 0xffffff, .1 );
        dirLight3->position.set( 0, 125, 0 );
        scene->add( dirLight3 );

        geometries = {
                SphereGeometry::create( 1, 64, 64 ),
                BoxGeometry::create( 1, 1, 1 ),
                ConeGeometry::create( 1, 1, 32 ),
                TetrahedronGeometry::create( 1 ),
                TorusKnotGeometry::create( 1, .4 )
        };

        Group::sptr grp = Group::create();
        for ( int i = 0; i < 25; i ++ ) {

            auto geom = geometries[ math::floor( math::random() * geometries.size() ) ];

            auto color = Color();
            color.setHSL( math::random(), .7f + .2f * math::random(), .5f + .1f * math::random() );
//            color.setRGB(math::random(), .7f + .2f * math::random(), .5f + .1f * math::random());

            auto mat = MeshPhongMaterial::create( color );
            mat->shininess = 200;

            auto mesh = Mesh::create( geom, mat );

            auto s = 4 + math::random() * 10;
            mesh->scale.set( s, s, s );
            mesh->position.set( math::random() - 0.5, math::random() - 0.5, math::random() - 0.5 ).normalize();
            mesh->position.multiplyScalar( math::random() * 200 );
            mesh->rotation.set( math::random() * 2, math::random() * 2, math::random() * 2 );
            grp->add( mesh );

        }

        scene->add( grp );

        composer = std::make_shared<threecpp::EffectComposer>( renderer, renderer->getRenderTarget() );
        auto renderPass = std::make_shared<threecpp::RenderPass>( scene, camera );
        composer->addPass( renderPass );

        auto pixelPass = std::make_shared<threecpp::ShaderPass>( ShaderData::getPixelShader() );
        Vector2 rVal = Vector2( screenX,screenY );
        //rVal.multiplyScalar(aspect);
        pixelPass->uniforms->set("resolution",rVal);
        composer->addPass( pixelPass );

        Vector4 screen = Vector4(0, 0, screenX, screenY);
        tControl = std::make_shared<TrackballControls>(camera,screen);
        tControl->rotateSpeed = 2.0;
        tControl->panSpeed = 0.8;
        tControl->zoomSpeed = 1.5;
        controller = tControl;

//        oControl = std::make_shared<OrbitControl>(camera);
//        oControl->rotateSpeed = 2.0;
//        oControl->panSpeed = 0.8;
//        oControl->zoomSpeed = 1.5;
//        controller = oControl;


    }

    virtual void render() override;

    virtual void showControls() override;
};


#endif //THREE_CPP_GL_POSTPROCESSING_PIXEL_H
