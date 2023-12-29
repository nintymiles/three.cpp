//
// Created by SeanR on 12/27/2023.
//

#include "gl_depth_texture.h"

void GLDepthTexture::render(){
#ifdef __APPLE__
    float delta = timer.getDelta()/1000.0; //no vsync for glfw on Mac
#else
    float delta = timer.getDelta();
#endif

//    // render scene into target
    renderer->setClearColor(0x000000,1.f);
    renderer->clear(true,true,false);
    renderer->setRenderTarget( target );
    //camera->updateProjectionMatrix();
    renderer->render( scene, camera );

    // render post FX
    postMaterial->uniforms->set("tDiffuse",target->texture);//tDiffuse.value = target.texture;
    postMaterial->uniforms->set<Texture::sptr>("tDepth",target->depthTexture);

    renderer->setRenderTarget( nullptr );
    renderer->render( postScene, postCamera );

//    controls.update(); // required because damping is enabled
//
//    stats.update();
//
//    ApplicationBase::render();
}

void GLDepthTexture::showControls(){

}