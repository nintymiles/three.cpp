//
// Created by SeanR on 12/18/2023.
//

#ifndef THREE_CPP_GL_SHADER_LAVA_H
#define THREE_CPP_GL_SHADER_LAVA_H

#include "application_model.h"
#include "shader_material.h"
#include "torus_geometry.h"

#include "perspective_camera.h"
#include "texture_loader.h"

#include "orbit_control.h"
#include "timer.h"

class GLShaderLava: public ApplicationBase{
    OrbitControl::sptr pCameraControl;
    UniformValues::sptr uniforms;
    Timer timer;
    Mesh::sptr mesh;

    std::string fragmentShader = R""(

			uniform float time;

			uniform float fogDensity;
			uniform vec3 fogColor;

			uniform sampler2D texture1;
			uniform sampler2D texture2;

			varying vec2 vUv;

			void main( void ) {

				//vec2 position = - 1.0 + 2.0 * vUv;

				vec4 noise = texture2D( texture1, vUv );
				vec2 T1 = vUv + vec2( 1.5, - 1.5 ) * time * 0.02;
				vec2 T2 = vUv + vec2( - 0.5, 2.0 ) * time * 0.01;

				T1.x += noise.x * 2.0;
				T1.y += noise.y * 2.0;
				T2.x -= noise.y * 0.2;
				T2.y += noise.z * 0.2;

				float p = texture2D( texture1, T1 * 2.0 ).a;

				vec4 color = texture2D( texture2, T2 * 2.0 );
				vec4 temp = color * ( vec4( p, p, p, p ) * 2.0 ) + ( color * color - 0.1 );

				if( temp.r > 1.0 ) { temp.bg += clamp( temp.r - 2.0, 0.0, 100.0 ); }
				if( temp.g > 1.0 ) { temp.rb += temp.g - 1.0; }
				if( temp.b > 1.0 ) { temp.rg += temp.b - 1.0; }

				gl_FragColor = temp;

				float depth = gl_FragCoord.z / gl_FragCoord.w;
				const float LOG2 = 1.442695;
				float fogFactor = exp2( - fogDensity * fogDensity * depth * depth * LOG2 );
				fogFactor = 1.0 - clamp( fogFactor, 0.0, 1.0 );

				gl_FragColor = mix( gl_FragColor, vec4( fogColor, gl_FragColor.w ), fogFactor );

			}

    )"";

    std::string vertexShader = R""(

			uniform vec2 uvScale;
			varying vec2 vUv;

			void main()
			{

				vUv = uvScale * uv;
				vec4 mvPosition = modelViewMatrix * vec4( position, 1.0 );
				gl_Position = projectionMatrix * mvPosition;

			}

    )"";

public:
    GLShaderLava(int x, int y): ApplicationBase(x, y){}

    virtual void init() override {
        renderer->setClearColor(Color().setHex(0x000000));
        renderer->outputEncoding = TextureEncoding::sRGBEncoding;

        auto aspect = (float)screenX / screenY;
        camera = std::make_shared<PerspectiveCamera>( 35, aspect, 1, 3000 );;
        camera->position.set( 0, 0, 4 );

        scene = std::make_shared<Scene>();
        scene->setBackgroundColor(Color().set(0x000000));

        // textures
        std::string rootDir = threecpp::getProjectPath();
        std::string fileSeparator = threecpp::getFileSeparator();
        std::string resourceDir = std::string(rootDir).append(fileSeparator).append("asset").append(fileSeparator)
                .append("textures").append(fileSeparator)
                .append("lava").append(fileSeparator);

        auto cloudMap = TextureLoader::load( resourceDir + "cloud.png" );
        cloudMap->wrapS = Wrapping::RepeatWrapping;
        cloudMap->wrapT = Wrapping::RepeatWrapping;

        auto tileMap = TextureLoader::load( resourceDir + "lavatile.jpg" );
        tileMap->wrapS = Wrapping::RepeatWrapping;
        tileMap->wrapT = Wrapping::RepeatWrapping;

        ShaderMaterial::sptr shaderMaterial = ShaderMaterial::create();

        shaderMaterial->name = "CustomShaderLava";

        uniforms =std::make_shared<UniformValues>();
        uniforms->set("fogDensity", 0.45f);
        uniforms->set("fogColor", Vector3( 0, 0, 0 ));
        uniforms->set("time", 1.f);
        uniforms->set("uvScale", Vector2( 3.0, 1.0 ));
        uniforms->set("texture1", cloudMap);
        uniforms->set("texture2", tileMap);
        shaderMaterial->uniforms = uniforms;

        shaderMaterial->vertexShader = vertexShader;
        shaderMaterial->fragmentShader = fragmentShader;
        shaderMaterial->blending = Blending::NoBlending;
        shaderMaterial->depthTest = true;
        shaderMaterial->depthWrite = true;
        //shaderMaterial->side = Side::DoubleSide;


        float size = 0.65f;

        TorusGeometry::sptr torusGeometry = TorusGeometry::create( size, 0.3, 30, 30 );
        mesh = Mesh::create( torusGeometry, shaderMaterial );
        mesh->rotation.setX( 0.3 );
        scene->add( mesh );

        pCameraControl = std::make_shared<OrbitControl>(camera);
        pCameraControl->enablePan = true;
        pCameraControl->enableDamping = true;
        controller = pCameraControl;

        timer = Timer();


    }

    virtual void render() override;

    virtual void showControls() override;
};


#endif //THREE_CPP_GL_SHADER_LAVA_H
