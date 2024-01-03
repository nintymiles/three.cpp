//
// Created by SeanR on 1/3/2024.
//

#ifndef THREE_CPP_GL_POINTS_TRAILS_H
#define THREE_CPP_GL_POINTS_TRAILS_H

#include "application_model.h"
#include "color.h"

#include "points_material.h"
#include "points.h"

#include "vector3.h"
#include "math_utils.h"
#include "timer.h"

#include "trackball_control.h"

#include <tuple>
#include <vector>


class GLPointsTrails : public ApplicationBase {
public:
    GLPointsTrails(int x, int y) : ApplicationBase(x,y) {}

    virtual void init() override {
        renderer->setClearColor(Color().setHex(0x000000));
        renderer->shadowMap->enabled = true;
        renderer->autoClear = false;
        renderer->autoClearColor = false;

        camera = std::make_shared<PerspectiveCamera>(60.0f, screenX/screenY , .1f, 10.0f);
        camera->position.set(0.0f, 0.0f, 1.0f);

        isPerspective = true;

        scene = std::make_shared<Scene>();
        scene->setBackgroundColor(Color().set(0x111111));

        std::vector<Color> colorArray = { Color( 0xff0080 ), Color( 0xffffff ), Color( 0x8000ff ) };
        std::vector<float> positions{};
        std::vector<float> colors{};

        for ( int i = 0; i < 10; i ++ ) {

            positions.insert(positions.end(),{math::random() - 0.5f, math::random() - 0.5f, math::random() - 0.5f});

            auto clr = colorArray[ math::floor( math::random() * colorArray.size() ) ];

            colors.insert(colors.end(), {clr.r, clr.g, clr.b} );

        }

        auto geometry = BufferGeometry::create();
        geometry->setAttribute( AttributeName::position, BufferAttribute<float>::create( positions, 3 ) );
        geometry->setAttribute( AttributeName::color, BufferAttribute<float>::create( colors, 3 ) );

        auto material = PointsMaterial::create();
        material->size = 4;
        material->vertexColors = true;
        material->depthTest = false;
        material->sizeAttenuation = false;

        auto mesh = Points::create( geometry, material );
        scene->add( mesh );

        Vector4 screen = Vector4(0, 0, screenX, screenY);
        std::shared_ptr<TrackballControls> tcontroller = std::make_shared<TrackballControls>(camera, screen);
        controller = tcontroller;

//        tcontroller->staticMoving = false;
//        tcontroller->rotateSpeed = 4.0f;
//        tcontroller->zoomSpeed = 3;
//        tcontroller->panSpeed = 3;
//        tcontroller->noZoom = false;
//        tcontroller->noPan = false;
//        tcontroller->noRotate = false;
//        tcontroller->dynamicDampingFactor = 0.3f;

        timer = Timer();

    }

    virtual void render() override;


private:
    Timer timer;

};


#endif //THREE_CPP_GL_POINTS_TRAILS_H
