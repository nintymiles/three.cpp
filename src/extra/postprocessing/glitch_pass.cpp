//
// Created by SeanR on 1/3/2024.
//

#include "glitch_pass.h"

#include "data_texture.h"
#include "shader_data.h"
#include "gl_renderer.h"
#include "gl_render_target.h"

namespace threecpp{
    GlitchPass::GlitchPass(int dt_size):Pass() {

        material = ShaderData::getDigitalGlitchShader();

        uniforms =  material->uniforms ;
        uniforms->set("tDisp", generateHeightmap(dt_size));

        fsQuad = std::make_shared<FullScreenQuad>( material );

        goWild = false;
        curF = 0;

        generateTrigger();
    }

    GlitchPass &GlitchPass::render(std::shared_ptr<GLRenderer> renderer, std::shared_ptr<GLRenderTarget> writeBuffer,
                                   std::shared_ptr<GLRenderTarget> readBuffer) {

        //if ( renderer.capabilities.isWebGL2 === false ) this.uniforms[ 'tDisp' ].value.format = THREE.LuminanceFormat;
        uniforms->set<Texture::sptr>("tDiffuse", readBuffer->texture); //diffuse texture

        uniforms->set("seed", math::random()); //default seeding
        uniforms->set("byp", 0);

        if ( curF % randX == 0 || goWild == true ) {
            uniforms->set("amount", math::random() / 30);
            uniforms->set("angle", math::randFloat( - math_number::PI, math_number::PI ));
            uniforms->set("seed_x", math::randFloat( -1,1 ));
            uniforms->set("seed_y", math::randFloat( -1,1 ));
            uniforms->set("distortion_x", math::randFloat( 0,1 ));
            uniforms->set("distortion_y", math::randFloat( 0,1 ));

            curF = 0;
            generateTrigger();

        } else if ( curF % randX < randX / 5 ) {

            uniforms->set("amount", math::random() / 90);
            uniforms->set("angle", math::randFloat( - math_number::PI, math_number::PI ));
            uniforms->set("seed_x", math::randFloat( -0.3,0.3 ));
            uniforms->set("seed_y", math::randFloat( -0.3,0.3 ));
            uniforms->set("distortion_x", math::randFloat( 0,1 ));
            uniforms->set("distortion_y", math::randFloat( 0,1 ));

        } else if ( goWild == false ) {

            uniforms->set("byp", 1);

        }

        curF ++;

        if ( renderToScreen ) {

            renderer->setRenderTarget( nullptr );
            fsQuad->render( renderer );

        } else {

            renderer->setRenderTarget( writeBuffer );
            if ( clear ) renderer->clear();
            fsQuad->render( renderer );

        }

        return *this;
    }

    Texture::sptr GlitchPass::generateHeightmap( int dt_size ) {
        auto length = dt_size * dt_size;
        std::vector<unsigned char> data_arr( length );

        for ( int i = 0; i < length; i ++ ) {
            auto val = math::randFloat( 0, 1 ) * 256;
            data_arr[ i ] = val;
        }

        auto texture = DataTexture::create( data_arr, dt_size, dt_size );
        texture->format = PixelFormat::RedFormat;
        texture->type = TextureDataType::FloatType;
        texture->needsUpdate = true;
        return texture;

    }

    GlitchPass& GlitchPass::generateTrigger() {
        randX = math::randInt( 120, 240 );

        return *this;
    }

    void GlitchPass::setSize(int width, int height) {}
}