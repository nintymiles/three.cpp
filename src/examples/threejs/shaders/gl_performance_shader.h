//
// Created by SeanR on 12/25/2023.
//

#ifndef THREE_CPP_GL_PERFORMANCE_SHADER_H
#define THREE_CPP_GL_PERFORMANCE_SHADER_H

#include "application_model.h"
#include "shader_material.h"
#include "torus_geometry.h"

#include "perspective_camera.h"
#include "texture_loader.h"

#include "orbit_control.h"
#include "timer.h"

#include "frustum.h"

class GLPerformanceShader: public ApplicationBase{
    OrbitControl::sptr pCameraControl;
    UniformValues::sptr uniforms;
    Timer timer;
    Mesh::sptr mesh;

    std::vector<Material::sptr> materials;


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

    void removeAllMeshes() {
        for ( int i = scene->children.size() - 1; i >= 0; i -- ) {
            auto obj = scene->children[ i ];
            scene->remove( obj );
//            obj.geometry.dispose();
//            obj.material.dispose();
        }

    }

    void addMeshes() {

        removeAllMeshes();
        //reset pseudorandom number
        //THREE.MathUtils.seededRandom( 1 );

        auto projScreenMatrix = Matrix4();
        auto frustum = Frustum();
        camera->updateMatrixWorld();
        projScreenMatrix.multiplyMatrices( camera->projectionMatrix, camera->matrixWorldInverse );
        frustum.setFromProjectionMatrix( projScreenMatrix );

        std::random_device rd;
        std::mt19937 rng(rd());
        std::uniform_real_distribution<float> dis(0, 1);

        auto size = 0.65f;
        int meshesCount = 0;
        while ( meshesCount < 2500 ) {

            auto material = ShaderMaterial::create();
            material->uniforms = uniforms;
            material->vertexShader = vertexShader;
            material->fragmentShader = fragmentShader;

            auto mesh = Mesh::create( TorusGeometry::create( size, 0.3, 30, 30 ), material );

            mesh->position.x = dis(rng) * 20 - 10;
            mesh->position.y = dis(rng) * 20 - 10;
            mesh->position.z = dis(rng) * 20 - 10;
            mesh->rotation.setX( dis(rng) * 2 * math_number::PI );
            mesh->rotation.setY( dis(rng) * 2 * math_number::PI );
            float scaleVal = dis(rng) * .2 + 0.1;
            mesh->scale.set(scaleVal,scaleVal,scaleVal);

//            mesh->updateMatrixWorld();

            if ( frustum.intersectsObject( *mesh ) ) {
                // mesh.rotation.x = 0.3;
                materials.push_back( material );
                scene->add( mesh );
                meshesCount ++;
            }

        }

    }


public:
    GLPerformanceShader(int x, int y): ApplicationBase(x, y){}

    virtual void init() override {
        renderer->setClearColor(Color().setHex(0x000000));
        renderer->outputEncoding = TextureEncoding::sRGBEncoding;

        auto aspect = (float)screenX / screenY;
        camera = std::make_shared<PerspectiveCamera>( 60, aspect, 1, 3000 );;
        camera->position.set( 0, 0, 7 );

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


        uniforms =std::make_shared<UniformValues>();
        uniforms->set("fogDensity", 0.45f);
        uniforms->set("fogColor", Vector3( 0, 0, 0 ));
        uniforms->set("time", 1.f);
        uniforms->set("uvScale", Vector2( 3.0, 1.0 ));
        uniforms->set("texture1", cloudMap);
        uniforms->set("texture2", tileMap);

        addMeshes();

        pCameraControl = std::make_shared<OrbitControl>(camera);
        pCameraControl->enablePan = true;
        pCameraControl->enableDamping = true;
        controller = pCameraControl;

        timer = Timer();


    }

    virtual void render() override;

    virtual void showControls() override;
};




#endif //THREE_CPP_GL_PERFORMANCE_SHADER_H
