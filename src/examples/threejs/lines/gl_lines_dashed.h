//
// Created by SeanR on 2023/11/6.
//

#ifndef THREE_CPP_GL_LINES_DASHED_H
#define THREE_CPP_GL_LINES_DASHED_H

#include "application_model.h"
#include "color.h"
#include "line_dashed_material.h"
#include "line.h"

#include "vector3.h"
#include "math_utils.h"
#include "geometry_utils.h"
#include "catmull_rom_cuve3.h"

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

        std::vector<Object3D::sptr> objects{};

        const int subdivisions = 6;
        const int recursion = 1;

        std::vector<Vector3> points = geometry_utils::hilbert3D(Vector3( 0, 0, 0 ), 25.0, recursion, 0, 1, 2, 3, 4, 5, 6, 7);
        CatmullRomCurve3 spline = CatmullRomCurve3( points );


        std::vector<Vector3> samples = spline.getPoints( points.size() * subdivisions );

        BufferGeometry::sptr geometrySpline = BufferGeometry::create();
        geometrySpline->setFromPoints( samples );

        Line::sptr line = Line::create( geometrySpline, LineDashedMaterial::create());
        line->computeLineDistances();

        objects.push_back(line);
        scene->add(line);

        BufferGeometry::sptr geometryBox = box( 50, 50, 50 );

        LineSegments::sptr lineSegments = LineSegments::create( geometryBox, LineDashedMaterial::create( Color(0xffaa00)) );
        lineSegments->computeLineDistances();

        objects.push_back( lineSegments );
        scene->add( lineSegments );


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

    //void rotateChildrenCallback(Object3D &e);

    BufferGeometry::sptr box( float width, float height, float depth ) {
        width = width * 0.5,
        height = height * 0.5,
        depth = depth * 0.5;

        BufferGeometry::sptr geometry = BufferGeometry::create();
        std::vector<float> position{
                -width, -height, -depth,
                -width, height, -depth,

                -width, height, -depth,
                width, height, -depth,

                width, height, -depth,
                width, -height, -depth,

                width, -height, -depth,
                -width, -height, -depth,

                -width, -height, depth,
                -width, height, depth,

                -width, height, depth,
                width, height, depth,

                width, height, depth,
                width, -height, depth,

                width, -height, depth,
                -width, -height, depth,

                -width, -height, -depth,
                -width, -height, depth,

                -width, height, -depth,
                -width, height, depth,

                width, height, -depth,
                width, height, depth,

                width, -height, -depth,
                width, -height, depth
        };

        geometry->setAttribute( AttributeName::position, BufferAttribute<float>::create( position, 3 ) );

        return geometry;
    }
};


#endif //THREE_CPP_GL_LINES_DASHED_H
