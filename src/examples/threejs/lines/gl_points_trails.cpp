//
// Created by SeanR on 1/3/2024.
//

#include "gl_points_trails.h"

#include "imgui.h"

void GLPointsTrails::render() {
    auto time = timer.get_elapsed_time();

    scene->rotation.setY( time * 0.5 );

    //renderer.render( scene, camera );

    ApplicationBase::render();
}