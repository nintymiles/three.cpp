//
// Created by SeanR on 12/14/2023.
//

#ifndef THREE_CPP_PMREM_GENERATOR_H
#define THREE_CPP_PMREM_GENERATOR_H

/**
 * This class generates a Prefiltered, Mipmapped Radiance Environment Map
 * (PMREM) from a cubeMap environment texture. This allows different levels of
 * blur to be quickly accessed based on material roughness. It is packed into a
 * special CubeUV format that allows us to perform custom interpolation so that
 * we can support nonlinear formats such as RGBE. Unlike a traditional mipmap
 * chain, it only goes down to the LOD_MIN level (above), and then creates extra
 * even more filtered 'mips' at the same LOD_MIN resolution, associated with
 * higher roughness levels. In this way we maintain resolution to smoothly
 * interpolate diffuse lighting while limiting sampling computation.
 *
 * Paper: Fast, Accurate Image-Based Lighting
 * https://drive.google.com/file/d/15y8r_UpKlU9SvV4ILb0C3qCPecS8pvLz/view
*/

#include "shader_material.h"

#include "gl_render_target.h"
#include "buffer_geometry.h"
#include "scene.h"

class GLRenderer;
class PMREMGenerator {
    std::shared_ptr<GLRenderer> _renderer;
    GLRenderTarget::sptr _pingPongRenderTarget;

    int _lodMax = 0;
    int _cubeSize = 0;

    std::vector<BufferGeometry::sptr> _lodPlanes;
    std::vector<int> _sizeLods;
    std::vector<float> _sigmas;

    ShaderMaterial::sptr _blurMaterial,_cubemapMaterial,_equirectMaterial;

public:
    using sptr = std::shared_ptr<PMREMGenerator>;

    PMREMGenerator(std::shared_ptr<GLRenderer> renderer):_renderer(renderer){
        _compileMaterial(_blurMaterial);
    }

    static sptr create(std::shared_ptr<GLRenderer> renderer){
        return std::make_shared<PMREMGenerator>(renderer);
    }

    struct PlaneData{
        std::vector<BufferGeometry::sptr> lodPlanes{};
        std::vector<int> sizeLods{};
        std::vector<float> sigmas{};
    };

    /**
     * Generates a PMREM from a supplied Scene, which can be faster than using an
     * image if networking bandwidth is low. Optional sigma specifies a blur radius
     * in radians to be applied to the scene before PMREM generation. Optional near
     * and far planes ensure the scene is rendered in its entirety (the cubeCamera
     * is placed at the origin).
     */
    GLRenderTarget::sptr fromScene( Scene::sptr scene, float sigma = 0, float near = 0.1, float far = 100 );

    /**
     * Generates a PMREM from an equirectangular texture, which can be either LDR
     * or HDR. The ideal input image size is 1k (1024 x 512),
     * as this matches best with the 256 x 256 cubemap output.
     */
    GLRenderTarget::sptr fromEquirectangular( Texture::sptr equirectangular, GLRenderTarget::sptr renderTarget = nullptr ) {
        return _fromTexture( equirectangular, renderTarget );

    }

    /**
     * Generates a PMREM from an cubemap texture, which can be either LDR
     * or HDR. The ideal input cube size is 256 x 256,
     * as this matches best with the 256 x 256 cubemap output.
     */
    GLRenderTarget::sptr fromCubemap( Texture::sptr cubemap, GLRenderTarget::sptr renderTarget = nullptr ) {
        return _fromTexture( cubemap, renderTarget );

    }

    /**
     * Pre-compiles the cubemap shader. You can get faster start-up by invoking this method during
     * your texture's network fetch for increased concurrency.
     */
    void compileCubemapShader() {
        if ( _cubemapMaterial == nullptr ) {
            _cubemapMaterial = _getCubemapMaterial();
            _compileMaterial( _cubemapMaterial );
        }

    }

    /**
     * Pre-compiles the equirectangular shader. You can get faster start-up by invoking this method during
     * your texture's network fetch for increased concurrency.
     */
    void compileEquirectangularShader() {
        if ( _equirectMaterial == nullptr ) {
            _equirectMaterial = _getEquirectMaterial();
            _compileMaterial( _equirectMaterial );
        }

    }

//    /**
//     * Disposes of the PMREMGenerator's internal memory. Note that PMREMGenerator is a static class,
//     * so you should not need more than one PMREMGenerator object. If you do, calling dispose() on
//     * one of them will cause any others to also become unusable.
//     */
//    dispose() {
//
//        this._dispose();
//
//        if ( this._cubemapMaterial !== null ) this._cubemapMaterial.dispose();
//        if ( this._equirectMaterial !== null ) this._equirectMaterial.dispose();
//
//    }



private:

    void _compileMaterial( Material::sptr material );

    // private interface
    void _setSize( float cubeSize ) {
        _lodMax = math::floor( std::log2( cubeSize ) );
        _cubeSize = math::pow( 2, _lodMax );
    }

    GLRenderTarget::sptr _allocateTargets();

    GLRenderTarget::sptr _createRenderTarget( int width, int height, GLRenderTargetParameter::sptr params );

    PlaneData _createPlanes( int lodMax );

    ShaderMaterial::sptr _getBlurShader( int lodMax, int width, int height );

    std::string _getCommonVertexShader();

    void  _sceneToCubeUV( Scene::sptr scene, float _near, float _far, GLRenderTarget::sptr cubeUVRenderTarget );

    void _setViewport(GLRenderTarget::sptr target, int x, int y, int width, int height );

    /**
     * This is a two-pass Gaussian blur for a cubemap. Normally this is done
     * vertically and horizontally, but this breaks down on a cube. Here we apply
     * the blur latitudinally (around the poles), and then longitudinally (towards
     * the poles) to approximate the orthogonally-separable blur. It is least
     * accurate at the poles, but still does a decent job.
     */
    void _blur( GLRenderTarget::sptr cubeUVRenderTarget, int lodIn, int lodOut, float sigma, Vector3* poleAxis );

    void _halfBlur( GLRenderTarget::sptr targetIn, GLRenderTarget::sptr targetOut, int lodIn, int lodOut, float sigmaRadians, std::string& direction, Vector3* poleAxis );

    void _applyPMREM( GLRenderTarget::sptr cubeUVRenderTarget );

    void _cleanup( GLRenderTarget::sptr outputTarget );

    GLRenderTarget::sptr _fromTexture( Texture::sptr texture, GLRenderTarget::sptr renderTarget );

    void _textureToCubeUV( Texture::sptr texture, GLRenderTarget::sptr cubeUVRenderTarget );

    ShaderMaterial::sptr _getCubemapMaterial();

    ShaderMaterial::sptr _getEquirectMaterial();
};

#endif //THREE_CPP_PMREM_GENERATOR_H
