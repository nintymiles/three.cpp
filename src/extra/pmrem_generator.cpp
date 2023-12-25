//
// Created by SeanR on 12/14/2023.
//

#include "pmrem_generator.h"

#include "orthographic_camera.h"
#include "perspective_camera.h"
#include "uniform_values.h"

#include "box_geometry.h"
#include "gl_renderer.h"

#include <array>
#include <exception>
#include <cmath>

namespace pmrem_generator{
    auto LOD_MIN = 4;

    // The standard deviations (radians) associated with the extra mips. These are
    // chosen to approximate a Trowbridge-Reitz distribution function times the
    // geometric shadowing function. These sigma values squared must match the
    // variance #defines in cube_uv_reflection_fragment.glsl.js.
    std::array<float,6> EXTRA_LOD_SIGMA = { 0.125, 0.215, 0.35, 0.446, 0.526, 0.582 };

    // The maximum length of the blur for loop. Smaller sigmas will use fewer
    // samples and exit early, but not recompile the shader.
    auto MAX_SAMPLES = 20;

    auto _flatCamera = OrthographicCamera::create();
    auto _clearColor = Color();
    GLRenderTarget::sptr _oldTarget = nullptr;

    // Golden Ratio
    auto PHI = ( 1 + math::sqrt<float>( 5 ) ) / 2;
    auto INV_PHI = 1 / PHI;

    // Vertices of a dodecahedron (except the opposites, which represent the
    // same axis), used as axis directions evenly spread on a sphere.
    std::array<Vector3,10> _axisDirections = {
    Vector3( 1, 1, 1 ),
    Vector3( - 1, 1, 1 ),
    Vector3( 1, 1, - 1 ),
    Vector3( - 1, 1, - 1 ),
    Vector3( 0, PHI, INV_PHI ),
    Vector3( 0, PHI, - INV_PHI ),
    Vector3( INV_PHI, 0, PHI ),
    Vector3( - INV_PHI, 0, PHI ),
    Vector3( PHI, INV_PHI, 0 ),
    Vector3( - PHI, INV_PHI, 0 ) };
}

using namespace pmrem_generator;

void PMREMGenerator::_compileMaterial( Material::sptr material ) {
    auto geo = _lodPlanes.size() > 0 ? _lodPlanes[0] : BufferGeometry::create();
    auto tmpMesh = Mesh::create( geo, material );
    auto scene = Scene::create();
    scene->add(tmpMesh);
    _renderer->compile( *scene, *_flatCamera );
}

GLRenderTarget::sptr PMREMGenerator::fromScene( Scene::sptr scene, float sigma, float _near, float _far ){

        _oldTarget = _renderer->getRenderTarget();

        _setSize( 256 );

        auto cubeUVRenderTarget = _allocateTargets();
        cubeUVRenderTarget->depthBuffer = true;

        _sceneToCubeUV( scene, _near, _far, cubeUVRenderTarget );

        if ( sigma > 0 ) {
            _blur( cubeUVRenderTarget, 0, 0, sigma, nullptr );
        }

        _applyPMREM( cubeUVRenderTarget );
        _cleanup( cubeUVRenderTarget );

        return cubeUVRenderTarget;
}


GLRenderTarget::sptr PMREMGenerator::_allocateTargets() {
    auto width = 3 * std::max( _cubeSize, 16 * 7 );
    auto height = 4 * _cubeSize;

    GLRenderTargetParameter::sptr params = GLRenderTargetParameter::create();
    params->magFilter = TextureFilter::LinearFilter;
    params->minFilter = TextureFilter::LinearFilter;
    params->generateMipmaps = false;
    params->type = TextureDataType::HalfFloatType;
    params->encoding = TextureEncoding::LinearEncoding;
    params->depthBuffer = false;

    auto cubeUVRenderTarget = _createRenderTarget( width, height, params );

    if ( _pingPongRenderTarget == nullptr || _pingPongRenderTarget->width != width ) {

        if ( _pingPongRenderTarget != nullptr ) {
            //_dispose();
        }

        _pingPongRenderTarget = _createRenderTarget( width, height, params );

        //const { _lodMax } = this; //destructing assiangment in ES6
        auto _loadMax = this->_lodMax;
        //( { sizeLods: this._sizeLods, lodPlanes: this._lodPlanes, sigmas: this._sigmas } = _createPlanes( _lodMax ) );
        auto plane = _createPlanes( _lodMax );
        _sizeLods = plane.sizeLods;
        _lodPlanes = plane.lodPlanes;
        _sigmas = plane.sigmas;


        _blurMaterial = _getBlurShader( _lodMax, width, height );

    }

    return cubeUVRenderTarget;

}

GLRenderTarget::sptr PMREMGenerator::_createRenderTarget( int width, int height, GLRenderTargetParameter::sptr params ) {
    auto cubeUVRenderTarget = GLRenderTarget::create( width, height, params );
    cubeUVRenderTarget->texture->mapping = TextureMapping::CubeUVReflectionMapping;
    cubeUVRenderTarget->texture->name = "PMREM.cubeUv";
    cubeUVRenderTarget->scissorTest = true;
    return cubeUVRenderTarget;
}

PMREMGenerator::PlaneData PMREMGenerator::_createPlanes( int lodMax ) {
    std::vector<BufferGeometry::sptr> lodPlanes{};
    std::vector<int> sizeLods{};
    std::vector<float> sigmas{};

    int lod = lodMax;

    auto totalLods = lodMax - LOD_MIN + 1 + EXTRA_LOD_SIGMA.size();

    for ( int i = 0; i < totalLods; i ++ ) {
        auto sizeLod = math::pow( 2, lod );
        sizeLods.push_back( sizeLod );
        float sigma = 1.0f / sizeLod;

        if ( i > lodMax - LOD_MIN ) {
            sigma = EXTRA_LOD_SIGMA[ i - lodMax + LOD_MIN - 1 ];

        } else if ( i == 0 ) {
            sigma = 0;

        }

        sigmas.push_back( sigma );

        auto texelSize = 1.0f / ( sizeLod - 2 );
        auto min = - texelSize;
        auto max = 1 + texelSize;
        std::vector<float> uv1 = { min, min, max, min, max, max, min, min, max, max, min, max };

        auto cubeFaces = 6;
        auto vertices = 6;
        auto positionSize = 3;
        auto uvSize = 2;
        auto faceIndexSize = 1;

        std::vector<float> position = std::vector<float>{};
        std::vector<float> uv = std::vector<float>( uvSize * vertices * cubeFaces );
        std::vector<float> faceIndex = std::vector<float>( faceIndexSize * vertices * cubeFaces );

        for ( int face = 0; face < cubeFaces; face ++ ) {
            auto x = ( face % 3 ) * 2.f / 3 - 1;
            auto y = face > 2 ? 0.f : - 1;
            std::vector<float> coordinates = {
                    x, y, 0,
                    x + 2.f / 3, y, 0,
                    x + 2.f / 3, y + 1, 0,
                    x, y, 0,
                    x + 2.f / 3, y + 1, 0,
                    x, y + 1, 0
            };
            position.insert( position.end(),coordinates.begin(),coordinates.end() );
            uv.insert( uv.end(),uv1.begin(),uv1.end() );
            float facef = (float)face;
            std::vector<float> fill = { facef, facef, facef, facef, facef, facef };
            faceIndex.insert( faceIndex.end(),fill.begin(), fill.end() );

        }

        auto planes = BufferGeometry::create();
        planes->setAttribute( AttributeName::position, BufferAttribute<float>::create( position, positionSize ) );
        planes->setAttribute( AttributeName::uv, BufferAttribute<float>::create( uv, uvSize ) );
        planes->setAttribute( AttributeName::faceIndex, BufferAttribute<float>::create( faceIndex, faceIndexSize ) );

        lodPlanes.push_back( planes );

        if ( lod > LOD_MIN ) {
            lod --;
        }

    }

    return { lodPlanes, sizeLods, sigmas };
}

ShaderMaterial::sptr PMREMGenerator::_getBlurShader( int lodMax, int width, int height ){

    auto weights = std::vector<float>( MAX_SAMPLES );
    auto poleAxis = Vector3( 0, 1, 0 );
    auto shaderMaterial = ShaderMaterial::create();
    shaderMaterial->name = "SphericalGaussianBlur";
    shaderMaterial->definesFloat = {
            {"n", MAX_SAMPLES},
            {"CUBEUV_TEXEL_WIDTH", 1.0 / width},
            {"CUBEUV_TEXEL_HEIGHT", 1.0 / height},
            {"CUBEUV_MAX_MIP", lodMax},

    };

    UniformValues::sptr uniforms =std::make_shared<UniformValues>();
    uniforms->set<Texture::sptr>("envMap", nullptr);
    uniforms->set("samples", 1);
    uniforms->set("weights", weights);
    uniforms->set("latitudinal", false);
    uniforms->set("dTheta", 0);
    uniforms->set("mipInt", 0);
    uniforms->set("poleAxis", poleAxis);
    shaderMaterial->uniforms = uniforms;
    shaderMaterial->vertexShader = _getCommonVertexShader();
    shaderMaterial->fragmentShader = /* glsl */R""(

            precision mediump float;
            precision mediump int;

            varying vec3 vOutputDirection;

            uniform sampler2D envMap;
            uniform int samples;
            uniform float weights[ n ];
            uniform bool latitudinal;
            uniform float dTheta;
            uniform float mipInt;
            uniform vec3 poleAxis;

            #define ENVMAP_TYPE_CUBE_UV
            #include <cube_uv_reflection_fragment>

            vec3 getSample( float theta, vec3 axis ) {

                float cosTheta = cos( theta );
                // Rodrigues' axis-angle rotation
                vec3 sampleDirection = vOutputDirection * cosTheta
                                       + cross( axis, vOutputDirection ) * sin( theta )
                                       + axis * dot( axis, vOutputDirection ) * ( 1.0 - cosTheta );

                return bilinearCubeUV( envMap, sampleDirection, mipInt );

            }

            void main() {

                vec3 axis = latitudinal ? poleAxis : cross( poleAxis, vOutputDirection );

                if ( all( equal( axis, vec3( 0.0 ) ) ) ) {

                    axis = vec3( vOutputDirection.z, 0.0, - vOutputDirection.x );

                }

                axis = normalize( axis );

                gl_FragColor = vec4( 0.0, 0.0, 0.0, 1.0 );
                gl_FragColor.rgb += weights[ 0 ] * getSample( 0.0, axis );

                for ( int i = 1; i < n; i++ ) {

                    if ( i >= samples ) {

                        break;

                    }

                    float theta = dTheta * float( i );
                    gl_FragColor.rgb += weights[ i ] * getSample( -1.0 * theta, axis );
                    gl_FragColor.rgb += weights[ i ] * getSample( theta, axis );

                }

            }
    )"";

    shaderMaterial->blending = Blending::NoBlending;
    shaderMaterial->depthTest = false;
    shaderMaterial->depthWrite = false;

    return shaderMaterial;

}

std::string PMREMGenerator::_getCommonVertexShader() {

    return /* glsl */R""(

    precision mediump float;
    precision mediump int;

    attribute float faceIndex;

    varying vec3 vOutputDirection;

    // RH coordinate system; PMREM face-indexing convention
    vec3 getDirection( vec2 uv, float face ) {

        uv = 2.0 * uv - 1.0;

        vec3 direction = vec3( uv, 1.0 );

        if ( face == 0.0 ) {

            direction = direction.zyx; // ( 1, v, u ) pos x

        } else if ( face == 1.0 ) {

            direction = direction.xzy;
            direction.xz *= -1.0; // ( -u, 1, -v ) pos y

        } else if ( face == 2.0 ) {

            direction.x *= -1.0; // ( -u, v, 1 ) pos z

        } else if ( face == 3.0 ) {

            direction = direction.zyx;
            direction.xz *= -1.0; // ( -1, v, -u ) neg x

        } else if ( face == 4.0 ) {

            direction = direction.xzy;
            direction.xy *= -1.0; // ( -u, -1, v ) neg y

        } else if ( face == 5.0 ) {

            direction.z *= -1.0; // ( u, v, -1 ) neg z

        }

        return direction;

    }

    void main() {

        vOutputDirection = getDirection( uv, faceIndex );
        gl_Position = vec4( position, 1.0 );

    }

    )"";

}

void  PMREMGenerator::_sceneToCubeUV( Scene::sptr scene, float _near, float _far, GLRenderTarget::sptr cubeUVRenderTarget ) {

    float fov = 90;
    float aspect = 1;
    auto cubeCamera = PerspectiveCamera::create( fov, aspect, _near, _far );
    std::vector<int> upSign = { 1, - 1, 1, 1, 1, 1 };
    std::vector<int> forwardSign = { 1, 1, 1, - 1, - 1, - 1 };
    auto renderer = _renderer;

    auto originalAutoClear = renderer->autoClear;
    auto toneMapping = renderer->toneMapping;
    _clearColor = renderer->getClearColor();

    renderer->toneMapping = ToneMapping::NoToneMapping;
    renderer->autoClear = false;

    auto backgroundMaterial = MeshBasicMaterial::create();
    backgroundMaterial->name = "PMREM.Background";
    backgroundMaterial->side = Side::BackSide;
    backgroundMaterial->depthWrite = false;
    backgroundMaterial->depthTest = false;

    auto backgroundBox = Mesh::create( BoxGeometry::create(), backgroundMaterial );

    auto useSolidColor = false;
    auto background = scene->getBackgroundTexture();
    auto backgroundColor = scene->getBackgroundColor();

    if ( !backgroundColor.isNull() ) {

        backgroundMaterial->color.copy( backgroundColor );
        //scene->background = null;
        useSolidColor = true;

    } else {

        backgroundMaterial->color.copy( _clearColor );
        useSolidColor = true;

    }

    for ( int i = 0; i < 6; i ++ ) {
        auto col = i % 3;
        if ( col == 0 ) {
            cubeCamera->up.set( 0, upSign[ i ], 0 );
            cubeCamera->lookAt( forwardSign[ i ], 0, 0 );

        } else if ( col == 1 ) {
            cubeCamera->up.set( 0, 0, upSign[ i ] );
            cubeCamera->lookAt( 0, forwardSign[ i ], 0 );

        } else {
            cubeCamera->up.set( 0, upSign[ i ], 0 );
            cubeCamera->lookAt( 0, 0, forwardSign[ i ] );

        }

        auto size = _cubeSize;

        _setViewport( cubeUVRenderTarget, col * size, i > 2 ? size : 0, size, size );

        renderer->setRenderTarget( cubeUVRenderTarget );

        if ( useSolidColor ) {
            Scene::sptr scene1 = Scene::create();
            scene1->add(backgroundBox);
            renderer->render( scene1, cubeCamera );
        }

        renderer->render( scene, cubeCamera );

    }

//    backgroundBox->geometry.dispose();
//    backgroundBox->material.dispose();

    renderer->toneMapping = toneMapping;
    renderer->autoClear = originalAutoClear;
    //scene->background = background;

}

void PMREMGenerator::_setViewport(GLRenderTarget::sptr target, int x, int y, int width, int height ) {
    target->viewport.set( x, y, width, height );
    target->scissor.set( x, y, width, height );
}

/**
 * This is a two-pass Gaussian blur for a cubemap. Normally this is done
 * vertically and horizontally, but this breaks down on a cube. Here we apply
 * the blur latitudinally (around the poles), and then longitudinally (towards
 * the poles) to approximate the orthogonally-separable blur. It is least
 * accurate at the poles, but still does a decent job.
 */
void PMREMGenerator::_blur( GLRenderTarget::sptr cubeUVRenderTarget, int lodIn, int lodOut, float sigma, Vector3* poleAxis ) {
    auto pingPongRenderTarget = _pingPongRenderTarget;

    std::string dir1 = "latitudinal";
    _halfBlur(
            cubeUVRenderTarget,
            pingPongRenderTarget,
            lodIn,
            lodOut,
            sigma,
            dir1,
            poleAxis );

    std::string dir2 = "longitudinal";
    _halfBlur(
            pingPongRenderTarget,
            cubeUVRenderTarget,
            lodOut,
            lodOut,
            sigma,
            dir2,
            poleAxis );

}


void PMREMGenerator::_halfBlur( GLRenderTarget::sptr targetIn, GLRenderTarget::sptr targetOut, int lodIn, int lodOut, float sigmaRadians, std::string& direction, Vector3* poleAxis ) {

    auto renderer = _renderer;
    auto blurMaterial = _blurMaterial;

    if ( direction != "latitudinal" && direction != "longitudinal" ) {
        std::cout << (
                "blur direction must be either latitudinal or longitudinal!" ) << std::endl;

    }

    // Number of standard deviations at which to cut off the discrete approximation.
    auto STANDARD_DEVIATIONS = 3;

    auto blurMesh = Mesh::create( _lodPlanes[ lodOut ], blurMaterial );
    auto blurUniforms = blurMaterial->uniforms;

    auto pixels = _sizeLods[ lodIn ] - 1;
    auto radiansPerPixel = std::isfinite( sigmaRadians ) ? math_number::PI / ( 2 * pixels ) : 2 * math_number::PI / ( 2 * MAX_SAMPLES - 1 );
    auto sigmaPixels = sigmaRadians / radiansPerPixel;
    int samples = std::isfinite( sigmaRadians ) ? 1 + std::floor( STANDARD_DEVIATIONS * sigmaPixels ) : MAX_SAMPLES;

    if ( samples > MAX_SAMPLES ) {
        std::cout << "sigmaRadians, " << sigmaRadians << ", is too large and will clip, as it requested "
            << samples << " samples when the maximum is set to " << MAX_SAMPLES << std::endl;
    }

    std::vector<float> weights{};
    int sum = 0;

    for ( int i = 0; i < MAX_SAMPLES; ++ i ) {

        auto x = i / sigmaPixels;
        auto weight = std::exp( - x * x / 2 );
        weights.push_back( weight );

        if ( i == 0 ) {
            sum += weight;
        } else if ( i < samples ) {
            sum += 2 * weight;
        }

    }

    for ( int i = 0; i < weights.size(); i ++ ) {
        weights[ i ] = weights[ i ] / sum;
    }

    blurUniforms->set("envMap",targetIn->texture);
    blurUniforms->set("samples", samples);
    blurUniforms->set("weights", weights);
    blurUniforms->set("latitudinal" , direction == "latitudinal");

    if ( poleAxis ) {
        blurUniforms->set("poleAxis",*poleAxis);
    }

    //const { _lodMax } = this;
    auto lodMax = this->_lodMax;
    blurUniforms->set<float>("dTheta", radiansPerPixel);
    blurUniforms->set("setmipInt",lodMax - lodIn);

    auto outputSize = _sizeLods[ lodOut ];
    auto x = 3 * outputSize * ( lodOut > _lodMax - LOD_MIN ? lodOut - _lodMax + LOD_MIN : 0 );
    auto y = 4 * ( _cubeSize - outputSize );

    _setViewport( targetOut, x, y, 3 * outputSize, 2 * outputSize );
    renderer->setRenderTarget( targetOut );
    Scene::sptr scene1 = Scene::create();
    scene1->add(blurMesh);
    renderer->render( scene1, _flatCamera );

}

void PMREMGenerator::_applyPMREM( GLRenderTarget::sptr cubeUVRenderTarget ) {
    auto renderer = _renderer;
    auto autoClear = renderer->autoClear;
    renderer->autoClear = false;

    for ( int i = 1; i < _lodPlanes.size(); i ++ ) {
        auto sigma = math::sqrt( _sigmas[ i ] * _sigmas[ i ] - _sigmas[ i - 1 ] * _sigmas[ i - 1 ] );

        auto poleAxis = _axisDirections[ ( i - 1 ) % _axisDirections.size() ];

        _blur( cubeUVRenderTarget, i - 1, i, sigma, &poleAxis );
    }

    renderer->autoClear = autoClear;
}

void PMREMGenerator::_cleanup( GLRenderTarget::sptr outputTarget ) {
    _renderer->setRenderTarget( _oldTarget );
    outputTarget->scissorTest = false;
    _setViewport( outputTarget, 0, 0, outputTarget->width, outputTarget->height );

}


GLRenderTarget::sptr PMREMGenerator::_fromTexture( Texture::sptr texture, GLRenderTarget::sptr renderTarget ) {
    if ( texture->mapping == TextureMapping::CubeReflectionMapping || texture->mapping == TextureMapping::CubeRefractionMapping ) {
        _setSize( texture->image.size() == 0 ? 16 : texture->imageWidth );
    } else { // Equirectangular
        _setSize( texture->imageWidth / 4 );
    }

    _oldTarget = _renderer->getRenderTarget();

    auto cubeUVRenderTarget = renderTarget ? renderTarget : _allocateTargets();
    _textureToCubeUV( texture, cubeUVRenderTarget );
    _applyPMREM( cubeUVRenderTarget );
    _cleanup( cubeUVRenderTarget );

    return cubeUVRenderTarget;

}

void PMREMGenerator::_textureToCubeUV( Texture::sptr texture, GLRenderTarget::sptr cubeUVRenderTarget ) {
    auto renderer = _renderer;

    auto isCubeTexture = ( texture->mapping == TextureMapping::CubeReflectionMapping || texture->mapping == TextureMapping::CubeRefractionMapping );

    if ( isCubeTexture ) {

        if ( _cubemapMaterial == nullptr ) {
            _cubemapMaterial = _getCubemapMaterial();
        }

        _cubemapMaterial->uniforms->set("flipEnvMap", (texture->isRenderTargetTexture == false ) ? - 1 : 1);

    } else {

        if ( _equirectMaterial == nullptr ) {
            _equirectMaterial = _getEquirectMaterial();

        }

    }

    auto material = isCubeTexture ? _cubemapMaterial : _equirectMaterial;
    auto mesh = Mesh::create( _lodPlanes[ 0 ], material );

    auto uniforms = material->uniforms;

    uniforms->set("envMap",texture);

    auto size = _cubeSize;

    _setViewport( cubeUVRenderTarget, 0, 0, 3 * size, 2 * size );

    renderer->setRenderTarget( cubeUVRenderTarget );
    Scene::sptr scene1 = Scene::create();
    scene1->add(mesh);
    renderer->render( scene1, _flatCamera );

}

ShaderMaterial::sptr PMREMGenerator::_getCubemapMaterial() {
    ShaderMaterial::sptr shaderMaterial = ShaderMaterial::create();

    shaderMaterial->name = "CubemapToCubeUV";

    UniformValues::sptr uniforms =std::make_shared<UniformValues>();

    uniforms->set<Texture::sptr>("envMap", nullptr);
    uniforms->set("flipEnvMap", -1);
    shaderMaterial->uniforms = uniforms;

    shaderMaterial->vertexShader = _getCommonVertexShader();

    shaderMaterial->fragmentShader = /* glsl */R""(

            precision mediump float;
            precision mediump int;

            uniform float flipEnvMap;

            varying vec3 vOutputDirection;

            uniform samplerCube envMap;

            void main() {

                gl_FragColor = textureCube( envMap, vec3( flipEnvMap * vOutputDirection.x, vOutputDirection.yz ) );

            }

        )"";

    shaderMaterial->blending = Blending::NoBlending;
    shaderMaterial->depthTest = false;
    shaderMaterial->depthWrite = false;

    return shaderMaterial;

}

ShaderMaterial::sptr PMREMGenerator:: _getEquirectMaterial() {
    ShaderMaterial::sptr shaderMaterial = ShaderMaterial::create();

    shaderMaterial->name = "EquirectangularToCubeUV";

    UniformValues::sptr uniforms =std::make_shared<UniformValues>();
    uniforms->set<Texture::sptr>("envMap", nullptr);

    shaderMaterial->uniforms = uniforms;

    shaderMaterial->vertexShader = _getCommonVertexShader();

    shaderMaterial->fragmentShader = /* glsl */R""(

            precision mediump float;
            precision mediump int;

            varying vec3 vOutputDirection;

            uniform sampler2D envMap;

#include <common>

            void main() {

                vec3 outputDirection = normalize( vOutputDirection );
                vec2 uv = equirectUv( outputDirection );

                gl_FragColor = vec4( texture2D ( envMap, uv ).rgb, 1.0 );

            }

    )"";

    shaderMaterial->blending = Blending::NoBlending;
    shaderMaterial->depthTest = false;
    shaderMaterial->depthWrite = false;

    return shaderMaterial;

}











