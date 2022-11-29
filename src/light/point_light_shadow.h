//
// Created by seanren on 11/20/2022.
//

#ifndef THREE_CPP_POINT_LIGHT_SHADOW_H
#define THREE_CPP_POINT_LIGHT_SHADOW_H

#include "light_shadow.h"
#include "point_light.h"
#include "cameras/perspective_camera.h"

#include "matrix4.h"
#include "vector4.h"
#include "vector3.h"
#include "vector2.h"

Matrix4 _pointLightShadow_projScreenMatrix;
Vector3 _pointLightShadow_lightPositionWorld;
Vector3 _pointLightShadow_lookTarget;


class PointLightShadow:public LightShadow{
public:

    bool isPointLightShadow = true;

    Vector2 _frameExtents{ 4, 2 };

    int _viewportCount = 6;

    Vector4d _viewports[6] = {
        // positive X
        Vector4d(2, 1, 1, 1),
        // negative X
        Vector4d(0, 1, 1, 1),
        // positive Z
        Vector4d(3, 1, 1, 1),
        // negative Z
        Vector4d(1, 1, 1, 1),
        // positive Y
        Vector4d(3, 0, 1, 1),
        // negative Y
        Vector4d(1, 0, 1, 1)
    };

    Vector3 _cubeDirections[6] = {
        Vector3(1, 0, 0), Vector3(-1, 0, 0), Vector3(0, 0, 1),
        Vector3(0, 0, -1), Vector3(0, 1, 0), Vector3(0, -1, 0)
    };

    Vector3 _cubeUps[6] = {
        Vector3(0, 1, 0), Vector3(0, 1, 0), Vector3(0, 1, 0),
        Vector3(0, 1, 0), Vector3(0, 0, 1), Vector3(0, 0, -1)
    };


    PointLightShadow(): LightShadow(std::make_shared<PerspectiveCamera>(90,1,0.5,500)){}


    PointLightShadow& updateMatrices( std::shared_ptr<Light> light, int viewportIndex = 0 ) {
        PerspectiveCamera &camera = *dynamic_pointer_cast<PerspectiveCamera>(this->camera);//dynamic_cast<std::shared_ptr<PerspectiveCamera>>(this->camera);
        Matrix4 &shadowMatrix = this->matrix;

        const double far = std::dynamic_pointer_cast<PointLight>(light)->distance || camera.far;

        if ( far != camera.far ) {
            camera.far = far;
            camera.updateProjectionMatrix();
        }

        _pointLightShadow_lightPositionWorld.setFromMatrixPosition( *light->matrixWorld );
        camera.position->copy( _pointLightShadow_lightPositionWorld );

        _pointLightShadow_lookTarget.copy( *camera.position );
        _pointLightShadow_lookTarget.add( _cubeDirections[ viewportIndex ] );
        camera.up->copy( _cubeUps[ viewportIndex ] );
        camera.lookAt( _pointLightShadow_lookTarget.x,_pointLightShadow_lookTarget.y,_pointLightShadow_lookTarget.z );
        camera.updateMatrixWorld(true);

        shadowMatrix.makeTranslation( - _pointLightShadow_lightPositionWorld.x, - _pointLightShadow_lightPositionWorld.y, - _pointLightShadow_lightPositionWorld.z );

        _pointLightShadow_projScreenMatrix.multiplyMatrices( *camera.projectionMatrix, *camera.matrixWorldInverse );
        _frustum.setFromProjectionMatrix( _pointLightShadow_projScreenMatrix );

        return *this;
    }

};


#endif //THREE_CPP_POINT_LIGHT_SHADOW_H
