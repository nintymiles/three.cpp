//
// Created by SeanR on 2023/10/11.
//

#ifndef THREE_CPP_GL_TEXTURES_H
#define THREE_CPP_GL_TEXTURES_H

#include "gl_extensions.h"
#include "gl_properties.h"
#include "gl_capabilities.h"
#include "gl_info.h"
#include "gl_state.h"

class GLRenderTarget;

class GLTextures {

private:
    GLExtensions::sptr extensions;

    GLState::sptr state;

    GLProperties::sptr properties;
    GLCapabilities::sptr capabilities;
    GLInfo::sptr info;

    bool isGLES3 = false;

    GLint maxTextures;

    GLsizei maxCubemapSize = 0;

    GLsizei maxTexturesize = 0;

    GLint maxSamples = 0;

    GLuint textureUnits = 0;



public:
    using sptr = std::shared_ptr<GLTextures>;

    GLTextures(GLExtensions::sptr& extensions, GLState::sptr& state, GLProperties::sptr& properties, GLCapabilities::sptr& capabilities, GLInfo::sptr& info);

    virtual ~GLTextures() = default;

    GLTextures& copy(const GLTextures& source) {
        extensions = source.extensions;
        state = source.state;
        properties = source.properties;
        capabilities = source.capabilities;
        info = source.info;
        isGLES3 = source.isGLES3;
        maxTextures = source.maxTextures;
        maxCubemapSize = source.maxCubemapSize;
        maxTexturesize = source.maxTexturesize;
        maxSamples = source.maxSamples;
        textureUnits = source.textureUnits;

        return *this;
    }
    GLTextures& operator = (const GLTextures& source) {
        return copy(source);
    }

private:

    void initTexture(Texture& texture);

    void setTextureParameters(TextureTarget textureType, Texture& texture, bool supportsMips);

    void uploadTexture(Texture& texture, unsigned slot);

    void updateVideoTexture(Texture& texture);

    bool isPowerOfTwo(const Texture& texture);

    bool textureNeedsPowerOfTwo(Texture& texture);

    std::vector<unsigned char> resizeImage(std::vector<unsigned char> image, unsigned inwidth,unsigned inheight,unsigned outwidth, unsigned outheight,int nChannel);

    std::vector<unsigned char> resizeImage(std::vector<unsigned char> image,GLsizei*inwidth,GLsizei*inheight,bool needsPowerOfTwo, GLsizei maxSize,int nChannel);

    GLint getInternalFormat(PixelFormat glFormat, TextureDataType glType);

    bool textureNeedsGenerateMipmaps(Texture& texture, bool supportsMips);

    void generateMipmap(TextureTarget target, Texture& texture, GLsizei width, GLsizei height);

    void deallocateTexture(Texture& texture);

    GLuint getRenderTargetSamples(GLRenderTarget& renderTarget);

    void setupRenderBufferStorage(GLuint renderbuffer, GLRenderTarget& renderTarget, bool isMultisample=false);

    void setupFrameBufferTexture(GLuint framebuffer, GLRenderTarget& renderTarget, GLuint attachment, GLenum textureTarget);

    void setupDepthRenderbuffer(GLRenderTarget& renderTarget);

    void setupDepthTexture(GLuint framebuffer, GLRenderTarget& renderTarget);

    void deallocateRenderTarget(GLRenderTarget& renderTarget);
public:
    GLuint allocateTextureUnit();

    void resetTextureUnits();

    void setTexture2D(Texture& texture, unsigned slot);

    void setTexture2DArray(Texture& texture, unsigned slot);

    void setTexture3D(Texture& texture, unsigned slot);

    void setTextureCube(Texture& texture, unsigned slot);

    void setTextureCubeDynamic(Texture& texture, unsigned slot);

    void setupRenderTarget(GLRenderTarget& renderTarget);

    void updateRenderTargetMipmap(GLRenderTarget& renderTarget);

    void updateMultisampleRenderTarget(GLRenderTarget& renderTarget);

    void safeSetTexture2D(Texture& texture, unsigned slot);

    void safeSetTextureCube(Texture& texture, unsigned slot);

    void onTextureDispose(Texture& texture);

    void onRenderTargetDispose(GLRenderTarget& renderTarget);
};

#endif //THREE_CPP_GL_TEXTURES_H
