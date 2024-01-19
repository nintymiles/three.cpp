//
// Created by SeanR on 12/17/2023.
//

#ifndef THREE_CPP_CUBE_CAMERA_H
#define THREE_CPP_CUBE_CAMERA_H

#include "object_3d.h"

#include "gl_render_target.h"
#include "perspective_camera.h"
#include "gl_renderer.h"

class CubeCamera: public Object3D{
    const float fov = - 90, aspect = 1;
    GLRenderTarget::sptr renderTarget;

public:
    CubeCamera(float _near,float _far,GLRenderTarget::sptr renderTarget):Object3D(){
        type = "CubeCamera";

        this->renderTarget = renderTarget;

        auto cameraPX = PerspectiveCamera::create( fov, aspect, _near, _far );
        cameraPX->layers = this->layers;
        cameraPX->up.set( 0, 1, 0 );
        cameraPX->lookAt( Vector3( 1, 0, 0 ) );
        add( cameraPX );

        auto cameraNX = PerspectiveCamera::create( fov, aspect,  _near, _far );
        cameraNX->layers = this->layers;
        cameraNX->up.set( 0, 1, 0 );
        cameraNX->lookAt( Vector3( - 1, 0, 0 ) );
        add( cameraNX );

        auto cameraPY = PerspectiveCamera::create( fov, aspect,  _near, _far );
        cameraPY->layers = this->layers;
        cameraPY->up.set( 0, 0, - 1 );
        cameraPY->lookAt( Vector3( 0, 1, 0 ) );
        add( cameraPY );

        auto cameraNY = PerspectiveCamera::create( fov, aspect,  _near, _far );
        cameraNY->layers = this->layers;
        cameraNY->up.set( 0, 0, 1 );
        cameraNY->lookAt( Vector3( 0, - 1, 0 ) );
        add( cameraNY );

        auto cameraPZ = PerspectiveCamera::create( fov, aspect,  _near, _far );
        cameraPZ->layers = this->layers;
        cameraPZ->up.set( 0, 1, 0 );
        cameraPZ->lookAt( Vector3( 0, 0, 1 ) );
        add( cameraPZ );

        auto cameraNZ = PerspectiveCamera::create( fov, aspect,  _near, _far );
        cameraNZ->layers = this->layers;
        cameraNZ->up.set( 0, 1, 0 );
        cameraNZ->lookAt( Vector3( 0, 0, - 1 ) );
        add( cameraNZ );
    }

    void update( GLRenderer* renderer, Scene::sptr scene ) {

        if ( this->parent == nullptr ) this->updateMatrixWorld();

        auto renTarget = this->renderTarget;

        //const [ cameraPX, cameraNX, cameraPY, cameraNY, cameraPZ, cameraNZ ] = this.children;
        auto cameraPX = std::dynamic_pointer_cast<Camera>(children[0]);
        auto cameraNX = std::dynamic_pointer_cast<Camera>(children[1]);
        auto cameraPY = std::dynamic_pointer_cast<Camera>(children[2]);
        auto cameraNY = std::dynamic_pointer_cast<Camera>(children[3]);
        auto cameraPZ = std::dynamic_pointer_cast<Camera>(children[4]);
        auto cameraNZ = std::dynamic_pointer_cast<Camera>(children[5]);

        auto currentRenderTarget = renderer->getRenderTarget();

        auto currentToneMapping = renderer->toneMapping;
        //const currentXrEnabled = renderer.xr.enabled;

        renderer->toneMapping = ToneMapping::NoToneMapping;
        //renderer->xr.enabled = false;

        auto generateMipmaps = renTarget->texture->generateMipmaps;

        renTarget->texture->generateMipmaps = false;

//        GLRenderTarget::sptr rendTarget;
//        rendTarget.reset(renderTarget);
        renderer->setRenderTarget( renTarget, 0 );
        renderer->render( scene, cameraPX );

        renderer->setRenderTarget( renTarget, 1 );
        renderer->render( scene, cameraNX );

        renderer->setRenderTarget( renTarget, 2 );
        renderer->render( scene, cameraPY );

        renderer->setRenderTarget( renTarget, 3 );
        renderer->render( scene, cameraNY );

        renderer->setRenderTarget( renTarget, 4 );
        renderer->render( scene, cameraPZ );

        renTarget->texture->generateMipmaps = generateMipmaps;

        renderer->setRenderTarget( renTarget, 5 );
        renderer->render( scene, cameraNZ );

        renderer->setRenderTarget( currentRenderTarget );

        renderer->toneMapping = currentToneMapping;
        //renderer.xr.enabled = currentXrEnabled;

        //renderTarget->texture->needsPMREMUpdate = true;

    }

};




#endif //THREE_CPP_CUBE_CAMERA_H
