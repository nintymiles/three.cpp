//
// Created by ultraman on 11/23/2023.
//

#include "gl_lights_spotlight.h"

void GLLightsSpotlight::render(){
    auto time = threecpp::getSystemTimeInMillis()/3000.0;

    spotLight->position.x = math::cos( time ) * 25;
    //spotLight->position.y = math::sin( time ) * 25;
    spotLight->position.z = math::sin( time ) * 25;

    spotlightHelper->update();

    ApplicationBase::render();
}