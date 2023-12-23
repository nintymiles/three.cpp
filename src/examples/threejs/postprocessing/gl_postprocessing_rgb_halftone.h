//
// Created by SeanR on 12/22/2023.
//

#ifndef THREE_CPP_GL_POSTPROCESSING_RGB_HALFTONE_H
#define THREE_CPP_GL_POSTPROCESSING_RGB_HALFTONE_H

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
#include "half_tone_pass.h"

#include "group.h"
#include "timer.h"

#include <variant>
#include <unordered_map>


class GLPostProcessingRGBHalfTone: public ApplicationBase{
    OrbitControl::sptr orbitControl;
    UniformValues::sptr uniforms;
    Timer timer;

    std::shared_ptr<threecpp::EffectComposer> composer;
    Group::sptr group;

    const float rotationSpeed = math_number::PI / 64;

    struct Settings{
        float radius = 4.f;
        float rotateR = (float)math_number::PI / 12;
        float rotateG = (float)math_number::PI / 12 * 2;
        float rotateB = (float)math_number::PI / 12 * 3;
        float scatter = 0.f;
        bool greyscale = false;
        bool disable = false;
        float blending = 1.f;
        int blendingMode = 1;
        int shape = 1;
    };
    Settings setting{};

    std::vector<std::string> shapeVec = { "Dot", "Ellipse", "Line", "Square" },
                            blendModeVec = { "Linear", "Multiply", "Add", "Lighter", "Darker" };



public:
    GLPostProcessingRGBHalfTone(int x, int y): ApplicationBase(x, y){}

    virtual void init() override {
        renderer->setClearColor(Color().setHex(0x000000));
        renderer->outputEncoding = TextureEncoding::sRGBEncoding;

        auto aspect = (float)screenX / screenY;
        camera = std::make_shared<PerspectiveCamera>( 75, aspect, 1, 1000 );
        camera->position.set( 0, 0, 12 );

        scene = std::make_shared<Scene>();
        scene->setBackgroundColor(Color().set(0x444444));

        group = Group::create();
        auto floor = Mesh::create( BoxGeometry::create( 100, 1, 100 ), MeshPhongMaterial::create( ) );
        floor->position.y = - 10;
        auto light = PointLight::create( 0xffffff, 1.0, 50, 2 );
        light->position.y = 2;
        group->add( floor );
        group->add( light );
        scene->add( group );

        auto material = ShaderMaterial::create();
        UniformValues::sptr uniforms =std::make_shared<UniformValues>();
        material->uniforms = uniforms;

        material->vertexShader = R""(

            varying vec2 vUV;
            varying vec3 vNormal;

            void main() {

                vUV = uv;
                vNormal = vec3( normal );
                gl_Position = projectionMatrix * modelViewMatrix * vec4( position, 1.0 );

            }

        )"";

        material->fragmentShader = R""(

            varying vec2 vUV;
            varying vec3 vNormal;

            void main() {
                vec4 c = vec4( abs( vNormal ) + vec3( vUV, 0.0 ), 0.0 );
                gl_FragColor = c;

            }

        )"";

        for ( int i = 0; i < 50; ++ i ) {
            // fill scene with coloured cubes
            auto mesh =Mesh::create( BoxGeometry::create( 2, 2, 2 ), material );
            mesh->position.set( math::random() * 16 - 8, math::random() * 16 - 8, math::random() * 16 - 8 );
            mesh->rotation.set( math::random() * math_number::PI * 2, math::random() * math_number::PI * 2, math::random() * math_number::PI * 2 );
            group->add( mesh );

        }

        // postprocessing
        composer = std::make_shared<threecpp::EffectComposer>( renderer, renderer->getRenderTarget() );
        auto renderPass = std::make_shared<threecpp::RenderPass>( scene, camera );
        composer->addPass( renderPass );

        std::unordered_map<std::string,std::variant<float,int,bool>> params = {
                {"shape", 1},
                {"radius", 4.f},
                {"rotateR", (float)math_number::PI / 12},
                {"rotateB", (float)math_number::PI / 12 * 2},
                {"rotateG", (float)math_number::PI / 12 * 3},
                {"scatter", 0.f},
                {"blending", 1.f},
                {"blendingMode", 1},
                {"greyscale", false},
                {"disable", false}
        };
        auto halftonePass = std::make_shared<threecpp::HalfTonePass>( screenX, screenY, params );
        composer->addPass( halftonePass );
        this->uniforms = halftonePass->uniforms;

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



#endif //THREE_CPP_GL_POSTPROCESSING_RGB_HALFTONE_H
