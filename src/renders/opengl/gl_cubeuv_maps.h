//
// Created by SeanR on 12/15/2023.
//

#ifndef THREE_CPP_GL_CUBEUV_MAPS_H
#define THREE_CPP_GL_CUBEUV_MAPS_H

#include "texture.h"

#include "pmrem_generator.h"

class GLRenderer;

class GLCubeUVMaps {
private:
    GLRenderer* renderer;
    PMREMGenerator::sptr pmremGenerator;
    std::unordered_map<sole::uuid, GLRenderTarget::sptr> cubeuvmaps;
public:
    using sptr = std::shared_ptr<GLCubeUVMaps>;

    GLCubeUVMaps(GLRenderer* renderer): renderer(renderer){}

    static sptr create(GLRenderer* renderer) {
        return std::make_shared<GLCubeUVMaps>(renderer);
    }

    ~GLCubeUVMaps() = default;

    Texture::sptr get(const Texture::sptr texture){
        if(texture){
            auto mapping = texture->mapping;

            auto isEquirectMap = ( mapping == TextureMapping::EquirectangularReflectionMapping || mapping == TextureMapping::EquirectangularRefractionMapping );
            auto isCubeMap = ( mapping == TextureMapping::CubeReflectionMapping || mapping == TextureMapping::CubeRefractionMapping );

            // equirect/cube map to cubeUV conversion
            if ( isEquirectMap || isCubeMap ) {
                if ( texture->isRenderTargetTexture && texture->needsPMREMUpdate == true ) {
                    texture->needsPMREMUpdate = false;

                    auto renderTarget = cubeuvmaps.at( texture->uuid );

                    if ( pmremGenerator == nullptr ) pmremGenerator = PMREMGenerator::create( std::shared_ptr<GLRenderer>(renderer) );

                    renderTarget = isEquirectMap ? pmremGenerator->fromEquirectangular( texture, renderTarget ) : pmremGenerator->fromCubemap( texture, renderTarget );
                    cubeuvmaps.insert( {texture->uuid, renderTarget} );

                    return renderTarget->texture;

                } else {

                    if ( cubeuvmaps.find( texture->uuid ) != cubeuvmaps.end() ) {
                        return cubeuvmaps.at(texture->uuid)->texture;

                    } else {
                        //auto image = texture->image;

                        if ( ( isEquirectMap && texture->image->imageData.size()>0 && texture->image->height > 0 ) || ( isCubeMap && texture->images.size()>0 && isCubeTextureComplete( texture->images ) ) ) {
                            if ( pmremGenerator == nullptr ) pmremGenerator = PMREMGenerator::create( std::shared_ptr<GLRenderer>(renderer) );

                            auto renderTarget = isEquirectMap ? pmremGenerator->fromEquirectangular( texture ) : pmremGenerator->fromCubemap( texture );
                            cubeuvmaps.insert( {texture->uuid, renderTarget} );

                            //todo:fix this->add dispose procedure
                            //texture.addEventListener( 'dispose', onTextureDispose );

                            return renderTarget->texture;
                        } else {

                            // image not yet ready. try the conversion next frame
                            return nullptr;
                        }

                    }

                }

            }

        }

        return texture;
    }

    bool isCubeTextureComplete( std::vector<Texture::sptr> images ) {
        auto count = 0;
        auto length = 6;

        for ( auto i = 0; i < length; i ++ ) {
            if ( images[ i ] != nullptr ) count ++;

        }

        return count == length;

    }
};

#endif //THREE_CPP_GL_CUBEUV_MAPS_H
