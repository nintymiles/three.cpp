//
// Created by SeanR on 12/18/2024.
//

#ifndef THREE_CPP_GL_SHADER_DEMO2_H
#define THREE_CPP_GL_SHADER_DEMO2_H

#include "application_model.h"
#include "shader_material.h"
#include "plane_geometry.h"
#include "box_geometry.h"

#include "texture_loader.h"

#include "orthographic_camera.h"

#include "orbit_control.h"
#include "timer.h"

class GLShaderDemo2: public ApplicationBase{
    OrthographicCamera::sptr orthographicCamera;
    OrbitControl::sptr pCameraControl;
    UniformValues::sptr uniforms1,uniforms2;
    Timer timer;

    std::string fragmentShader1 = R""(

            uniform float time;

			varying vec2 vUv;

			void main(void) {

				vec2 p = - 1.0 + 2.0 * vUv;
				float a = time * 40.0;
				float d, e, f, g = 1.0 / 40.0 ,h ,i ,r ,q;

				e = 400.0 * ( p.x * 0.5 + 0.5 );
				f = 400.0 * ( p.y * 0.5 + 0.5 );
				i = 200.0 + sin( e * g + a / 150.0 ) * 20.0;
				d = 200.0 + cos( f * g / 2.0 ) * 18.0 + cos( e * g ) * 7.0;
				r = sqrt( pow( abs( i - e ), 2.0 ) + pow( abs( d - f ), 2.0 ) );
				q = f / r;
				e = ( r * cos( q ) ) - a / 2.0;
				f = ( r * sin( q ) ) - a / 2.0;
				d = sin( e * g ) * 176.0 + sin( e * g ) * 164.0 + r;
				h = ( ( f + d ) + a / 2.0 ) * g;
				i = cos( h + r * p.x / 1.3 ) * ( e + e + a ) + cos( q * g * 6.0 ) * ( r + h / 3.0 );
				h = sin( f * g ) * 144.0 - sin( e * g ) * 212.0 * p.x;
				h = ( h + ( f - e ) * q + sin( r - ( a + h ) / 7.0 ) * 10.0 + i / 4.0 ) * g;
				i += cos( h * 2.3 * sin( a / 350.0 - q ) ) * 184.0 * sin( q - ( r * 4.3 + a / 12.0 ) * g ) + tan( r * g + h ) * 184.0 * cos( r * g + h );
				i = mod( i / 5.6, 256.0 ) / 64.0;
				if ( i < 0.0 ) i += 4.0;
				if ( i >= 2.0 ) i = 4.0 - i;
				d = r / 350.0;
				d += sin( d * d * 8.0 ) * 0.52;
				f = ( sin( a * g ) + 1.0 ) / 2.0;
				gl_FragColor = vec4( vec3( f * i / 1.6, i / 2.0 + d / 13.0, i ) * d * p.x + vec3( i / 1.3 + d / 8.0, i / 2.0 + d / 18.0, i ) * d * ( 1.0 - p.x ), 1.0 );

			}
    )"";

    std::string fragmentShader2 = R""(

            uniform float time;

			uniform sampler2D colorTexture;

			varying vec2 vUv;

			void main( void ) {

				vec2 position = - 1.0 + 2.0 * vUv;

				float a = atan( position.y, position.x );
				float r = sqrt( dot( position, position ) );

				vec2 uv;
				uv.x = cos( a ) / r;
				uv.y = sin( a ) / r;
				uv /= 10.0;
				uv += time * 0.05;

				vec3 color = texture2D( colorTexture, uv ).rgb;

				gl_FragColor = vec4( color * r * 1.5, 1.0 );

			}
    )"";

    std::string fragmentShader3 = R""(

            uniform float time;

			varying vec2 vUv;

			void main( void ) {

				vec2 position = vUv;

				float color = 0.0;
				color += sin( position.x * cos( time / 15.0 ) * 80.0 ) + cos( position.y * cos( time / 15.0 ) * 10.0 );
				color += sin( position.y * sin( time / 10.0 ) * 40.0 ) + cos( position.x * sin( time / 25.0 ) * 40.0 );
				color += sin( position.x * sin( time / 5.0 ) * 10.0 ) + sin( position.y * sin( time / 35.0 ) * 80.0 );
				color *= sin( time / 10.0 ) * 0.5;

				gl_FragColor = vec4( vec3( color, color * 0.5, sin( color + time / 3.0 ) * 0.75 ), 1.0 );

			}
    )"";

    std::string fragmentShader4 = R""(

            uniform float time;

			varying vec2 vUv;

			void main( void ) {

				vec2 position = - 1.0 + 2.0 * vUv;

				float red = abs( sin( position.x * position.y + time / 5.0 ) );
				float green = abs( sin( position.x * position.y + time / 4.0 ) );
				float blue = abs( sin( position.x * position.y + time / 3.0 ) );
				gl_FragColor = vec4( red, green, blue, 1.0 );

			}
    )"";

    std::string vertexShader = R""(

        varying vec2 vUv;

        void main()
        {
            vUv = uv;
            vec4 mvPosition = modelViewMatrix * vec4( position, 1.0 );
            gl_Position = projectionMatrix * mvPosition;
        }
    )"";

public:
    GLShaderDemo2(int x, int y): ApplicationBase(x, y){}

    virtual void init() override {
        renderer->setClearColor(Color().setHex(0x000000));
        renderer->outputEncoding = TextureEncoding::sRGBEncoding;

        auto aspect = (float)screenX / screenY;
        camera = std::make_shared<PerspectiveCamera>( 40, aspect, 1, 3000 );;
        camera->position.set( 0, 0, 4 );

        //scene->add(camera);

        scene = std::make_shared<Scene>();
        scene->setBackgroundColor(Color().set(0x000000));

        uniforms1 =std::make_shared<UniformValues>();
        uniforms1->set("time", 1.f);


        // textures
        std::string rootDir = threecpp::getProjectPath();
        std::string fileSeparator = threecpp::getFileSeparator();
        std::string resourceDir = std::string(rootDir).append(fileSeparator).append("asset").append(fileSeparator)
                .append("textures").append(fileSeparator);

        auto disturbMap = TextureLoader::load( resourceDir + "disturb.jpg" );
        disturbMap->wrapS = Wrapping::RepeatWrapping;
        disturbMap->wrapT = Wrapping::RepeatWrapping;

        uniforms2 =std::make_shared<UniformValues>();
        uniforms2->set("time", 1.f);

        uniforms2->set("colorTexture",disturbMap);

        std::vector<std::pair<std::string,UniformValues::sptr>> params = {
                { "fragment_shader1", uniforms1 },
                { "fragment_shader2", uniforms2 },
                { "fragment_shader3", uniforms1 },
                { "fragment_shader4", uniforms1 }
                };
        std::vector<std::string> frags = {fragmentShader1,fragmentShader2,fragmentShader3,fragmentShader4};


//        ShaderMaterial::sptr shaderMaterial = ShaderMaterial::create();
//
//        shaderMaterial->name = "CustomShader1";
//
//        shaderMaterial->uniforms = uniforms1;
//
//        shaderMaterial->vertexShader = vertexShader;
//        shaderMaterial->fragmentShader = fragmentShader1;



        //
        auto geometry = BoxGeometry::create( 0.75, 0.75, 0.75 );
        for ( auto i = 0; i < params.size(); i ++ ) {

            ShaderMaterial::sptr material = ShaderMaterial::create();
            material->uniforms = params[i].second;
            material->vertexShader = vertexShader;
            material->fragmentShader = frags[i];
            material->name = params[i].first;

            material->blending = Blending::NoBlending;
            material->depthTest = false;
            material->depthWrite = false;


            auto mesh = Mesh::create( geometry, material );
            mesh->position.x = i - ( (int)frags.size() - 1 ) / 2;
            mesh->position.y = i % 2 - 0.5;
            scene->add( mesh );

        }





//        Mesh::sptr planeMesh = Mesh::create( geometry, shaderMaterial );
//        scene->add( planeMesh );

        pCameraControl = std::make_shared<OrbitControl>(camera);
        //this setting make drawing objects projected out of screen
//        pCameraControl->minDistance = 500;
//        pCameraControl->maxDistance = 2500;
        pCameraControl->enablePan = true;
        pCameraControl->enableDamping = true;
        controller = pCameraControl;

        timer = Timer();


    }

    virtual void render() override;

    virtual void showControls() override;
};


#endif //THREE_CPP_GL_SHADER_DEMO2_H
