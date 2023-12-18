//
// Created by SeanR on 2023/10/11.
//

#ifndef THREE_CPP_GL_CUBE_MAPS_H
#define THREE_CPP_GL_CUBE_MAPS_H

#include "texture.h"
#include "gl_render_target.h"

class GLRenderer;
class GLCubeMaps {
private:
    GLRenderer* renderer;
//    std::unordered_map<std::string, Texture::sptr> cubemaps;
    std::unordered_map<sole::uuid, GLRenderTarget::sptr> cubemaps;

public:
    using sptr = std::shared_ptr<GLCubeMaps>;

    GLCubeMaps(GLRenderer* renderer): renderer(renderer){}

    static sptr create(GLRenderer* renderer) {
        return std::make_shared<GLCubeMaps>(renderer);
    }

    ~GLCubeMaps() = default;

    Texture::sptr get(const Texture::sptr texture){
        if ( texture && texture->isRenderTargetTexture == false ) {

            auto mapping = texture->mapping;

            if ( mapping == TextureMapping::EquirectangularReflectionMapping || mapping == TextureMapping::EquirectangularRefractionMapping ) {

                if ( cubemaps.find( texture->uuid ) != cubemaps.end() ) {
                    auto cubemap = cubemaps.at( texture->uuid )->texture;
                    return mapTextureMapping( cubemap, texture->mapping );

                } else {
                    auto image = texture->image;

                    if ( image.size()>0 ) {
                        auto renderTarget = GLCubeRenderTarget::create( texture->imageHeight / 2 );
                        //renderTarget->height = texture->imageHeight / 2;

                        renderTarget->fromEquirectangularTexture( renderer, texture );
                        cubemaps.insert( {texture->uuid, renderTarget} );

                       // texture.addEventListener( 'dispose', onTextureDispose );

                        return mapTextureMapping( renderTarget->texture, texture->mapping );
                    } else {
                        // image not yet ready. try the conversion next frame
                        return nullptr;

                    }

                }

            }

        }

        return texture;
    }

    Texture::sptr mapTextureMapping( Texture::sptr texture, TextureMapping mapping ) {
        if ( mapping == TextureMapping::EquirectangularReflectionMapping ) {
            texture->mapping = TextureMapping::CubeReflectionMapping;

        } else if ( mapping == TextureMapping::EquirectangularRefractionMapping ) {
            texture->mapping = TextureMapping::CubeRefractionMapping;

        }

        return texture;
    }

};

#endif //THREE_CPP_GL_CUBE_MAPS_H
