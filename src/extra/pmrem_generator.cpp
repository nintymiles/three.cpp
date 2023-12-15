//
// Created by SeanR on 12/14/2023.
//

#include "pmrem_generator.h"

#include "orthographic_camera.h"

#include <array>

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
    auto tmpMesh = Mesh::create( _lodPlanes[ 0 ], material );
    auto scene = Scene::create();
    scene->add(tmpMesh);
    _renderer->compile( *scene, *_flatCamera );
}

GLRenderTarget::sptr PMREMGenerator::fromScene( Scene::sptr scene, float sigma, float near, float far ){

        _oldTarget = _renderer->getRenderTarget();

        _setSize( 256 );

        //auto cubeUVRenderTarget = _allocateTargets();
        //cubeUVRenderTarget->depthBuffer = true;

        //_sceneToCubeUV( scene, near, far, cubeUVRenderTarget );

        if ( sigma > 0 ) {
           // _blur( cubeUVRenderTarget, 0, 0, sigma );
        }

        //_applyPMREM( cubeUVRenderTarget );
        //_cleanup( cubeUVRenderTarget );

        //return cubeUVRenderTarget;
}


void PMREMGenerator::_allocateTargets() {
    auto width = 3 * std::max( _cubeSize, 16 * 7 );
    auto height = 4 * _cubeSize;

//    const params = {
//            magFilter: LinearFilter,
//            minFilter: LinearFilter,
//            generateMipmaps: false,
//            type: HalfFloatType,
//            format: RGBAFormat,
//            encoding: LinearEncoding,
//            depthBuffer: false
//    };
    GLRenderTargetParameter params{};
    params.magFilter = TextureFilter::LinearFilter;
    params.minFilter = TextureFilter::LinearFilter;
    params.generateMipmaps = false;
    params.type = TextureDataType::HalfFloatType;
    params.encoding = TextureEncoding::LinearEncoding;
    params.depthBuffer = false;

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


        //_blurMaterial = _getBlurShader( _lodMax, width, height );

    }

    //return cubeUVRenderTarget;

}

GLRenderTarget::sptr PMREMGenerator::_createRenderTarget( int width, int height, GLRenderTargetParameter& params ) {
    auto cubeUVRenderTarget = GLRenderTarget::create( width, height, &params );
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



