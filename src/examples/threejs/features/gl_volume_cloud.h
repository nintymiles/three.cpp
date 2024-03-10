//
// Created by SeanR on 3/4/2024.
//

#ifndef THREE_CPP_GL_VOLUME_CLOUD_H
#define THREE_CPP_GL_VOLUME_CLOUD_H

#include "application_model.h"
#include "group.h"

#include "orthographic_camera.h"

#include "orbit_control.h"

#include "box_geometry.h"
#include "sphere_geometry.h"

#include "improved_noise.h"

#include "vector3.h"
#include "data_3d_texture.h"

#include <thread>
#include <filesystem>

class GLVolumeCloud: public ApplicationBase{

public:
    GLVolumeCloud(int x, int y):ApplicationBase(x,y){}

    virtual void init() override {
        renderer->setClearColor(Color().setHex(0x000000));
        renderer->shadowMap->enabled = true;
        //renderer->shadowMap->type = ShadowMapType::PCFSoftShadowMap;

        auto aspect = (float)screenX / screenY;
        //camera = OrthographicCamera::create( - height * aspect, height * aspect, height, - height, 1000, 2500 );
        camera = std::make_shared<PerspectiveCamera>(60.0f, aspect , .1f, 100.0f);
        camera->position.set(0.0f, 0.0f, 1.5f);

        isPerspective = true;

        scene = std::make_shared<Scene>();
        scene->setBackgroundColor(Color().set(0x000000));

        //sky
        auto skyMat = MeshBasicMaterial::create();
        skyMat->side = Side::BackSide;
        auto sky = Mesh::create(SphereGeometry::create(10),skyMat);
        scene->add( sky );

        // Texture
        auto size = 128;
        std::vector<unsigned char> data( size * size * size );

        auto i = 0;
        float scale = 0.05;
        ImprovedNoise perlin{};
        Vector3 vector{};

        for ( int z = 0; z < size; z ++ ) {
            for ( int y = 0; y < size; y ++ ) {
                for ( int x = 0; x < size; x ++ ) {
                    float d = 1.0 - vector.set( x, y, z ).subScalar( size / 2 ).divideScalar( size ).length();
                    data[ i ] = ( 128 + 128 * perlin.noise( x * scale / 1.5, y * scale, z * scale / 1.5 ) ) * d * d;
                    i ++;
                }
            }
        }

        auto texture = Data3DTexture::create( data, size, size, size );
        texture->format = PixelFormat::RedFormat;
        texture->minFilter = TextureFilter::LinearFilter;
        texture->magFilter = TextureFilter::LinearFilter;
        texture->unpackAlignment = 1;
        texture->needsUpdate = true;

        //material
        UniformValues::sptr uniforms =std::make_shared<UniformValues>();
        uniforms->set("base", Color(0x798aa0));
        uniforms->set<Texture::sptr>("map", texture);
        uniforms->set("cameraPos", Vector3());
        uniforms->set("threshold", .25f);
        uniforms->set("opacity", .25f);
        uniforms->set("range", .1f);
        uniforms->set("steps", 100);
        uniforms->set("frame", 0);

        auto material = getShader();
        material->uniforms = uniforms;
        material->side = Side::BackSide;
        material->transparent = true;

        auto geometry = BoxGeometry::create( 1, 1, 1 );
        auto mesh = Mesh::create( geometry, material );
        scene->add( mesh );


        //control
        std::shared_ptr<OrbitControl> orbitControl = std::make_shared<OrbitControl>(camera);
        controller = orbitControl;

        orbitControl->minDistance = 1000;
        orbitControl->maxDistance = 2400;
        orbitControl->enablePan = true;
        orbitControl->enableDamping = true;


    }

    virtual void render() override;

private:

    ShaderMaterial::sptr getShader(){
        ShaderMaterial::sptr shader = std::make_shared<ShaderMaterial>();

        shader->vertexShader = R""(

            in vec3 position;

            uniform mat4 modelMatrix;
            uniform mat4 modelViewMatrix;
            uniform mat4 projectionMatrix;
            uniform vec3 cameraPos;

            out vec3 vOrigin;
            out vec3 vDirection;

            void main() {
                vec4 mvPosition = modelViewMatrix * vec4( position, 1.0 );

                vOrigin = vec3( inverse( modelMatrix ) * vec4( cameraPos, 1.0 ) ).xyz;
                vDirection = position - vOrigin;

                gl_Position = projectionMatrix * mvPosition;
            }

        )"";

        shader->fragmentShader = R""(

            precision highp float;
            precision highp sampler3D;

            uniform mat4 modelViewMatrix;
            uniform mat4 projectionMatrix;

            in vec3 vOrigin;
            in vec3 vDirection;

            out vec4 color;

            uniform vec3 base;
            uniform sampler3D map;

            uniform float threshold;
            uniform float range;
            uniform float opacity;
            uniform float steps;
            uniform float frame;

            uint wang_hash(uint seed)
            {
                seed = (seed ^ 61u) ^ (seed >> 16u);
                seed *= 9u;
                seed = seed ^ (seed >> 4u);
                seed *= 0x27d4eb2du;
                seed = seed ^ (seed >> 15u);
                return seed;
            }

            float randomFloat(inout uint seed)
            {
                return float(wang_hash(seed)) / 4294967296.;
            }

            vec2 hitBox( vec3 orig, vec3 dir ) {
                const vec3 box_min = vec3( - 0.5 );
                const vec3 box_max = vec3( 0.5 );
                vec3 inv_dir = 1.0 / dir;
                vec3 tmin_tmp = ( box_min - orig ) * inv_dir;
                vec3 tmax_tmp = ( box_max - orig ) * inv_dir;
                vec3 tmin = min( tmin_tmp, tmax_tmp );
                vec3 tmax = max( tmin_tmp, tmax_tmp );
                float t0 = max( tmin.x, max( tmin.y, tmin.z ) );
                float t1 = min( tmax.x, min( tmax.y, tmax.z ) );
                return vec2( t0, t1 );
            }

            float sample1( vec3 p ) {
                return texture( map, p ).r;
            }

            float shading( vec3 coord ) {
                float step = 0.01;
                return sample1( coord + vec3( - step ) ) - sample1( coord + vec3( step ) );
            }

            void main(){
                vec3 rayDir = normalize( vDirection );
                vec2 bounds = hitBox( vOrigin, rayDir );

                if ( bounds.x > bounds.y ) discard;

                bounds.x = max( bounds.x, 0.0 );

                vec3 p = vOrigin + bounds.x * rayDir;
                vec3 inc = 1.0 / abs( rayDir );
                float delta = min( inc.x, min( inc.y, inc.z ) );
                delta /= steps;

                // Jitter

                // Nice little seed from
                // https://blog.demofox.org/2020/05/25/casual-shadertoy-path-tracing-1-basic-camera-diffuse-emissive/
                uint seed = uint( gl_FragCoord.x ) * uint( 1973 ) + uint( gl_FragCoord.y ) * uint( 9277 ) + uint( frame ) * uint( 26699 );
                vec3 size = vec3( textureSize( map, 0 ) );
                float randNum = randomFloat( seed ) * 2.0 - 1.0;
                p += rayDir * randNum * ( 1.0 / size );

                //
                vec4 ac = vec4( base, 0.0 );

                for ( float t = bounds.x; t < bounds.y; t += delta ) {

                    float d = sample1( p + 0.5 );

                    d = smoothstep( threshold - range, threshold + range, d ) * opacity;

                    float col = shading( p + 0.5 ) * 3.0 + ( ( p.x + p.y ) * 0.25 ) + 0.2;

                    ac.rgb += ( 1.0 - ac.a ) * d * col;

                    ac.a += ( 1.0 - ac.a ) * d;

                    if ( ac.a >= 0.95 ) break;

                    p += rayDir * delta;

                }

                color = ac;

                if ( color.a == 0.0 ) discard;

            }

        )"";

        return shader;
    }


};


#endif //THREE_CPP_GL_VOLUME_CLOUD_H
