//
// Created by SeanR on 2023/11/6.
//

#ifndef THREE_CPP_GL_LINES_DASHED_H
#define THREE_CPP_GL_LINES_DASHED_H

#include "application_model.h"
#include "color.h"
#include "line_basic_material.h"
#include "line.h"

#include "vector3.h"
#include "math_utils.h"
#include "geometry_utils.h"

#include <tuple>
#include <vector>


class GLLinesDashed : public ApplicationBase {
public:
    GLLinesDashed(int x, int y) : ApplicationBase(x,y) {}

    virtual void init() override {
        renderer->setClearColor(Color().setHex(0x000000));
        renderer->shadowMap->enabled = true;
        //renderer->shadowMap->type = ShadowMapType::PCFSoftShadowMap;

        camera = std::make_shared<PerspectiveCamera>(60.0f, screenX/screenY , 1.f, 200.0f);
        camera->position.set(0.0f, 0.0f, 150.0f);
        //camera->lookAt(Vector3());
        isPerspective = true;

        scene = std::make_shared<Scene>();
        scene->setBackgroundColor(Color().set(0x111111));

        const int subdivisions = 6;
        const int recursion = 1;

        std::vector<Vector3> points = geometry_utils::hilbert3D(Vector3( 0, 0, 0 ), 25.0, recursion, 0, 1, 2, 3, 4, 5, 6, 7);


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

    virtual void render() override;


private:
    const float r = 450;

    void rotateChildrenCallback(Object3D &e);

    BufferGeometry::sptr createGeometry() {
        std::vector<float> vertices;

        Vector3 vertex{};

        for ( int i = 0; i < 1500; i ++ ) {
            vertex.x = math::random() * 2 - 1;
            vertex.y = math::random() * 2 - 1;
            vertex.z = math::random() * 2 - 1;
            vertex.normalize();
            vertex.multiplyScalar( r );

            vertices.push_back(vertex.x);
            vertices.push_back(vertex.y);
            vertices.push_back(vertex.z);

            vertex.multiplyScalar( math::random() * 0.09 + 1 );

            vertices.push_back(vertex.x);
            vertices.push_back(vertex.y);
            vertices.push_back(vertex.z);
        }

        auto geometry = BufferGeometry::create();
        //geometry.setAttribute( 'position', new THREE.Float32BufferAttribute( vertices, 3 ) );
        geometry->setAttribute(AttributeName::position,BufferAttribute<float>::create(vertices, 3));

        return geometry;
    }
};


#endif //THREE_CPP_GL_LINES_DASHED_H
