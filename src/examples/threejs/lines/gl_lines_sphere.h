//
// Created by SeanR on 2023/11/2.
//

#ifndef THREE_CPP_GL_LINES_SPHERE_H
#define THREE_CPP_GL_LINES_SPHERE_H

#include "application_model.h"
#include "color.h"
#include "line_basic_material.h"
#include "line.h"

#include "vector3.h"
#include "math_utils.h"

#include "trackball_control.h"

#include <tuple>
#include <vector>

class GLLinesSphere : public ApplicationBase{
public:
    GLLinesSphere(int x, int y) :  ApplicationBase(x,y) {}

    virtual void init() override {
        renderer->setClearColor(Color().setHex(0x000000));
        renderer->shadowMap->enabled = true;
        //renderer->shadowMap->type = ShadowMapType::PCFSoftShadowMap;

        camera = std::make_shared<PerspectiveCamera>(80.0f, screenX/screenY , 1.f, 3000.0f);
        camera->position.set(0.0f, 0.0f, 1000.0f);
        //camera->lookAt(Vector3());
        isPerspective = true;

        std::tuple<float,int,int> parameters[] = {{ 0.25, 0xff7700, 1 }, { 0.5, 0xff9900, 1 }, { 0.75, 0xffaa00, 0.75 }, { 1, 0xffaa00, 0.5 }, { 1.25, 0x000833, 0.8 },
                                                    { 3.0, 0xaaaaaa, 0.75 }, { 3.5, 0xffffff, 0.5 }, { 4.5, 0xffffff, 0.25 }, { 5.5, 0xffffff, 0.125 }};

        scene = std::make_shared<Scene>();
        scene->setBackgroundColor(Color().set(0x000000));

        for ( int i = 0; i < sizeof(parameters)/sizeof(std::tuple<float,int,int>); ++ i ) {
            std::tuple p = parameters[ i ];

            const auto material = LineBasicMaterial::create( Color(std::get<1>(p)) );
            //material.alphaMap;

            BufferGeometry::sptr geometry = createGeometry();

            auto line = LineSegments::create( geometry, material );
            line->scale.x = line->scale.y = line->scale.z = std::get<0>(p);
            //line.userData.originalScale = std::get<0>(p);
            line->rotation.setY(math::random() * M_PI);
            line->updateMatrix();
            scene->add(line);

        }

        Vector4 screen = Vector4(0, 0, screenX, screenY);
        std::shared_ptr<TrackballControls> tcontroller = std::make_shared<TrackballControls>(camera, screen);
        controller = tcontroller;

        tcontroller->staticMoving = false;
        tcontroller->rotateSpeed = 4.0f;
        tcontroller->zoomSpeed = 3;
        tcontroller->panSpeed = 3;
        tcontroller->noZoom = false;
        tcontroller->noPan = false;
        tcontroller->noRotate = false;
        tcontroller->dynamicDampingFactor = 0.3f;

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


#endif //THREE_CPP_GL_LINES_SPHERE_H
