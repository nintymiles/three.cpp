//
// Created by SeanR on 2023/11/6.
//

#ifndef THREE_CPP_GL_BUFFER_GEOMETRY_ATTRIBUTES_NONE_H
#define THREE_CPP_GL_BUFFER_GEOMETRY_ATTRIBUTES_NONE_H

#include "application_model.h"
#include "color.h"
#include "line_basic_material.h"
#include "line.h"
#include "fog.h"
#include "raw_shader_material.h"

#include "vector3.h"
#include "math_utils.h"

#include "buffer_geometry.h"

#include <tuple>
#include <vector>

class GLBufferGeometryAttributesNone : public ApplicationBase {
public:
    GLBufferGeometryAttributesNone(int x, int y) :  ApplicationBase(x,y) {}

    virtual void init() override {
        renderer->setClearColor(Color().setHex(0x000000));
        renderer->shadowMap->enabled = true;
        //renderer->shadowMap->type = ShadowMapType::PCFSoftShadowMap;

        camera = std::make_shared<PerspectiveCamera>(80.0f, screenX/screenY , 1.f, 3500.0f);
        camera->position.set(0.0f, 0.0f, 1000.0f);
        //camera->lookAt(Vector3());
        isPerspective = true;

        scene = std::make_shared<Scene>();
        scene->setBackgroundColor(Color().set(0x050505));
        scene->fog = Fog::create( 0x050505, 2000, 3500 );

        const int triangleCount = 10000;
        const int vertexCountPerTriangle = 3;
        const int vertexCount = triangleCount * vertexCountPerTriangle;

        BufferGeometry::sptr geometry = BufferGeometry::create();
        geometry->setDrawRange( 0, vertexCount );

        const std::string vertexShader = R"(

			uniform mat4 modelViewMatrix;
			uniform mat4 projectionMatrix;

			uniform float seed;

			const uint ieeeMantissa = 0x007FFFFFu;
			const uint ieeeOne = 0x3F800000u;

			uint hash(uint x) {
				x += ( x << 10u );
				x ^= ( x >>  6u );
				x += ( x <<  3u );
				x ^= ( x >> 11u );
				x += ( x << 15u );
				return x;
			}

			uint hash(uvec2 v) { return hash( v.x ^ hash(v.y) ); }

			float hashNoise(vec2 xy) {
				uint m = hash(floatBitsToUint(xy));

				m &= ieeeMantissa;
				m |= ieeeOne;

				return uintBitsToFloat( m ) - 1.0;
			}

			float pseudoRandom(float lower, float delta, in vec2 xy) {
				return lower + delta*hashNoise(xy);
			}

			vec3 pseudoRandomVec3(float lower, float upper, int index) {
				float delta = upper - lower;
				float x = pseudoRandom(lower, delta, vec2(index, 0));
				float y = pseudoRandom(lower, delta, vec2(index, 1));
				float z = pseudoRandom(lower, delta, vec2(index, 2));
				return vec3(x, y, z);
			}

			out vec3 vColor;

			void main()	{

				const float scale = 1.0/64.0;
				vec3 position = pseudoRandomVec3(-1.0, +1.0, gl_VertexID/3) + scale * pseudoRandomVec3(-1.0, +1.0, gl_VertexID);
				vec3 color = pseudoRandomVec3(0.25, 1.0, gl_VertexID/3);
				gl_Position = projectionMatrix * modelViewMatrix * vec4(position, 1.0 );
				vColor = color;

			}
        )";
        const std::string fragmentShader = R"(

			precision mediump float;

			in vec3 vColor;

			out vec4 fColor;

			void main()	{

				fColor = vec4(vColor, 1);

			}
        )";

        RawShaderMaterial::sptr material = RawShaderMaterial::create();
        material->vertexShader = vertexShader;
        material->fragmentShader = fragmentShader;
        material->side = Side::DoubleSide;
        material->glslVersion = GLSLVersion::GLSL3;

        Mesh::sptr mesh = Mesh::create( geometry, material );
        mesh->frustumCulled = false;
        scene->add( mesh );

        Vector4 screen = Vector4(0, 0, screenX, screenY);
        controller = std::make_shared<control::TrackballControls>(camera, screen);

        controller->staticMoving = false;
        controller->rotateSpeed = 4.0f;
        controller->zoomSpeed = 3;
        controller->panSpeed = 3;
        controller->noZoom = false;
        controller->noPan = false;
        controller->noRotate = false;
        controller->dynamicDampingFactor = 0.3f;

    }

    //virtual void render() override;


private:

};


#endif //THREE_CPP_GL_BUFFER_GEOMETRY_ATTRIBUTES_NONE_H
