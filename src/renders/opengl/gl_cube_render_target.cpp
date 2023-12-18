//
// Created by ultraman on 12/17/2023.
//

#include "gl_cube_render_target.h"

#include "box_geometry.h"
#include "shader_material.h"
#include "uniform_values.h"
#include "mesh.h"
#include "cube_camera.h"
#include "gl_renderer.h"

GLCubeRenderTarget::GLCubeRenderTarget( int size,GLRenderTargetParameter::sptr options ) : GLRenderTarget(size,size,options) {
    this->type = "GLCubeRenderTarget";

    isGLCubeRenderTarget = true;

//        const image = { width: size, height: size, depth: 1 };
//        const images = [ image, image, image, image, image, image ];
    auto tex = Texture::create();
    tex->imageWidth = size;
    tex->imageHeight = size;
    tex->depth = 1;
    std::vector<Texture::sptr> images = {tex,tex,tex,tex,tex,tex};

    texture = CubeTexture::create( images, options->mapping, options->wrapS, options->wrapT, options->magFilter, options->minFilter, options->format, options->type, options->anisotropy, options->encoding );

    // By convention -- likely based on the RenderMan spec from the 1990's -- cube maps are specified by WebGL (and three.js)
    // in a coordinate system in which positive-x is to the right when looking up the positive-z axis -- in other words,
    // in a left-handed coordinate system. By continuing this convention, preexisting cube maps continued to render correctly.

    // three.js uses a right-handed coordinate system. So environment maps used in three.js appear to have px and nx swapped
    // and the flag isRenderTargetTexture controls this conversion. The flip is not required when using WebGLCubeRenderTarget.texture
    // as a cube texture (this is detected when isRenderTargetTexture is set to true for cube textures).

    texture->isRenderTargetTexture = true;

    texture->generateMipmaps = options->generateMipmaps;
    texture->minFilter = options->minFilter;
}

GLCubeRenderTarget& GLCubeRenderTarget::fromEquirectangularTexture( GLRenderer* renderer, Texture::sptr texture ) {

    this->texture->type = texture->type;
    this->texture->encoding = texture->encoding;

    this->texture->generateMipmaps = texture->generateMipmaps;
    this->texture->minFilter = texture->minFilter;
    this->texture->magFilter = texture->magFilter;

    auto geometry = BoxGeometry::create( 5, 5, 5 );

    ShaderMaterial::sptr shaderMaterial = ShaderMaterial::create();

    shaderMaterial->name = "CubemapFromEquirect";

    UniformValues::sptr uniforms =std::make_shared<UniformValues>();

    uniforms->set<Texture::sptr>("tEquirect", texture);

    shaderMaterial->uniforms = uniforms;
    shaderMaterial->vertexShader = /* glsl */R""(

                varying vec3 vWorldDirection;

                vec3 transformDirection( in vec3 dir, in mat4 matrix ) {

                    return normalize( ( matrix * vec4( dir, 0.0 ) ).xyz );

                }

                void main() {

                    vWorldDirection = transformDirection( position, modelMatrix );

                    #include <begin_vertex>
                    #include <project_vertex>

                }
        )"";

    shaderMaterial->fragmentShader = /* glsl */R""(

                uniform sampler2D tEquirect;

                varying vec3 vWorldDirection;

#include <common>

                void main() {

                    vec3 direction = normalize( vWorldDirection );

                    vec2 sampleUV = equirectUv( direction );

                    gl_FragColor = texture2D( tEquirect, sampleUV );

                }
        )"";
    shaderMaterial->side = Side::BackSide;
    //shaderMaterial->blending = NoBlending;

    auto mesh = Mesh::create( geometry, shaderMaterial );

    auto currentMinFilter = texture->minFilter;

    // Avoid blurred poles
    if ( texture->minFilter == TextureFilter::LinearMipmapLinearFilter ) texture->minFilter = TextureFilter::LinearFilter;

    GLCubeRenderTarget::sptr renderTarget = GLCubeRenderTarget::create(texture->imageHeight);
    renderTarget->clone(this);
    auto camera = new CubeCamera( 1, 10, renderTarget );
    Scene::sptr scene1 = Scene::create();
    scene1->add(mesh);
    camera->update( renderer, scene1 );
    //delete camera;

    texture->minFilter = currentMinFilter;

//        mesh.geometry.dispose();
//        mesh.material.dispose();

    return *this;

}

GLCubeRenderTarget& GLCubeRenderTarget::clone(GLCubeRenderTarget* target){
    target->copy(*this);

    return *target;
}

GLCubeRenderTarget& GLCubeRenderTarget::copy(const GLCubeRenderTarget& source){
    type = "GLRenderTarget";
    width = source.width;
    height = source.height;
    scissor.copy(source.scissor);
    scissorTest = source.scissorTest;
    isGLCubeRenderTarget = source.isGLCubeRenderTarget;
    isGLMultisampleRenderTarget = source.isGLMultisampleRenderTarget;
    isGLMultiviewRenderTarget = source.isGLMultiviewRenderTarget;
    viewport.copy(source.viewport);
    if (texture != nullptr && source.texture != nullptr) {
        //texture.reset();
        texture->copy(*source.texture);
    }
    if (depthTexture != nullptr && source.depthTexture != nullptr) {
        //depthTexture.reset();
        depthTexture->copy(*source.depthTexture);
    }
    depthBuffer = source.depthBuffer;
    stencilBuffer = source.stencilBuffer;
    samples = source.samples;
    //options.merge(source.options);
    options = source.options;

    return *this;
}
