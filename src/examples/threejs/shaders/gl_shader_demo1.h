//
// Created by SeanR on 12/18/2023.
//

#ifndef THREE_CPP_GL_SHADER_DEMO1_H
#define THREE_CPP_GL_SHADER_DEMO1_H

#include "application_model.h"
#include "shader_material.h"
#include "plane_geometry.h"

#include "orthographic_camera.h"

#include "orbit_control.h"
#include "timer.h"

class GLShaderDemo1: public ApplicationBase{
    OrthographicCamera::sptr orthographicCamera;
    OrbitControl::sptr pCameraControl;
    UniformValues::sptr uniforms;
    Timer timer;

    std::string fragmentShader = R""(

        varying vec2 vUv;

        uniform float time;

        void main()	{

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

    std::string vertexShader = R""(

        varying vec2 vUv;

        void main()	{

            vUv = uv;

            gl_Position = vec4( position, 1.0 );

        }

    )"";

public:
    GLShaderDemo1(int x, int y): ApplicationBase(x, y){}

    virtual void init() override {
        renderer->setClearColor(Color().setHex(0x000000));
        renderer->outputEncoding = TextureEncoding::sRGBEncoding;

        auto aspect = (float)screenX / screenY;
        orthographicCamera = std::make_shared<OrthographicCamera>( - 1, 1, 1, - 1, 0, 1 );
        //orthographicCamera->position.set( 0, 0, 1000 );
        //scene->add(camera);

        camera = orthographicCamera;

        scene = std::make_shared<Scene>();
        scene->setBackgroundColor(Color().set(0x000000));

        ShaderMaterial::sptr shaderMaterial = ShaderMaterial::create();

        shaderMaterial->name = "CustomShader1";

        uniforms =std::make_shared<UniformValues>();
        uniforms->set("time", 1.f);
        shaderMaterial->uniforms = uniforms;

        shaderMaterial->vertexShader = vertexShader;
        shaderMaterial->fragmentShader = fragmentShader;
        shaderMaterial->blending = Blending::NoBlending;
        shaderMaterial->depthTest = false;
        shaderMaterial->depthWrite = false;


        //
        auto geometry = PlaneGeometry::create( 2, 2 );

        Mesh::sptr planeMesh = Mesh::create( geometry, shaderMaterial );
        scene->add( planeMesh );

        pCameraControl = std::make_shared<OrbitControl>(orthographicCamera);
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


#endif //THREE_CPP_GL_SHADER_DEMO1_H
