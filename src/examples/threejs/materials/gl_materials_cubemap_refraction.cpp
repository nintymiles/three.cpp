//
// Created by SeanR on 12/14/2023.
//

#include "gl_materials_cubemap_refraction.h"
#include "imgui.h"

void GLMaterialsCubeMapRefraction::render(){
    auto timer = - 0.0002 * threecpp::getSystemTimeInMillis();
    
    pointLight->position.x = 1500 * math::cos( timer );
    pointLight->position.z = 1500 * math::sin( timer );

    ApplicationBase::render();
}

void GLMaterialsCubeMapRefraction::showControls(){

}