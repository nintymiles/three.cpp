//
// Created by SeanR on 12/25/2023.
//

#ifndef THREE_CPP_GL_CUSTOM_ATTRIBUTES_H
#define THREE_CPP_GL_CUSTOM_ATTRIBUTES_H


#include "application_model.h"
#include "shader_material.h"
#include "sphere_geometry.h"

#include "perspective_camera.h"
#include "texture_loader.h"

#include "orbit_control.h"
#include "timer.h"

class GLCustomAttributes: public ApplicationBase {
    OrbitControl::sptr pCameraControl;
    UniformValues::sptr uniforms;
    Timer timer;
    Mesh::sptr sphere;

    std::vector<float> displacement;
    std::vector<float> noise;

    std::string fragmentShader = R""(

        varying vec3 vNormal;
        varying vec2 vUv;

        uniform vec3 color;
        uniform sampler2D colorTexture;

        void main() {

            vec3 light = vec3( 0.5, 0.2, 1.0 );
            light = normalize( light );

            float dProd = dot( vNormal, light ) * 0.5 + 0.5;

            vec4 tcolor = texture2D( colorTexture, vUv );
            vec4 gray = vec4( vec3( tcolor.r * 0.3 + tcolor.g * 0.59 + tcolor.b * 0.11 ), 1.0 );

            gl_FragColor = gray * vec4( vec3( dProd ) * vec3( color ), 1.0 );

        }

    )"";

    std::string vertexShader = R""(


        uniform float amplitude;

        attribute float displacement;

        varying vec3 vNormal;
        varying vec2 vUv;

        void main() {

            vNormal = normal;
            vUv = ( 0.5 + amplitude ) * uv + vec2( amplitude );

            vec3 newPosition = position + amplitude * normal * vec3( displacement );
            gl_Position = projectionMatrix * modelViewMatrix * vec4( newPosition, 1.0 );

        }

    )"";

public:
    GLCustomAttributes(int x, int y) : ApplicationBase(x, y) {}

    virtual void init() override {
        renderer->setClearColor(Color().setHex(0x000000));
        renderer->outputEncoding = TextureEncoding::sRGBEncoding;

        auto aspect = (float) screenX / screenY;
        camera = std::make_shared<PerspectiveCamera>(30, aspect, 1, 10000);;
        camera->position.set(0, 0, 300);

        scene = std::make_shared<Scene>();
        scene->setBackgroundColor(Color().set(0x050505));

        // textures
        std::string rootDir = threecpp::getProjectPath();
        std::string fileSeparator = threecpp::getFileSeparator();
        std::string resourceDir = std::string(rootDir).append(fileSeparator).append("asset").append(fileSeparator)
                .append("textures").append(fileSeparator);

        auto waterMap = TextureLoader::load(resourceDir + "water.jpg");
        waterMap->wrapS = Wrapping::RepeatWrapping;
        waterMap->wrapT = Wrapping::RepeatWrapping;


        ShaderMaterial::sptr shaderMaterial = ShaderMaterial::create();

        shaderMaterial->name = "CustomAttributeShader";

        uniforms = std::make_shared<UniformValues>();
        uniforms->set("color", Color(0xff2200));
        uniforms->set("amplitude", 1.f);
        uniforms->set("colorTexture", waterMap);
        shaderMaterial->uniforms = uniforms;

        shaderMaterial->vertexShader = vertexShader;
        shaderMaterial->fragmentShader = fragmentShader;
        shaderMaterial->blending = Blending::NoBlending;
        shaderMaterial->depthTest = true;
        shaderMaterial->depthWrite = true;
        //shaderMaterial->side = Side::DoubleSide;


        auto radius = 50, segments = 128, rings = 64;

        auto geometry = SphereGeometry::create( radius, segments, rings );
        //displacement = new Float32Array( geometry.attributes.position.count );
        size_t positionCount = geometry->attributes[{AttributeName::position,0}]->count;
        displacement = std::vector<float>(positionCount);
        noise = std::vector<float>(positionCount);

        for ( int i = 0; i < displacement.size(); i ++ ) {
            noise[ i ] = math::random() * 5;
        }
        geometry->setAttribute(AttributeName::dispalcement,BufferAttribute<float>::create(displacement,1));

        sphere = Mesh::create( geometry, shaderMaterial );
        scene->add( sphere );

//        pCameraControl = std::make_shared<OrbitControl>(camera);
//        pCameraControl->enablePan = true;
//        pCameraControl->enableDamping = true;
//        controller = pCameraControl;

        timer = Timer();


    }

    virtual void render() override;

    virtual void showControls() override;
};

#endif //THREE_CPP_GL_CUSTOM_ATTRIBUTES_H
