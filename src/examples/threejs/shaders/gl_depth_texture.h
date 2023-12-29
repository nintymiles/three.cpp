//
// Created by SeanR on 12/27/2023.
//

#ifndef THREE_CPP_GL_DEPTH_TEXTURE_H
#define THREE_CPP_GL_DEPTH_TEXTURE_H

#include "application_model.h"
#include "shader_material.h"
#include "torus_geometry.h"

#include "perspective_camera.h"
#include "orthographic_camera.h"
#include "texture_loader.h"
#include "depth_texture.h"

#include "gl_render_target.h"
#include "plane_geometry.h"
#include "torus_knot_geometry.h"
#include "mesh_basic_material.h"

#include "common_types.h"

#include "orbit_control.h"
#include "timer.h"

class GLDepthTexture: public ApplicationBase{
    OrbitControl::sptr pCameraControl;
    UniformValues::sptr uniforms;
    Timer timer;
    Mesh::sptr mesh;

    GLRenderTarget::sptr target;
    Scene::sptr postScene;
    OrthographicCamera::sptr postCamera;
    ShaderMaterial::sptr postMaterial;

    std::string fragmentShader = R""(

			#include <packing>

			varying vec2 vUv;
			uniform sampler2D tDiffuse;
			uniform sampler2D tDepth;
			uniform float cameraNear;
			uniform float cameraFar;


			float readDepth( sampler2D depthSampler, vec2 coord ) {
				float fragCoordZ = texture2D( depthSampler, coord ).x;
				float viewZ = perspectiveDepthToViewZ( fragCoordZ, cameraNear, cameraFar );
				return viewZToOrthographicDepth( viewZ, cameraNear, cameraFar );
			}

			void main() {
				//vec3 diffuse = texture2D( tDiffuse, vUv ).rgb;
				float depth = readDepth( tDepth, vUv );

				gl_FragColor.rgb = 1.0 - vec3( depth );
				gl_FragColor.a = 1.0;
			}

    )"";

    std::string vertexShader = R""(

			varying vec2 vUv;

			void main() {
				vUv = uv;
				gl_Position = projectionMatrix * modelViewMatrix * vec4(position, 1.0);
			}

    )"";

    void setupRenderTarget() {

//        if ( target ) target->dispose();

//        const format = parseFloat( params.format );
//        const type = parseFloat( params.type );

        target = GLRenderTarget::create( screenX,screenY );
        target->depthTexture = DepthTexture::create(screenX,screenY );
        target->depthTexture->isRenderTargetTexture = true;
        target->texture = Texture::create();
        target->texture->name = "depth_fb_tex";
        target->texture->format = PixelFormat::RGBFormat;
        target->texture->type = TextureDataType::UnsignedShort565Type;
        target->texture->isRenderTargetTexture = true;
        target->texture->imageHeight = screenY;
        target->texture->imageWidth = screenX;
        target->texture->minFilter = TextureFilter::LinearFilter;
        target->texture->magFilter = TextureFilter::LinearFilter;
//        target->stencilBuffer = true ;

        target->depthTexture->format = PixelFormat::DepthFormat;
        target->depthTexture->type = TextureDataType::UnsignedShortType;
        target->depthTexture->minFilter = TextureFilter::NearestFilter;
        target->depthTexture->magFilter = TextureFilter::NearestFilter;

    }

    void setupPost() {

        // Setup post processing stage
        postCamera = OrthographicCamera::create( - 1, 1, 1, - 1, 0, 1 );
        postMaterial = ShaderMaterial::create();
        postMaterial->vertexShader = vertexShader;
        postMaterial->fragmentShader = fragmentShader;
        uniforms =std::make_shared<UniformValues>();
        uniforms->set("cameraNear", camera->_near);
        uniforms->set("cameraFar", camera->_far);
        uniforms->set<Texture::sptr>("tDiffuse", nullptr);
        uniforms->set<Texture::sptr>("tDepth", nullptr);
        postMaterial->uniforms = uniforms;

        auto postPlane = PlaneGeometry::create( 2, 2 );
        auto postQuad = Mesh::create( postPlane, postMaterial );
        postScene = Scene::create();
        postScene->add( postQuad );

    }

    void setupScene() {

        scene = Scene::create();

        auto geometry = TorusKnotGeometry::create( 1, 0.3, 128, 64 );
        auto material = MeshBasicMaterial::create( Color(threecpp::Colors::blue) );
        material->depthTest = true;

        auto count = 50;
        auto scale = 5;

        for ( int i = 0; i < count; i ++ ) {
            auto r = math::random() * 2.0 * math_number::PI;
            auto z = ( math::random() * 2.0 ) - 1.0;
            auto zScale = math::sqrt( 1.0 - z * z ) * scale;

            auto mesh = Mesh::create( geometry, material );
            mesh->position.set(
                    math::cos( r ) * zScale,
                    math::sin( r ) * zScale,
                    z * scale
            );
            mesh->rotation.set( math::random(), math::random(), math::random() );
            scene->add( mesh );

        }

    }

public:
    GLDepthTexture(int x, int y): ApplicationBase(x, y){}

    virtual void init() override {
        renderer->setClearColor(Color().setHex(0x000000));
        renderer->outputEncoding = TextureEncoding::sRGBEncoding;

        auto aspect = (float)screenX / screenY;
        camera = std::make_shared<PerspectiveCamera>( 70.f, aspect, .01f, 50.f );;
        camera->position.set( 0, 0, 4 );

        scene = std::make_shared<Scene>();
        scene->setBackgroundColor(Color().set(0x000000));

        setupRenderTarget();

        setupScene();

        setupPost();

        pCameraControl = std::make_shared<OrbitControl>(camera);
        pCameraControl->enablePan = true;
        pCameraControl->enableDamping = true;
        controller = pCameraControl;

        timer = Timer();


    }

    virtual void render() override;

    virtual void showControls() override;
};

#endif //THREE_CPP_GL_DEPTH_TEXTURE_H
