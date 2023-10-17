//
// Created by SeanR on 2023/10/11.
//

#ifndef THREE_CPP_DEPTH_TEXTURE_H
#define THREE_CPP_DEPTH_TEXTURE_H

#include "texture.h"

#include "common_types.h"

class DepthTexture : public Texture {
private:
    size_t width, height;
public:
    using sptr = std::shared_ptr<DepthTexture>;

    DepthTexture() :Texture(),width(0), height(0) {
        isDepthTexture = true;
    }

    DepthTexture(size_t width, size_t height,
                 TextureMapping mapping = TextureMapping::UVMapping,
                 Wrapping wraps = Wrapping::ClampToEdgeWrapping,
                 Wrapping wrapt = Wrapping::ClampToEdgeWrapping,
                 TextureFilter magFilter = TextureFilter::NearestFilter,
                 unsigned anisotropy = 1,
                 PixelFormat  format = PixelFormat::None);
};

#endif //THREE_CPP_DEPTH_TEXTURE_H
