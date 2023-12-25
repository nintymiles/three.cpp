//
// Created by ultraman on 12/25/2023.
//

#include "gl_custom_attributes.h"

void GLCustomAttributes::render(){
#ifdef __APPLE__
    float delta = timer.getDelta()/1000.0; //no vsync for glfw on Mac
#else
    float delta = timer.getDelta();
#endif

    auto time = threecpp::getSystemTimeInMillis() * 0.01;

    sphere->rotation.set(0,0.01 * time,0.01 * time);

    uniforms->set("amplitude",2.5f * sin( sphere->rotation.y() * 0.125f ));
    uniforms->set("color",Color().setHSL(0.0005, 0, 0));

    for ( int i = 0; i < displacement.size(); i ++ ) {

        displacement[ i ] = sin( 0.1f * i + time );

        noise[ i ] += 0.5 * ( 0.5 - math::random() );
        noise[ i ] = math::clamp<double>( noise[ i ], - 5, 5 );

        displacement[ i ] += noise[ i ];
    }

    BufferGeometry::sptr bufferGeometr = std::dynamic_pointer_cast<BufferGeometry>(sphere->geometry);
    bufferGeometr->setAttribute(AttributeName::dispalcement,BufferAttribute<float>::create(displacement,1));
    bufferGeometr->attributes[{AttributeName::dispalcement,0}]->setNeedsUpdate(true);

    ApplicationBase::render();
}

void GLCustomAttributes::showControls(){

}