//
// Created by SeanR on 2023/10/11.
//
#include "gl_textures.h"

#include "gl_render_target.h"
#include "gl_properties.h"

#include "number.h"
#include "texture_loader.h"

#include <cassert>
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb_image_resize.h>

namespace gltextures {
    //ThreeDataTable _videoTextures;
    std::unordered_map<sole::uuid, int> _videoTextures;
}
using namespace gltextures;
GLTextures::GLTextures(GLExtensions::sptr& extensions, GLState::sptr& state, GLProperties::sptr& properties, GLCapabilities::sptr& capabilities, GLInfo::sptr& info)
        :extensions(extensions), state(state), properties(properties), capabilities(capabilities), info(info){
    isGLES3 = capabilities->isGLES3;

    maxTextures = capabilities->maxTextures;

    maxCubemapSize = capabilities->maxCubemapSize;

    maxTexturesize = capabilities->maxTextureSize;

    maxSamples = capabilities->maxSamples;

}

void GLTextures::initTexture(Texture& texture){
    auto& textureProperties = properties->getProperties(texture.uuid);
    if(!textureProperties.glInit){
        textureProperties.glInit = true;
        texture.onDispose.connect(*this, &GLTextures::onTextureDispose);

        GLuint tex;
        glGenTextures(1, &tex);
        textureProperties.texture = tex;
        texture.id = tex;
        info->memory.textures++;

    }
}

void GLTextures::setTextureParameters(TextureTarget textureType, Texture& texture, bool supportsMips){
    if (supportsMips) {

        glTexParameteri((GLuint)textureType, GL_TEXTURE_WRAP_S, (GLuint)texture.wrapS);
        glTexParameteri((GLuint)textureType, GL_TEXTURE_WRAP_T, (GLuint)texture.wrapT);

        if (textureType ==  TextureTarget::Texture3D || textureType == TextureTarget::Texture2DArray) {

            if(texture.isDataTexture3D)
                glTexParameteri((GLuint)textureType, GL_TEXTURE_WRAP_R, (GLuint)texture.wrapR);

        }

        glTexParameterf((GLuint)textureType, GL_TEXTURE_MAG_FILTER, (GLuint)texture.magFilter);
        glTexParameterf((GLuint)textureType, GL_TEXTURE_MIN_FILTER, (GLuint)texture.minFilter);

    } else {
        glTexParameteri((GLuint)textureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri((GLuint)textureType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        if (textureType == TextureTarget::Texture3D || textureType == TextureTarget::Texture2DArray) {

            glTexParameteri((GLuint)textureType,GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        }

        /*if (texture->wrapS != = ClampToEdgeWrapping || texture->wrapT != = ClampToEdgeWrapping) {
            console.warn('THREE.WebGLRenderer: Texture is not power of two. Texture.wrapS and Texture.wrapT should be set to THREE.ClampToEdgeWrapping.');
        }*/

        glTexParameterf((GLuint)textureType, GL_TEXTURE_MAG_FILTER, (GLuint)texture.magFilter);
        glTexParameterf((GLuint)textureType, GL_TEXTURE_MIN_FILTER, (GLuint)texture.minFilter);

        /*if (texture->minFilter != = NearestFilter && texture->minFilter != = LinearFilter) {
            console.warn('THREE.WebGLRenderer: Texture is not power of two. Texture.minFilter should be set to THREE.NearestFilter or THREE.LinearFilter.');
        }*/

    }

    bool extension = extensions->get(Extension::EXT_texture_filter_anisotropic);

    if (extension) {
        if (texture.type == TextureDataType::FloatType && extensions->get(Extension::OES_texture_float_linear) == false) return;
        if (texture.type == TextureDataType::HalfFloatType && (isGLES3 || extensions->get(Extension::OES_texture_half_float_linear)) == false) return;

        auto &textureProperties = properties->getProperties(texture.uuid);

        if (texture.anisotropy > 1 || textureProperties.currentAnisotropy) {
            //glTexParameterf((GLuint)textureType, GL_TEXTURE_MAX_ANISOTROPY, std::min((float)texture.anisotropy, capabilities->getMaxAnisotropy()));
            textureProperties.currentAnisotropy=texture.anisotropy;
        }
    }
}

void GLTextures::uploadTexture(Texture& texture, unsigned slot){
    auto textureType = TextureTarget::Texture2D;

    if (texture.isDataTexture2DArray) textureType = TextureTarget::Texture2DArray;

    if (texture.isDataTexture3D) textureType = TextureTarget::Texture3D;

    initTexture(texture);

    //state->activeTexture(GL_TEXTURE0 + slot);

    auto& textureProperties = properties->getProperties(texture.uuid);

    state->bindTexture(textureType, textureProperties.texture,GL_TEXTURE0 + slot);

    //glPixelStorei(GL_UNPACK_FLIP_Y_WEBGL, texture.flipY);
    //glPixelStorei(GL_UNPACK_PREMULTIPLY_ALPHA_WEBGL, texture.premultiplyAlpha);
    glPixelStorei(GL_UNPACK_ALIGNMENT, texture.unpackAlignment);

    bool needsPowerOfTwo = textureNeedsPowerOfTwo(texture) && isPowerOfTwo(texture) ==  false;
    //todo:fix this - is resizeImage necessary?
    std::vector<unsigned char> image = resizeImage(texture.image, &texture.imageWidth,&texture.imageHeight,needsPowerOfTwo, maxTexturesize,texture.channel);
    //std::vector<unsigned char> &image = texture.image;

    bool supportsMips = isPowerOfTwo(texture) ? isPowerOfTwo(texture) : isGLES3;
    PixelFormat glFormat = texture.format;
    TextureDataType glType = texture.type;
    GLint glInternalFormat = getInternalFormat(glFormat, glType);

    setTextureParameters(textureType, texture, supportsMips);

    threecpp::MipMap mipmap;
    std::vector<threecpp::MipMap>  mipmaps = texture.mipmaps;

    if (texture.isDepthTexture) {
        // populate depth texture with dummy data
        glInternalFormat = GL_DEPTH_COMPONENT;

        if (texture.type == TextureDataType::FloatType) {
            //if (isGLES3 == false) throw new Error('Float Depth Texture only supported in WebGL2.0');
            glInternalFormat = GL_DEPTH_COMPONENT32F;
        } else if (isGLES3) {
            // WebGL 2.0 requires signed internalformat for glTexImage2D
            glInternalFormat = GL_DEPTH_COMPONENT16;
        }

        if (texture.format == PixelFormat::DepthFormat && glInternalFormat == GL_DEPTH_COMPONENT) {
            // The error INVALID_OPERATION is generated by texImage2D if format and internalformat are
            // DEPTH_COMPONENT and type is not UNSIGNED_SHORT or UNSIGNED_INT
            // (https://www.khronos.org/registry/webgl/extensions/WEBGL_depth_texture/)
            if (texture.type !=  TextureDataType::UnsignedShortType && texture.type != TextureDataType::UnsignedIntType) {
                //console.warn('THREE.WebGLRenderer: Use UnsignedShortType or UnsignedIntType for DepthFormat DepthTexture.');
                texture.type = TextureDataType::UnsignedShortType;
                glType = TextureDataType::UnsignedShortType;
            }

        }

        // Depth stencil textures need the DEPTH_STENCIL internal format
        // (https://www.khronos.org/registry/webgl/extensions/WEBGL_depth_texture/)
        if (texture.format == PixelFormat::DepthStencilFormat) {

            glInternalFormat = GL_DEPTH_STENCIL;

            // The error INVALID_OPERATION is generated by texImage2D if format and internalformat are
            // DEPTH_STENCIL and type is not UNSIGNED_INT_24_8_WEBGL.
            // (https://www.khronos.org/registry/webgl/extensions/WEBGL_depth_texture/)
            if (texture.type != TextureDataType::UnsignedInt248Type) {

                //console.warn('THREE.WebGLRenderer: Use UnsignedInt248Type for DepthStencilFormat DepthTexture.');

                texture.type = TextureDataType::UnsignedInt248Type;
                glType = TextureDataType::UnsignedInt248Type;

            }

        }
        state->texImage2D((GLenum)TextureTarget::Texture2D, 0,(GLenum)glInternalFormat,(GLsizei) texture.imageWidth, (GLsizei)texture.imageHeight, 0,(GLenum)glFormat, (GLenum)glType);

    }
    else if (texture.isDataTexture) {
        // use manually created mipmaps if available
        // if there are no manual mipmaps
        // set 0 level mipmap and then use GL to generate other mipmap levels

        if (mipmaps.size() > 0 && supportsMips) {

            for (unsigned i = 0;i < mipmaps.size(); i++) {

                mipmap = mipmaps[i];
                state->texImage2D(GL_TEXTURE_2D, i, glInternalFormat, mipmap.width, mipmap.height, 0, (GLenum)glFormat, (GLenum)glType, mipmap.data);

            }

            texture.generateMipmaps = false;

            textureProperties.maxMipLevel =mipmaps.size() - 1;

        }
        else {
            state->texImage2D(GL_TEXTURE_2D, 0, glInternalFormat, texture.imageWidth, texture.imageHeight, 0, (GLenum)glFormat, (GLenum)glType, image);
            textureProperties.maxMipLevel= 0;
        }

    }else if (texture.isCompressedTexture) {

        for (unsigned i = 0;i< mipmaps.size();i++) {

            mipmap = mipmaps[i];

            if (texture.format != PixelFormat::RGBAFormat && texture.format !=  PixelFormat::RGBFormat) {

                if (glFormat != PixelFormat::None) {
                    state->compressedTexImage2D(GL_TEXTURE_2D, i, glInternalFormat, mipmap.width, mipmap.height,mipmap.data);

                }
                /*else {
                    console.warn('THREE.GLRenderer: Attempt to load unsupported compressed texture format in .uploadTexture()');
                }*/

            }
            else {
                state->texImage2D(GL_TEXTURE_2D, i, glInternalFormat, mipmap.width, mipmap.height, 0, (GLenum)glFormat, (GLenum)glType, mipmap.data);
            }

        }
        textureProperties.maxMipLevel = mipmaps.size() - 1;
    }
    else if (texture.isDataTexture2DArray) {

        state->texImage3D(GL_TEXTURE_2D_ARRAY, 0, glInternalFormat, texture.imageWidth, texture.imageHeight, texture.depth, 0, (GLenum)glFormat, (GLenum)glType, image);
        textureProperties.maxMipLevel = 0;
    }
    else if (texture.isDataTexture3D) {

        state->texImage3D(GL_TEXTURE_3D, 0, glInternalFormat, texture.imageWidth, texture.imageHeight, texture.depth, 0, (GLenum)glFormat, (GLenum)glType, image);
        textureProperties.maxMipLevel = 0;
    }
    else {
        // regular Texture (image, video, canvas)

        // use manually created mipmaps if available
        // if there are no manual mipmaps
        // set 0 level mipmap and then use GL to generate other mipmap levels

        if (mipmaps.size() > 0 && supportsMips) {
            for (unsigned i = 0;i< mipmaps.size();i++) {

                mipmap = mipmaps[i];
                state->texImage2D(GL_TEXTURE_2D, i, glInternalFormat, mipmap.width,mipmap.height,0,(GLenum)glFormat, (GLenum)glType, mipmap.data);

            }

            texture.generateMipmaps = false;
            textureProperties.maxMipLevel = mipmaps.size() - 1;
        }
        else {
            state->texImage2D(GL_TEXTURE_2D, 0, glInternalFormat, texture.imageWidth,texture.imageHeight,0,(GLenum)glFormat, (GLenum)glType, image.data());
            textureProperties.maxMipLevel =0;
        }

    }

    if (textureNeedsGenerateMipmaps(texture, supportsMips)) {
        generateMipmap(textureType, texture, texture.imageWidth, texture.imageHeight);
    }

    textureProperties.version =texture.version;

    texture.onUpdate;
}

void GLTextures::updateVideoTexture(Texture& texture){
    unsigned frame = info->render.frame;

    // Check the last frame we updated the VideoTexture


    if (_videoTextures.count(texture.uuid)>0) {
        unsigned _frame = _videoTextures[texture.uuid];
        if (frame != _frame) {
            _frame = frame;
            _videoTextures[texture.uuid] = _frame;
            texture.updateTexture();
        }
    }
    else {
        _videoTextures[texture.uuid] =frame;
    }
    /*if (_videoTextures.get(texture) != = frame) {
        _videoTextures.set(texture, frame);
        texture.update();

    }*/
}

bool GLTextures::isPowerOfTwo(const Texture& texture){
    return math::isPowerOfTwo(texture.imageWidth) && math::isPowerOfTwo(texture.imageHeight);
}

bool GLTextures::textureNeedsPowerOfTwo(Texture& texture){
    if (isGLES3) return false;

    return (texture.wrapS !=  Wrapping::ClampToEdgeWrapping || texture.wrapT !=  Wrapping::ClampToEdgeWrapping) ||
           (texture.minFilter !=  TextureFilter::NearestFilter && texture.minFilter !=  TextureFilter::LinearFilter);
}

std::vector<unsigned char> GLTextures::resizeImage(std::vector<unsigned char> image, unsigned inwidth,unsigned inheight,unsigned outwidth, unsigned outheight,int nChannel){
    std::vector<unsigned char> output_image(outwidth * outheight * nChannel);

    //output_image = (unsigned char*)malloc(outwidth * outheight * nChannel);
    //todo:fix here
    stbir_resize_uint8(&image[0], inwidth, inheight, 0, output_image.data(), outwidth, outheight, 0, nChannel);

    return output_image;
}

std::vector<unsigned char> GLTextures::resizeImage(std::vector<unsigned char> image, GLsizei* inwidth,GLsizei* inheight,bool needsPowerOfTwo, GLsizei maxSize,int nChannel){
    float scale = 1.0f;

    if (*inwidth > maxSize || *inheight > maxSize)
    {
        scale = maxSize / (float)std::max(*inwidth, *inheight);
    }

    if (scale <= 1 || needsPowerOfTwo == true){
        if (image.size() >= 0){
            unsigned width = needsPowerOfTwo ? math::floorPowerOfTwo(scale * *inwidth) : (float)math::floor(scale * *inwidth);
            unsigned height = needsPowerOfTwo ? math::floorPowerOfTwo(scale * *inheight) : (float)math::floor(scale * *inheight);

            image = resizeImage(image, *inwidth,*inheight,width, height,nChannel);
            *inwidth = width;
            *inheight = height;
            return image;

        }
        else
        {
            return image;
        }
    }
    return image;
}

GLint GLTextures::getInternalFormat(PixelFormat glFormat, TextureDataType glType){
    if (isGLES3 == false) return (GLuint)glFormat;

    GLuint internalFormat = (GLuint)glFormat;

    if (glFormat ==  PixelFormat::RedFormat) {
        if (glType == TextureDataType::FloatType) internalFormat = GL_R32F;
        if (glType == TextureDataType::HalfFloatType) internalFormat = GL_R16F;
        if (glType == TextureDataType::UnsignedByteType) internalFormat = GL_R8;

    }

    if (glFormat == PixelFormat::RGBFormat) {
        if (glType == TextureDataType::FloatType) internalFormat = GL_RGB32F;
        if (glType == TextureDataType::HalfFloatType) internalFormat = GL_RGB16F;
        if (glType == TextureDataType::UnsignedByteType) internalFormat = GL_RGB8;

    }

    if (glFormat == PixelFormat::RGBAFormat) {

        if (glType == TextureDataType::FloatType) internalFormat = GL_RGBA32F;
        if (glType == TextureDataType::HalfFloatType) internalFormat = GL_RGBA16F;
        if (glType == TextureDataType::UnsignedByteType) internalFormat = GL_RGBA8;

    }

    if (glFormat == PixelFormat::DepthFormat) {

//        if (glType == TextureDataType::FloatType) internalFormat = GL_FLOAT;
//        if (glType == TextureDataType::UnsignedIntType) internalFormat = GL_UNSIGNED_INT;
//        if (glType == TextureDataType::UnsignedByteType) internalFormat = GL_DEPTH_COMPONENT16;
        internalFormat = GL_DEPTH_COMPONENT16;

    }

    if (glFormat == PixelFormat::DepthStencilFormat) {

//        if (glType == TextureDataType::FloatType) internalFormat = GL_DEPTH24_STENCIL8;
//        if (glType == TextureDataType::UnsignedByteType) internalFormat = GL_DEPTH24_STENCIL8;
        internalFormat = GL_DEPTH24_STENCIL8;

    }


    if (internalFormat == GL_R16F || internalFormat ==  GL_R32F ||
        internalFormat == GL_RGBA16F || internalFormat ==  GL_RGBA32F) {
        //extensions->get('EXT_color_buffer_float');
    }

    return internalFormat;
}

bool GLTextures::textureNeedsGenerateMipmaps(Texture& texture, bool supportsMips){
    return texture.generateMipmaps && supportsMips &&
           texture.minFilter != TextureFilter::NearestFilter && texture.minFilter !=  TextureFilter::LinearFilter;return false;
}

void GLTextures::generateMipmap(TextureTarget target, Texture& texture, GLsizei width, GLsizei height){
    glGenerateMipmap((GLenum)target);

    auto& textureProperties = properties->getProperties(texture.uuid);

    float v = (float)std::log(std::max(width, height)) * (float)std::log2(math_number::E);

    textureProperties.maxMipLevel = v;
}

void GLTextures::deallocateTexture(Texture& texture){
    auto& textureProperties = properties->getProperties(texture.uuid);

    if (!textureProperties.glInit) return;

    GLuint textureId = textureProperties.texture;
    if(textureId!=0)
        glDeleteTextures(1,&textureId);

    properties->remove(texture.uuid);
}

GLuint GLTextures::getRenderTargetSamples(GLRenderTarget& renderTarget){
    return isGLES3 && renderTarget.isGLMultisampleRenderTarget ? std::min((int)maxSamples, (int)renderTarget.samples) : 0;
}

void GLTextures::setupRenderBufferStorage(GLuint renderbuffer, GLRenderTarget& renderTarget, bool isMultisample){
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);

    if (renderTarget.depthBuffer && !renderTarget.stencilBuffer) {

        if (isMultisample) {

            GLuint samples = getRenderTargetSamples(renderTarget);

            glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH_COMPONENT16, renderTarget.width, renderTarget.height);

        }
        else {

            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, renderTarget.width, renderTarget.height);

        }

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);

    }else if (renderTarget.depthBuffer && renderTarget.stencilBuffer) {

        if (isMultisample) {

            GLuint samples = getRenderTargetSamples(renderTarget);

            glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, renderTarget.width, renderTarget.height);

        }
        else {

            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_STENCIL, renderTarget.width, renderTarget.height);

        }

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);
    }
    else {

        PixelFormat glFormat = renderTarget.texture->format;
        TextureDataType glType = renderTarget.texture->type;
        GLint glInternalFormat = getInternalFormat(glFormat, glType);

        if (isMultisample) {

            GLuint samples = getRenderTargetSamples(renderTarget);

            glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, glInternalFormat, renderTarget.width, renderTarget.height);

        }else {
            glRenderbufferStorage(GL_RENDERBUFFER, glInternalFormat, renderTarget.width, renderTarget.height);
        }
    }

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void GLTextures::setupFrameBufferTexture(GLuint framebuffer, GLRenderTarget& renderTarget, GLuint attachment, GLenum textureTarget){
    PixelFormat glFormat = renderTarget.texture->format;
    TextureDataType glType = renderTarget.texture->type;
    GLint glInternalFormat = getInternalFormat(glFormat, glType);
    state->texImage2D(textureTarget, 0, glInternalFormat, renderTarget.width, renderTarget.height, 0, (GLenum)glFormat, (GLenum)glType);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    auto& renderTargetTextureProperties = properties->getProperties(renderTarget.texture->uuid);
    GLint textureId = renderTargetTextureProperties.texture;

    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment,textureTarget, textureId, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void GLTextures::setupDepthRenderbuffer(GLRenderTarget& renderTarget){
    auto& renderTargetProperties = properties->getProperties(renderTarget.uuid);

    bool isCube = renderTarget.isGLCubeRenderTarget;

    if (renderTarget.depthTexture) {
        assert(("target.depthTexture not supported in Cube render targets", !isCube));
        //if (isCube) throw new Error('target.depthTexture not supported in Cube render targets');
        GLint glFramebuffer = renderTargetProperties.framebuffer;
        setupDepthTexture(glFramebuffer, renderTarget);

    }
    else {

        if (isCube) {
            std::vector<GLint> glFramebuffer = renderTargetProperties.framebuffers;
            std::vector<GLint> glDepthbuffer;

            if (renderTargetProperties.depthbuffers.size()>0) {
                glDepthbuffer = renderTargetProperties.depthbuffers;//>get<std::shared_ptr<std::vector<GLuint>>>("__glDepthbuffer");
                glDepthbuffer.clear();
            }
            else {

                renderTargetProperties.depthbuffers =std::vector<GLint>();
                glDepthbuffer = renderTargetProperties.depthbuffers;

            }
            //renderTargetProperties.__webglDepthbuffer = [];

            for (int i = 0; i < 6; i++) {

                glBindFramebuffer(GL_FRAMEBUFFER, glFramebuffer[i]);
                GLuint buffer;
                glGenRenderbuffers(1, &buffer);
                glDepthbuffer.push_back(buffer);
                //renderTargetProperties.__webglDepthbuffer[i] = _gl.createRenderbuffer();
                setupRenderBufferStorage(glDepthbuffer[i], renderTarget);

            }
        }
        else {
            GLuint glFramebuffer = renderTargetProperties.framebuffer;

            glBindFramebuffer(GL_FRAMEBUFFER, glFramebuffer);
            GLuint buffer;
            glGenRenderbuffers(1, &buffer);
            renderTargetProperties.depthbuffer =buffer;

            setupRenderBufferStorage(buffer, renderTarget);

        }

    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLTextures::setupDepthTexture(GLuint framebuffer, GLRenderTarget& renderTarget){
    bool isCube = renderTarget.isGLCubeRenderTarget;

    glEnable(GL_DEPTH_TEST);
    assert(("Depth Texture with cube render targets is not supported", !isCube));

    //if (isCube) throw new Error('Depth Texture with cube render targets is not supported');

    //todo:fix here
    //assert("renderTarget.depthTexture must be an instance of THREE.DepthTexture", renderTarget.depthTexture!=nullptr && renderTareget.depthTexture->isDepthTexture);

    /*if (!(renderTarget.depthTexture && is<DepthTexture>(renderTarget.depthTexture))) {

        throw new Error('renderTarget.depthTexture must be an instance of THREE.DepthTexture');

    }*/
    // upload an empty depth texture with framebuffer size
    auto& depthTextureProperties = properties->getProperties(renderTarget.depthTexture->uuid);
    if(renderTarget.depthBuffer && renderTarget.depthTexture){
        GLuint did;
        glGenTextures(1, &did);

        depthTextureProperties.texture = did;
        depthTextureProperties.glInit = true;
        renderTarget.depthTexture->id = did;

        info->memory.textures++;
    }

    if (depthTextureProperties.texture ||
        renderTarget.depthTexture->imageWidth != renderTarget.width ||
        renderTarget.depthTexture->imageHeight != renderTarget.height) {
        renderTarget.depthTexture->imageWidth = renderTarget.width;
        renderTarget.depthTexture->imageHeight = renderTarget.height;
        renderTarget.depthTexture->setNeedsUpdate(true);
    }

    //setTexture2D(*renderTarget.depthTexture, 0);
    state->bindTexture(TextureTarget::Texture2D, depthTextureProperties.texture, GL_TEXTURE0 + 0);
//    glBindTexture((GLenum)TextureTarget::Texture2D, depthTextureProperties.texture);
    setTextureParameters(TextureTarget::Texture2D,*renderTarget.depthTexture,false);

    PixelFormat glFormat = renderTarget.depthTexture->format; //PixelFormat::DepthFormat;
    TextureDataType glType = renderTarget.depthTexture->type; //TextureDataType::FloatType;
    GLint glInternalFormat = getInternalFormat(glFormat, glType);//GL_DEPTH24_STENCIL8;//GL_DEPTH_COMPONENT16;//getInternalFormat(glFormat, glType);
    state->texImage2D(GL_TEXTURE_2D, 0, glInternalFormat, renderTarget.width, renderTarget.height, 0, (GLenum)glFormat, (GLenum)glType);


    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    GLuint textureId = depthTextureProperties.texture;
    if (renderTarget.depthTexture->format == PixelFormat::DepthFormat) {

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureId, 0);

    }
    else if (renderTarget.depthTexture->format == PixelFormat::DepthStencilFormat) {

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, textureId, 0);

    }else {
        assert(("Unknown depthTexture format",1));
    }

    state->unbindTexture();
//    glBindTexture((GLenum)TextureTarget::Texture2D,-1);
    // check for framebuffer complete
    int status = glCheckFramebufferStatus ( GL_FRAMEBUFFER );
    if ( status != GL_FRAMEBUFFER_COMPLETE )
        std::cerr << "framebuffer object with depth texture is not complete" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void GLTextures::deallocateRenderTarget(GLRenderTarget& renderTarget){

    auto& renderTargetProperties = properties->getProperties(renderTarget.uuid);
    auto& textureProperties = properties->getProperties(renderTarget.texture->uuid);

    if(textureProperties.texture) {
        GLuint glTexture = textureProperties.texture;
        glDeleteTextures(1,&glTexture);
    }

    if (renderTarget.depthTexture) {
        //renderTarget.depthTexture->dispose();
        onTextureDispose(*renderTarget.depthTexture);
    }

    if (renderTarget.isGLCubeRenderTarget) {
        for (int i = 0; i < 6; i++) {
            std::vector<GLint> glFramebuffer = renderTargetProperties.framebuffers;
            GLuint buffer = glFramebuffer[i];
            glDeleteFramebuffers(1, &buffer);
            if (renderTargetProperties.depthbuffers.size()>0) {
                std::vector<GLint> glDepthbuffer = renderTargetProperties.depthbuffers;
                GLuint id = glDepthbuffer[i];
                glDeleteRenderbuffers(1, &id);
            }
        }
    }else {
        GLuint glFramebuffer = textureProperties.framebuffer;
        glDeleteFramebuffers(1, &glFramebuffer);
        if (renderTargetProperties.depthbuffer) {
            GLuint glDepthbuffer = renderTargetProperties.depthbuffer;
            glDeleteRenderbuffers(1, &glDepthbuffer);
        }
    }

    properties->remove(renderTarget.texture->uuid);
    properties->remove(renderTarget.uuid);
}

GLuint GLTextures::allocateTextureUnit(){
    GLuint textureUnit = textureUnits;
    assert(textureUnits <= maxTextures);

    textureUnits += 1;

    return textureUnit;
}

void GLTextures::resetTextureUnits(){
    textureUnits = 0;
}

void GLTextures::setTexture2D(Texture& texture, unsigned slot){
    auto textureProperties = properties->getProperties(texture.uuid);

    if (texture.isVideoTexture) updateVideoTexture(texture);

    if (texture.isRenderTargetTexture == false && texture.version > 0 && textureProperties.version !=  texture.version) {

        assert(texture.image.size() != 0);
        uploadTexture(texture, slot);
        return;
    }

    //state->activeTexture(GL_TEXTURE0 + slot);
    state->bindTexture(TextureTarget::Texture2D, textureProperties.texture, GL_TEXTURE0 + slot);
    //state->bindTexture(TextureTarget::Texture2D, texture.id);

}

void GLTextures::setTexture2DArray(Texture& texture, unsigned slot){
    auto& textureProperties = properties->getProperties(texture.uuid);

    GLuint version = textureProperties.version;

    if (texture.version > 0 && version !=  texture.version) {

        uploadTexture(texture, slot);
        return;

    }

    GLuint textureId = textureProperties.texture;

    state->activeTexture(GL_TEXTURE0 + slot);
    state->bindTexture(TextureTarget::Texture2DArray, textureId);
}

void GLTextures::setTexture3D(Texture& texture, unsigned slot){
    auto& textureProperties = properties->getProperties(texture.uuid);
    unsigned version = textureProperties.version;
    if (texture.version > 0 && version !=  texture.version) {
        uploadTexture(texture, slot);
        return;
    }

    GLuint textureId = textureProperties.texture;

    state->activeTexture(GL_TEXTURE0 + slot);
    state->bindTexture(TextureTarget::Texture3D, textureId);
}

void GLTextures::setTextureCube(Texture& texture, unsigned slot){
    //std::cout << "Texture cube image size = " << texture.images.size() << std::endl;
    if (texture.images.size() != 6) return;

    auto& textureProperties = properties->getProperties(texture.uuid);
    unsigned version = textureProperties.version;
    GLuint textureId = textureProperties.texture;

    if (texture.version > 0 && texture.version != version) {

        initTexture(texture);
        //textureId is updated above
        textureId = textureProperties.texture;

        //state->activeTexture(GL_TEXTURE0 + slot);
        state->bindTexture(TextureTarget::cubeMap, textureId,GL_TEXTURE0 + slot);

        //_gl.pixelStorei(_gl.UNPACK_FLIP_Y_WEBGL, texture.flipY);

        bool isCompressed = (texture.isCompressedTexture || texture.images[0]->isCompressedTexture);
        bool isDataTexture = texture.images[0]->isDataTexture;

        std::vector<std::vector<unsigned char>> cubeImage;
//        std::vector<unsigned char> cubeImage[6];
        std::vector<Texture::sptr> textures;

        GLsizei widths[6];
        GLsizei heights[6];
        cubeImage.resize(6);
        textures.reserve(6);
        for (int i = 0; i < 6; i++) {

            if (!isCompressed && !isDataTexture) {
                //todo:fix resizeImage here
                cubeImage[i] = texture.images[i]->image;//resizeImage(texture.images[i]->image, texture.images[i]->imageWidth,texture.images[i]->imageHeight,false, maxCubemapSize,texture.images[i]->channel);
                //cubeImage.push_back(texture.images[i]->image);
                widths[i] = texture.images[i]->imageWidth;
                heights[i] = texture.images[i]->imageHeight;
            }
            else {
                if (texture.images[i]->isDataTexture) {
                    cubeImage[i] = texture.images[i]->image;
                    widths[i] = texture.images[i]->imageWidth;
                    heights[i] = texture.images[i]->imageHeight;
                }
                else
                    textures.push_back(texture.images[i]);
            }

        }

        std::vector<unsigned char> image = cubeImage[0];
        bool supportsMips = isPowerOfTwo(texture) ? isPowerOfTwo(texture) : isGLES3;
        GLenum glFormat = (GLenum)texture.format;
        GLenum glType =(GLenum)texture.type;
        GLint glInternalFormat = getInternalFormat(texture.format,texture.type);

        setTextureParameters(TextureTarget::cubeMap, texture, supportsMips);

        std::vector<threecpp::MipMap> mipmaps;

        if (isCompressed) {
            for (unsigned i = 0; i < 6; i++) {
                mipmaps = textures[i]->mipmaps;
                for (unsigned j = 0; j < mipmaps.size(); j++) {
                    threecpp::MipMap mipmap = mipmaps[j];

                    if (texture.format != PixelFormat::RGBAFormat && texture.format != PixelFormat::RGBFormat) {

                        //if (glFormat != = null) {
                        state->compressedTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, j, glInternalFormat, mipmap.width, mipmap.height, mipmap.data);
                        /*}
                        else {
                            console.warn('THREE.WebGLRenderer: Attempt to load unsupported compressed texture format in .setTextureCube()');
                        }*/
                    }
                    else {
                        state->texImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, j, glInternalFormat, mipmap.width, mipmap.height, 0, glFormat, glType, mipmap.data);
                    }

                }

            }

            float v = mipmaps.size() - 1;
            textureProperties.maxMipLevel = mipmaps.size() - 1;
        }
        else {
            mipmaps = texture.mipmaps;

            for (int i = 0; i < 6; i++) {
                if (isDataTexture) {

                    state->texImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, glInternalFormat, widths[i], heights[i], 0, glFormat, glType, cubeImage[i]);

                    for (int j = 0; j < mipmaps.size(); j++) {

                        threecpp::MipMap mipmap = mipmaps[j];
                        unsigned char* mipmapImage = &mipmap.data[0];

                        state->texImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, j + 1, glInternalFormat, mipmaps[j].width, mipmaps[j].height, 0, glFormat, glType, mipmapImage);

                    }

                }else {
                    state->texImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, glInternalFormat, widths[i],heights[i],0,glFormat, glType, cubeImage[i]);

                    for (int j = 0; j < mipmaps.size(); j++) {

                        threecpp::MipMap mipmap = mipmaps[j];

                        state->texImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, j + 1, glInternalFormat, mipmaps[j].width,mipmaps[j].height,0,glFormat, glType, mipmap);

                    }

                }

            }

            float v = mipmaps.size();
            textureProperties.maxMipLevel = mipmaps.size();
        }

        if (textureNeedsGenerateMipmaps(texture, supportsMips)) {

            // We assume images for cube map have the same size.
            generateMipmap(TextureTarget::cubeMap, texture, widths[0], heights[0]);

        }
        textureProperties.version = texture.version;

        texture.updateTexture();

    }else {

        state->activeTexture(GL_TEXTURE0 + slot);
        state->bindTexture(TextureTarget::cubeMap, textureId);

    }

}

void GLTextures::setTextureCubeDynamic(Texture& texture, unsigned slot){
    state->activeTexture(GL_TEXTURE0 + slot);

    auto& textureProperties = properties->getProperties(texture.uuid);
    GLuint textureId = textureProperties.texture;

    state->bindTexture(TextureTarget::cubeMap,textureId);

}

void GLTextures::setupRenderTarget(GLRenderTarget& renderTarget){
    assert(renderTarget.texture);

    auto& renderTargetProperties = properties->getProperties(renderTarget.uuid);
    auto& textureProperties = properties->getProperties(renderTarget.texture->uuid);

    renderTarget.onDispose.connect(*this, &GLTextures::onRenderTargetDispose);
    //renderTarget.addEventListener('dispose', onRenderTargetDispose);


    GLuint id;
    glGenTextures(1, &id);

    textureProperties.texture = id;
    textureProperties.glInit = true;
    renderTarget.texture->id = id;
    //textureProperties.__webglTexture = _gl.createTexture();
    //textureProperties.version = renderTarget.texture->version;

    info->memory.textures++;

//    GLuint framebuffer;
//    GLuint depthRenderbuffer;
////    GLuint texture;
//    GLint texWidth = 512, texHeight = 512;
//    GLint maxRenderbufferSize;
//    glGetIntegerv ( GL_MAX_RENDERBUFFER_SIZE, &maxRenderbufferSize); // check if GL_MAX_RENDERBUFFER_SIZE is >= texWidth and texHeight
//    if ( ( maxRenderbufferSize <= texWidth ) || ( maxRenderbufferSize <= texHeight ) )
//    {
//        // cannot use framebuffer objects, as we need to create
//        // a depth buffer as a renderbuffer object
//        // return with appropriate error
//        std::cout << "cannot use framebuffer objects, as we need to create a depth buffer as a renderbuffer object" << std::endl;
//    }
//// generate the framebuffer, renderbuffer, and texture object names
//    glGenFramebuffers ( 1, &framebuffer );
//    glGenRenderbuffers ( 1, &depthRenderbuffer );
////    glGenTextures ( 1, &texture );
////// bind texture and load the texture mip level 0
////// texels are RGB565
////// no texels need to be specified as we are going to draw into // the texture
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture ( GL_TEXTURE_2D, id );
//    glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL );
//    glTexParameteri ( GL_TEXTURE_2D,  GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE );
//    glTexParameteri ( GL_TEXTURE_2D,  GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE );
//    glTexParameteri ( GL_TEXTURE_2D,  GL_TEXTURE_MAG_FILTER,GL_LINEAR );
//    glTexParameteri ( GL_TEXTURE_2D,  GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//// bind renderbuffer and create a 16-bit depth buffer
//// width and height of renderbuffer = width and height of
//// the texture
//    glBindRenderbuffer ( GL_RENDERBUFFER, depthRenderbuffer );
//    glRenderbufferStorage ( GL_RENDERBUFFER, GL_DEPTH_COMPONENT16,texWidth, texHeight );
//// bind the framebuffer
//    glBindFramebuffer ( GL_FRAMEBUFFER, framebuffer );
//// specify texture as color attachment
//    glFramebufferTexture2D ( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, id, 0 );
//// specify depth_renderbuffer as depth attachment
//    glFramebufferRenderbuffer ( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
//// check for framebuffer complete
//    GLint status = glCheckFramebufferStatus ( GL_FRAMEBUFFER );
//
//    if ( status == GL_FRAMEBUFFER_COMPLETE )
//    {
//        glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f );
//        glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
//    }
//    renderTargetProperties.framebuffer = framebuffer;
//
//    // Set the list of draw buffers.
//    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
//    glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
//
//    return ;

    bool isCube = renderTarget.isGLCubeRenderTarget;

    bool isMultisample = renderTarget.isGLMultisampleRenderTarget;

    bool supportsMips = isPowerOfTwo(*renderTarget.texture) ? isPowerOfTwo(*renderTarget.texture) : isGLES3;

    // Handles WebGL2 RGBFormat fallback - #18858
    if (isGLES3&& renderTarget.texture->format == PixelFormat::RGBFormat && (renderTarget.texture->type == TextureDataType::FloatType || renderTarget.texture->type == TextureDataType::HalfFloatType)) {
        renderTarget.texture->format = PixelFormat::RGBAFormat;
        //console.warn('THREE.WebGLRenderer: Rendering to textures with RGB format is not supported. Using RGBA format instead.');
    }

    // Setup framebuffer
    if (isCube) {
        //renderTargetProperties.__webglFramebuffer = [];
        renderTargetProperties.framebuffers = std::vector<GLint>(6);

        std::vector<GLint>& glFramebuffer = renderTargetProperties.framebuffers;
        glFramebuffer.clear();
        for (int i = 0; i < 6; i++) {
            //renderTargetProperties.__webglFramebuffer[i] = _gl.createFramebuffer();
            GLuint buffer;
            glGenFramebuffers(1, &buffer);
            glFramebuffer.push_back(buffer);
        }

    }else {
        GLuint buffer;
        glGenFramebuffers(1, &buffer);
        renderTargetProperties.framebuffer = buffer;

        if (isMultisample) {
            if (isGLES3) {
                GLuint glMultisampleFramebuffer;
                glGenFramebuffers(1, &glMultisampleFramebuffer);
                renderTargetProperties.multisampledFramebuffer = glMultisampleFramebuffer;


                GLuint glColorRenderbuffer;
                glGenRenderbuffers(1, &glColorRenderbuffer);
                renderTargetProperties.colorRenderbuffer =glColorRenderbuffer;

                glBindRenderbuffer(GL_RENDERBUFFER,glColorRenderbuffer);

                PixelFormat glFormat = renderTarget.texture->format;
                TextureDataType glType = renderTarget.texture->type;
                GLint glInternalFormat = getInternalFormat(glFormat, glType);
                GLuint samples = getRenderTargetSamples(renderTarget);
                glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, glInternalFormat, renderTarget.width, renderTarget.height);

                glBindFramebuffer(GL_FRAMEBUFFER,glMultisampleFramebuffer);
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, glColorRenderbuffer);
                glBindRenderbuffer(GL_RENDERBUFFER,0);

                if (renderTarget.depthBuffer) {
                    GLuint glDepthRenderbuffer;
                    glGenRenderbuffers(1, &glDepthRenderbuffer);
                    renderTargetProperties.depthRenderbuffer = glDepthRenderbuffer;
                    setupRenderBufferStorage(glDepthRenderbuffer, renderTarget, true);

                }

                glBindFramebuffer(GL_FRAMEBUFFER,0);

            }else {
                //console.warn('THREE.WebGLRenderer: WebGLMultisampleRenderTarget can only be used with WebGL2.');
            }

        }

    }

    // Setup color buffer
    auto textureId = textureProperties.texture;
    if (isCube) {

        state->bindTexture(TextureTarget::cubeMap, textureId);
        setTextureParameters(TextureTarget::cubeMap, *renderTarget.texture, supportsMips);
        std::vector<GLint> glFramebuffer = renderTargetProperties.framebuffers;
        for (int i = 0; i < 6; i++) {

            setupFrameBufferTexture(glFramebuffer[i], renderTarget, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);

        }

        if (textureNeedsGenerateMipmaps(*renderTarget.texture, supportsMips)) {

            generateMipmap(TextureTarget::cubeMap, *renderTarget.texture, renderTarget.width, renderTarget.height);

        }

        state->bindTexture(TextureTarget::cubeMap, -1);
    }
    else {
        GLuint buffer = renderTargetProperties.framebuffer;
        state->bindTexture(TextureTarget::Texture2D,textureId);
        setTextureParameters(TextureTarget::Texture2D, *renderTarget.texture, supportsMips);
        setupFrameBufferTexture(buffer, renderTarget, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D);

        if (textureNeedsGenerateMipmaps(*renderTarget.texture, supportsMips)) {
            generateMipmap(TextureTarget::Texture2D, *renderTarget.texture, renderTarget.width, renderTarget.height);
        }

        //state->bindTexture(TextureTarget::Texture2D, -1);
        state->unbindTexture();

    }

    // Setup depth and stencil buffers
    if (renderTarget.depthBuffer) {
        setupDepthRenderbuffer(renderTarget);
    }
}

void GLTextures::updateRenderTargetMipmap(GLRenderTarget& renderTarget){
    auto texture = renderTarget.texture;
    bool supportsMips = isPowerOfTwo(*renderTarget.texture) || isGLES3;

    if (textureNeedsGenerateMipmaps(*texture, supportsMips)) {

        TextureTarget target = renderTarget.isGLCubeRenderTarget ? TextureTarget::cubeMap : TextureTarget::Texture2D;

        auto& textureProperties = properties->getProperties(texture->uuid);
        GLuint glTexture = textureProperties.texture;
        //var webglTexture = properties.get(texture).__webglTexture;

        state->bindTexture(target, glTexture);
        generateMipmap(target, *texture, renderTarget.width, renderTarget.height);
        state->bindTexture(target, 0);

    }
}

void GLTextures::updateMultisampleRenderTarget(GLRenderTarget& renderTarget){
    if (renderTarget.isGLMultisampleRenderTarget) {
        if (isGLES3) {

            auto& renderTargetProperties = properties->getProperties(renderTarget.uuid);
            GLuint glMultisampledFramebufferPtr = renderTargetProperties.multisampledFramebuffer;
            GLuint glFramebufferPtr = renderTargetProperties.framebuffer;
            glBindFramebuffer(GL_READ_FRAMEBUFFER, glMultisampledFramebufferPtr);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, glFramebufferPtr);

            auto width = renderTarget.width;
            auto height = renderTarget.height;
            auto mask = GL_COLOR_BUFFER_BIT;

            if (renderTarget.depthBuffer) mask |= GL_DEPTH_BUFFER_BIT;
            if (renderTarget.stencilBuffer) mask |= GL_STENCIL_BUFFER_BIT;

            glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, mask, GL_NEAREST);

            glBindFramebuffer(GL_FRAMEBUFFER, glMultisampledFramebufferPtr); // see #18905

        }
        else {
            assert(("THREE.WebGLRenderer: WebGLMultisampleRenderTarget can only be used with WebGL2.", 1));
            //console.warn('THREE.WebGLRenderer: WebGLMultisampleRenderTarget can only be used with WebGL2.');
        }

    }
}

void GLTextures::safeSetTexture2D(Texture& texture, unsigned slot){
    setTexture2D(texture, slot);
}

void GLTextures::safeSetTextureCube(Texture& texture, unsigned slot){
    // currently relying on the fact that WebGLCubeRenderTarget.texture is a Texture and NOT a CubeTexture
    // TODO: unify these code paths
    if (texture.isCubeTexture || texture.images.size()==6) {
        //(texture.images) && texture.image.length == = 6)) {

        // CompressedTexture can have Array in image :/

        // this function alone should take care of cube textures
        setTextureCube(texture, slot);

    }else {

        // assumed: texture property of THREE.WebGLCubeRenderTarget
        setTextureCubeDynamic(texture, slot);

    }
}

void GLTextures::onTextureDispose(Texture& texture){
    deallocateTexture(texture);

    if (texture.isVideoTexture) {
        //_videoTextures.delete(texture);
        _videoTextures.erase(texture.uuid);
    }

    info->memory.textures--;
}
void GLTextures::onRenderTargetDispose(GLRenderTarget& renderTarget){
    renderTarget.onDispose.disconnectAll();
    deallocateRenderTarget(renderTarget);
    info->memory.textures--;
}

