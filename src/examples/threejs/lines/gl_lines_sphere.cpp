//
// Created by SeanR on 2023/11/2.
//

#include "gl_lines_sphere.h"

#include <chrono>

void GLLinesSphere::rotateChildrenCallback(Object3D &e) {
    std::chrono::time_point<std::chrono::steady_clock> time = std::chrono::steady_clock::now();
    auto timeInMillis = std::chrono::duration_cast<std::chrono::milliseconds>(time.time_since_epoch()).count();
    static int i;
    static float originalScale = 0.8;

    if (e.isLine) {
        e.rotation.setY(timeInMillis * (i < 4 ? (i + 1) : -(i + 1)));

        if (i < 5) {
            float scale = originalScale * (i / 5 + 1) * (1 + 0.5 * math::sin(7.0 * timeInMillis));

            e.scale.x = e.scale.y = e.scale.z = scale;
        }
    }

}

void GLLinesSphere::render(){
//    camera->position.y += ( - mouseY + 200 - camera.position.y ) * .05;
    camera->lookAt( scene->position );

    //renderer.render( scene, camera );
    ApplicationBase::render();

    const float originalScale = 0.8;

    std::chrono::time_point<std::chrono::steady_clock> time = std::chrono::steady_clock::now();
    auto timeInMillis = std::chrono::duration_cast<std::chrono::milliseconds>(time.time_since_epoch()).count();
    float timeInterval = timeInMillis * 0.0001;

    for (int i = 0; i < scene->children.size(); i++ ) {
        auto object = scene->children[i];
        if ( object->isLine ) {
            object->rotation.setY(timeInterval * ( i < 4 ? ( i + 1 ) : - ( i + 1 )));
            if ( i < 5 ) {
                //注意sin函数要接受浮点类型，否则为0
                float scale = originalScale * ( i / 5 + 1 ) * ( 1 + 0.5 * math::sin( 7.0 * timeInterval ) );

                object->scale.x = object->scale.y = object->scale.z = scale;
            }
        }
    }

}