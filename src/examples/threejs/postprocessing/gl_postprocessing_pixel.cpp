//
// Created by ultraman on 12/21/2023.
//

#include "gl_postprocessing_pixel.h"

void GLPostprocessingPixel::render(){
//    ApplicationBase::render();

    grp->rotation.setY(grp->rotation.getY() + 0.0015f);
    grp->rotation.setZ(grp->rotation.getZ() + 0.001f);

    composer->render();
}

void GLPostprocessingPixel::showControls(){

}