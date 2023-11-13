//
// Created by SeanR on 2023/11/3.
//

#include "gl_lights_pointlights.h"

#include "obj_loader.h"
#include "common_utils.h"


void GLLightsPointLights::render(){
    float delta = timer.getDelta();
    float time = threecpp::getSystemTimeInMillis()/1000.0;

    if(objGroup)
        objGroup->rotation.setY(delta*0.5);

    light1->position.x = math::sin( time * 0.7 ) * 30;
    light1->position.y = math::cos( time * 0.5 ) * 40;
    light1->position.z = math::cos( time * 0.3 ) * 30;

    light2->position.x = math::cos( time * 0.3 ) * 30;
    light2->position.y = math::sin( time * 0.5 ) * 40;
    light2->position.z = math::sin( time * 0.7 ) * 30;

    light3->position.x = math::sin( time * 0.7 ) * 30;
    light3->position.y = math::cos( time * 0.3 ) * 40;
    light3->position.z = math::sin( time * 0.5 ) * 30;

    light4->position.x = math::sin( time * 0.3 ) * 30;
    light4->position.y = math::cos( time * 0.7 ) * 40;
    light4->position.z = math::sin( time * 0.5 ) * 30;

    ApplicationBase::render();
}
