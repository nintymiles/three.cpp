//
// Created by SeanR on 2023/10/11.
//

#ifndef THREE_CPP_GL_RENDER_TARGET_H
#define THREE_CPP_GL_RENDER_TARGET_H

#include "common_types.h"
#include "depth_texture.h"

#include <unordered_map>
#include <memory>
#include <string>

using threecpp::TextureMapping;
using threecpp::Wrapping;
using threecpp::TextureFilter;
using threecpp::PixelFormat;
using threecpp::TextureDataType;
using threecpp::TextureEncoding;

struct GLRenderTargetParameter {
    TextureMapping mapping = TextureMapping::UVMapping;
    Wrapping wrapS = Wrapping::ClampToEdgeWrapping;
    Wrapping wrapT = Wrapping::ClampToEdgeWrapping;
    TextureFilter magFilter = TextureFilter::LinearFilter;
    TextureFilter minFilter = TextureFilter::LinearMipMapLinearFilter;
    PixelFormat format = PixelFormat::RGBAFormat;
    TextureDataType type = TextureDataType::UnsignedByteType;
    int anisotropy = 1;
    TextureEncoding encoding = TextureEncoding::LinearEncoding;
    bool generateMipmaps = false;
    bool depthBuffer = true;
    bool stencilBuffer = false;
    DepthTexture::sptr depthTexture = nullptr;
};

class GLRenderTarget {
    static size_t renderTargetId;
public:
    using sptr = std::shared_ptr<GLRenderTarget>;

    std::string type;
    size_t id;
    bool isGLMultiviewRenderTarget = false;
    int numViews = 0;
    sole::uuid uuid = sole::uuid1();
    size_t width = 0;
    size_t height = 0;
    bool scissorTest = false;

    Vector4f scissor;
    Vector4f viewport;

    Texture::sptr texture = nullptr;

    bool depthBuffer = false;
    bool stencilBuffer = false;

    DepthTexture::sptr depthTexture = nullptr;

    GLuint samples = 0;

    //ThreeDataTable options;
    std::unordered_map<std::string, int> options;

    bool isGLMultisampleRenderTarget = false;
    bool isGLCubeRenderTarget = false;

    GLRenderTarget() : id(renderTargetId++){
        type = "GLRenderTarget";
    }

    GLRenderTarget(size_t width, size_t height,GLRenderTargetParameter* options = nullptr);

    GLRenderTarget(GLRenderTarget& source);

    bool isEmpty()  const {
        return id == std::numeric_limits<size_t>::infinity() || id == 0;
    }
    virtual ~GLRenderTarget() = default;

    void setSize(size_t width, size_t height);

    GLRenderTarget& clone(GLRenderTarget* target);

    GLRenderTarget& copy(const GLRenderTarget& source);

    virtual void dispose();

    threecpp::Signal<void(GLRenderTarget&)> onDispose;



    GLRenderTarget& operator = (const GLRenderTarget& source) {
        return copy(*this);
    }
};

#endif //THREE_CPP_GL_RENDER_TARGET_H
