//
// Created by SeanR on 11/24/2023.
//

#include "spotlight_helper.h"

#include "vector3.h"
#include "buffer_geometry.h"
#include "line_basic_material.h"
#include "line.h"

namespace spotlight_helper{
    Vector3 _vector;
}

using namespace spotlight_helper;

SpotlightHelper::SpotlightHelper(Light::sptr light,Color color):Object3D(){
//    Object3D::Object3D();
    this->light = light;
    this->light->updateMatrixWorld();
    this->color = color;

    this->matrix = light->matrixWorld;
    this->matrixAutoUpdate = false;

    BufferGeometry::sptr geometry = BufferGeometry::create();

    std::vector<float> positions = {
            0, 0, 0, 0, 0, 1,
            0, 0, 0, 1, 0, 1,
            0, 0, 0, -1, 0, 1,
            0, 0, 0, 0, 1, 1,
            0, 0, 0, 0, -1, 1
    };

    for ( int i = 0, j = 1, l = 32; i < l; i ++, j ++ ) {
        float p1 = ( (float)i / l ) * math_number::PI2;
        float p2 = ( (float)j / l ) * math_number::PI2;

        positions.push_back(math::cos( p1 ));
        positions.push_back(math::sin( p1 ));
        positions.push_back(1);
        positions.push_back(math::cos( p2 ));
        positions.push_back(math::sin( p2 ));
        positions.push_back(1);
    }

    geometry->setAttribute( AttributeName::position, BufferAttribute<float>::create(positions,3));

    LineBasicMaterial::sptr material = LineBasicMaterial::create();
    material->fog = false;
    material->toneMapped = false;

    this->cone = LineSegments::create(geometry,material);

    this->add( this->cone );

    this->update();
}

SpotlightHelper& SpotlightHelper::update() {
    this->light->updateMatrixWorld();

    float coneLength = this->light->distance > 0 ? this->light->distance : 1000;
    float coneWidth = coneLength * math::tan( this->light->angle );

    this->cone->scale.set( coneWidth, coneWidth, coneLength );

    _vector.setFromMatrixPosition( this->light->target->matrixWorld );

    this->cone->lookAt( _vector );

    if ( !this->color.isNull() ) {
        this->cone->material->color.set( this->color );
    } else {
        this->cone->material->color.copy( this->light->color );
    }
    return *this;
}
