//
// Created by SeanR on 2024/01/03.
//

#include "gl_loader_gltf_demo.h"

void GLLoaderGLTFDemo::render(){

    const auto delta = timer.getDelta();

    //这个地方是动画的关键，此处才做具体的插值计算，应该接着更新uniform等需要的matrix等数据
    mixer.update( delta );

    ApplicationBase::render();
}
