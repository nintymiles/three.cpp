//
// Created by SeanR on 2023/11/6.
//

#include "gl_lines_dashed.h"

namespace gl_line_dashed {
    void rotateChildrenCallback(Object3D &e) {
        std::chrono::time_point<std::chrono::steady_clock> time = std::chrono::steady_clock::now();
        auto timeInSecs = std::chrono::duration_cast<std::chrono::milliseconds>(time.time_since_epoch()).count();
        float timeF = timeInSecs * 0.001;

        if (e.isLine) {
            e.rotation.setX(0.25 * timeF);
            e.rotation.setY(0.25 * timeF);
        }
    }
}

void GLLinesDashed::render(){
    scene->traverse(&gl_line_dashed::rotateChildrenCallback);
    ApplicationBase::render();
}