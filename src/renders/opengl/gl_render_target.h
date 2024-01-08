//
// Created by SeanR on 2023/10/11.
//

#ifndef THREE_CPP_GL_RENDER_TARGET_H
#define THREE_CPP_GL_RENDER_TARGET_H

#include "common_types.h"
#include "depth_texture.h"
#include "vector4.h"

#include <unordered_map>
#include <memory>
#include <string>

struct GLRenderTargetParameter {
    TextureMapping mapping = TextureMapping::UVMapping;
    Wrapping wrapS = Wrapping::ClampToEdgeWrapping;
    Wrapping wrapT = Wrapping::ClampToEdgeWrapping;
    TextureFilter magFilter = TextureFilter::LinearFilter;
    TextureFilter minFilter = TextureFilter::LinearMipmapLinearFilter;
    PixelFormat format = PixelFormat::RGBAFormat;
    TextureDataType type = TextureDataType::UnsignedByteType;
    int anisotropy = 1;
    TextureEncoding encoding = TextureEncoding::LinearEncoding;
    bool generateMipmaps = false;
    bool depthBuffer = true;
    bool stencilBuffer = false;
    DepthTexture::sptr depthTexture = nullptr;
    //int multisample = 1;

    using sptr = std::shared_ptr<GLRenderTargetParameter>;
    static sptr create(){
        return std::make_shared<GLRenderTargetParameter>();
    }
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
    size_t depth = 1;
    bool scissorTest = false;

    Vector4 scissor;
    Vector4 viewport;

    Texture::sptr texture = nullptr;

    //depthBuffer default = true
    bool depthBuffer = true;
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

    GLRenderTarget(size_t width, size_t height,GLRenderTargetParameter::sptr options = nullptr);

    static sptr create(){
        return std::make_shared<GLRenderTarget>();
    }

    static sptr create(size_t width, size_t height,GLRenderTargetParameter::sptr options = nullptr){
        return std::make_shared<GLRenderTarget>(width,height,options);
    }

    GLRenderTarget(GLRenderTarget& source);

    bool isEmpty()  const {
        return id == std::numeric_limits<size_t>::max() || id == 0;
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

class GLMultisampleRenderTarget : public GLRenderTarget {
public:
    GLMultisampleRenderTarget() :GLRenderTarget() {
        type = "GLMultisampleRenderTarget";
        isGLMultisampleRenderTarget = true;
    }
};

#endif //THREE_CPP_GL_RENDER_TARGET_H
