//
// Created by SeanR on 2022/12/30.
//

#ifndef THREE_CPP_SRC_RENDERS_OPENGL_GL_LIGHTS_H
#define THREE_CPP_SRC_RENDERS_OPENGL_GL_LIGHTS_H

#include "light/light.h"
#include "light/light_probe.h"
#include "color.h"
#include "vector2.h"

#include <any>
#include <vector>
#include <string>

class UniformsCache{
public:
    std::map<int,std::map<std::string,std::any>> lights;

    //加入缓存或者返回缓存中的值
    std::map<std::string,std::any> get(Light &light){
        if(lights.find(light.id)!=lights.end()){
            return lights[light.id];
        }
        std::map<std::string,std::any> uniforms;

        switch( light.type() ) {
            case threecpp::LightType::Light:
                uniforms = {
                    {"direction", Vector3()},
                    {"color",  Color()}
                };
                break;
            case threecpp::LightType::SpotLight:
                uniforms = {
                    {"position",  Vector3()},
                    {"direction",  Vector3()},
                    {"color",  Color()},
                    {"distance", 0},
                    {"coneCos", 0},
                    {"penumbraCos", 0},
                    {"decay", 0}
                };
                break;

            case threecpp::LightType::PointLight:
                uniforms = {
                    {"position",  Vector3()},
                        {"color",  Color()},
                            {"distance", 0},
                    {"decay", 0}
                };
                break;

            case threecpp::LightType::HemisphereLight:
                uniforms = {
                    {"direction",  Vector3()},
                        {"skyColor", Color()},
                            {"groundColor",  Color()}
                };
                break;

            case threecpp::LightType::RectAreaLight:
                uniforms = {
                    {"color", Color()},
                    {"position", Vector3()},
                            {"halfWidth",Vector3()},
                    {"halfHeight",Vector3()}
                };
                break;

        }

        lights[ light.id ] = uniforms;

        return uniforms;
    }


};
using UniformsCacheUptr = std::unique_ptr<UniformsCache>;
using UniformsCacheSptr = std::shared_ptr<UniformsCache>;

class ShadowUniformsCache{
public:
    std::map<int,std::map<std::string,std::any>> lights;

    //加入缓存或者返回缓存中的值
    std::map<std::string,std::any> get(Light &light){
        if(lights.find(light.id)!=lights.end()){
            return lights[light.id];
        }
        std::map<std::string,std::any> uniforms;

        switch( light.type() ) {

            case threecpp::LightType::DirectionalLight:
                uniforms = {
                    {"shadowBias", 0},
                     {"shadowNormalBias", 0},
                      {"shadowRadius", 1},
                    {"shadowMapSize", Vector2()}
                };
                break;

            case threecpp::LightType::SpotLight:
                uniforms = {
                    {"shadowBias",0},
                     {"shadowNormalBias", 0},
                      {"shadowRadius", 1},
                    {"shadowMapSize", Vector2()}
                };
                break;

            case threecpp::LightType::PointLight:
                uniforms = {
                    {"shadowBias", 0},
                     {"shadowNormalBias", 0},
                      {"shadowRadius", 1},
                    {"shadowMapSize", Vector2()},
                    {"shadowCameraNear", 1},
                    {"shadowCameraFar", 1000}
                };
                break;

        }

        lights[ light.id ] = uniforms;
        return uniforms;
    }


};

using ShadowUniformsCacheUptr = std::unique_ptr<ShadowUniformsCache>;
using ShadowUniformsCacheSptr = std::shared_ptr<ShadowUniformsCache>;

int nextVersion = 0;

int shadowCastingLightsFirst( Light& lightA,Light& lightB ) {
    return ( lightB.castShadow ? 1 : 0 ) - ( lightA.castShadow ? 1 : 0 );
}

class GLLights{
public:

    struct GLLightsState{
        int version = 0;
        std::map<std::string,int> hash = {
            {"directionalLength", - 1},
            {"pointLength", - 1},
            {"spotLength", - 1},
            {"rectAreaLength", - 1},
            {"hemiLength", - 1},
            {"numDirectionalShadows", - 1},
            {"numPointShadows", - 1},
            {"numSpotShadows", - 1}
        };

        Vector3 ambient{ 0, 0, 0 };
        std::vector<Vector3> probe;
        std::vector<Vector3> directional;
//            directionalShadow: [],
//            directionalShadowMap: [],
//            directionalShadowMatrix: [],
//            spot: [],
//            spotShadow: [],
//            spotShadowMap: [],
//            spotShadowMatrix: [],
//            rectArea: [],
//            rectAreaLTC1: null,
//            rectAreaLTC2: null,
//            point: [],
//            pointShadow: [],
//            pointShadowMap: [],
//            pointShadowMatrix: [],
//            hemi: []

    } state;

    GLLights& setup( std::vector<std::shared_ptr<Light>> lights, bool physicallyCorrectLights ) {
        int r = 0, g = 0, b = 0;

        for ( int i = 0; i < 9; i ++ )
            state.probe[ i ].set( 0, 0, 0 );

        int directionalLength = 0;
        int pointLength = 0;
        int spotLength = 0;
        int rectAreaLength = 0;
        int hemiLength = 0;

        int numDirectionalShadows = 0;
        int numPointShadows = 0;
        int numSpotShadows = 0;

        //lights.sort( shadowCastingLightsFirst );

        // artist-friendly light intensity scaling factor
        const double scaleFactor = ( physicallyCorrectLights != true ) ? Number::PI : 1;

        for ( int i = 0, l = lights.size(); i < l; i ++ ) {
            std::shared_ptr<Light> light = lights[ i ];

            ColorSptr color = light->color();
            double intensity = light->intensity();
            double distance = light->distance();

            //todo:
            //std::map<std::string,double> shadowMap = ( light->shadow() && light->shadow()->map.find("texture")!= light->shadow()->map.end() ) ? light->shadow()->map.texture : null;

            if ( light->isLight() && light->isAmbientLight() ) {

                r += color->r * intensity * scaleFactor;
                g += color->g * intensity * scaleFactor;
                b += color->b * intensity * scaleFactor;

            } else if ( light->isLightProbe() ) {
                //https://stackoverflow.com/questions/23795265/dynamic-cast-across-a-shared-ptr
                LightProbeSptr lightProbe = std::dynamic_pointer_cast<LightProbe>(light);
                for ( int j = 0; j < 9; j ++ ) {
                    state.probe[ j ].addScaledVector( *lightProbe->sh()->coefficients[ j ], intensity );
                }

            } else if ( light->isDirectionalLight() ) {
                std::map<std::string,std::any> uniforms = _cache->get( *light );

                if(uniforms.find("color")!=uniforms.end())
                    std::any_cast<Color>(uniforms.at("color")).copy( *light->color() ).multiplyScalar( light->intensity() * scaleFactor );

                if ( light->castShadow ) {
                    LightShadowSptr shadow = light->shadow();

                    std::map<std::string,std::any> shadowUniforms = _shadowCache->get( *light );

                    shadowUniforms["shadowBias"] = shadow->bias;
                    shadowUniforms["shadowNormalBias"] = shadow->normalBias;
                    shadowUniforms["shadowRadius"] = shadow->radius;
                    shadowUniforms["shadowMapSize"] = shadow->mapSize;

//                    state.directionalShadow[ directionalLength ] = shadowUniforms;
//                    state.directionalShadowMap[ directionalLength ] = shadowMap;
//                    state.directionalShadowMatrix[ directionalLength ] = light.shadow.matrix;

                    numDirectionalShadows ++;

                }

//                state.directional[ directionalLength ] = uniforms;

                directionalLength ++;

            }
//            else if ( light.isSpotLight ) {
//
//                const uniforms = cache.get( light );
//
//                uniforms.position.setFromMatrixPosition( light.matrixWorld );
//
//                uniforms.color.copy( color ).multiplyScalar( intensity * scaleFactor );
//                uniforms.distance = distance;
//
//                uniforms.coneCos = Math.cos( light.angle );
//                uniforms.penumbraCos = Math.cos( light.angle * ( 1 - light.penumbra ) );
//                uniforms.decay = light.decay;
//
//                if ( light.castShadow ) {
//
//                    const shadow = light.shadow;
//
//                    const shadowUniforms = shadowCache.get( light );
//
//                    shadowUniforms.shadowBias = shadow.bias;
//                    shadowUniforms.shadowNormalBias = shadow.normalBias;
//                    shadowUniforms.shadowRadius = shadow.radius;
//                    shadowUniforms.shadowMapSize = shadow.mapSize;
//
//                    state.spotShadow[ spotLength ] = shadowUniforms;
//                    state.spotShadowMap[ spotLength ] = shadowMap;
//                    state.spotShadowMatrix[ spotLength ] = light.shadow.matrix;
//
//                    numSpotShadows ++;
//
//                }
//
//                state.spot[ spotLength ] = uniforms;
//
//                spotLength ++;
//
//            } else if ( light.isRectAreaLight ) {
//
//                const uniforms = cache.get( light );
//
//                // (a) intensity is the total visible light emitted
//                //uniforms.color.copy( color ).multiplyScalar( intensity / ( light.width * light.height * Math.PI ) );
//
//                // (b) intensity is the brightness of the light
//                uniforms.color.copy( color ).multiplyScalar( intensity );
//
//                uniforms.halfWidth.set( light.width * 0.5, 0.0, 0.0 );
//                uniforms.halfHeight.set( 0.0, light.height * 0.5, 0.0 );
//
//                state.rectArea[ rectAreaLength ] = uniforms;
//
//                rectAreaLength ++;
//
//            } else if ( light.isPointLight ) {
//
//                const uniforms = cache.get( light );
//
//                uniforms.color.copy( light.color ).multiplyScalar( light.intensity * scaleFactor );
//                uniforms.distance = light.distance;
//                uniforms.decay = light.decay;
//
//                if ( light.castShadow ) {
//
//                    const shadow = light.shadow;
//
//                    const shadowUniforms = shadowCache.get( light );
//
//                    shadowUniforms.shadowBias = shadow.bias;
//                    shadowUniforms.shadowNormalBias = shadow.normalBias;
//                    shadowUniforms.shadowRadius = shadow.radius;
//                    shadowUniforms.shadowMapSize = shadow.mapSize;
//                    shadowUniforms.shadowCameraNear = shadow.camera.near;
//                    shadowUniforms.shadowCameraFar = shadow.camera.far;
//
//                    state.pointShadow[ pointLength ] = shadowUniforms;
//                    state.pointShadowMap[ pointLength ] = shadowMap;
//                    state.pointShadowMatrix[ pointLength ] = light.shadow.matrix;
//
//                    numPointShadows ++;
//
//                }
//
//                state.point[ pointLength ] = uniforms;
//
//                pointLength ++;
//
//            } else if ( light.isHemisphereLight ) {
//
//                const uniforms = cache.get( light );
//
//                uniforms.skyColor.copy( light.color ).multiplyScalar( intensity * scaleFactor );
//                uniforms.groundColor.copy( light.groundColor ).multiplyScalar( intensity * scaleFactor );
//
//                state.hemi[ hemiLength ] = uniforms;
//
//                hemiLength ++;
//
//            }

        }

//        if ( rectAreaLength > 0 ) {
//
//            if ( capabilities.isWebGL2 ) {
//
//                // WebGL 2
//
//                state.rectAreaLTC1 = UniformsLib.LTC_FLOAT_1;
//                state.rectAreaLTC2 = UniformsLib.LTC_FLOAT_2;
//
//            } else {
//
//                // WebGL 1
//
//                if ( extensions.has( 'OES_texture_float_linear' ) === true ) {
//
//                    state.rectAreaLTC1 = UniformsLib.LTC_FLOAT_1;
//                    state.rectAreaLTC2 = UniformsLib.LTC_FLOAT_2;
//
//                } else if ( extensions.has( 'OES_texture_half_float_linear' ) === true ) {
//
//                    state.rectAreaLTC1 = UniformsLib.LTC_HALF_1;
//                    state.rectAreaLTC2 = UniformsLib.LTC_HALF_2;
//
//                } else {
//
//                    console.error( 'THREE.WebGLRenderer: Unable to use RectAreaLight. Missing WebGL extensions.' );
//
//                }
//
//            }
//
//        }

        state.ambient[ 0 ] = r;
        state.ambient[ 1 ] = g;
        state.ambient[ 2 ] = b;

//        const hash = state.hash;
//
//        if ( hash.directionalLength !== directionalLength ||
//            hash.pointLength !== pointLength ||
//            hash.spotLength !== spotLength ||
//            hash.rectAreaLength !== rectAreaLength ||
//            hash.hemiLength !== hemiLength ||
//            hash.numDirectionalShadows !== numDirectionalShadows ||
//            hash.numPointShadows !== numPointShadows ||
//            hash.numSpotShadows !== numSpotShadows ) {
//
//            state.directional.length = directionalLength;
//            state.spot.length = spotLength;
//            state.rectArea.length = rectAreaLength;
//            state.point.length = pointLength;
//            state.hemi.length = hemiLength;
//
//            state.directionalShadow.length = numDirectionalShadows;
//            state.directionalShadowMap.length = numDirectionalShadows;
//            state.pointShadow.length = numPointShadows;
//            state.pointShadowMap.length = numPointShadows;
//            state.spotShadow.length = numSpotShadows;
//            state.spotShadowMap.length = numSpotShadows;
//            state.directionalShadowMatrix.length = numDirectionalShadows;
//            state.pointShadowMatrix.length = numPointShadows;
//            state.spotShadowMatrix.length = numSpotShadows;

//            hash.directionalLength = directionalLength;
//            hash.pointLength = pointLength;
//            hash.spotLength = spotLength;
//            hash.rectAreaLength = rectAreaLength;
//            hash.hemiLength = hemiLength;
//
//            hash.numDirectionalShadows = numDirectionalShadows;
//            hash.numPointShadows = numPointShadows;
//            hash.numSpotShadows = numSpotShadows;

            state.version = nextVersion ++;

        //}

    }

private:
    UniformsCacheUptr _cache = std::make_unique<UniformsCache>();
    ShadowUniformsCacheUptr _shadowCache=std::make_unique<ShadowUniformsCache>();


};


//function WebGLLights( extensions, capabilities ) {

//    for ( let i = 0; i < 9; i ++ ) state.probe.push( new Vector3() );
//
//    const vector3 = new Vector3();
//    const matrix4 = new Matrix4();
//    const matrix42 = new Matrix4();
//
//    function setup( lights, physicallyCorrectLights ) {
//
//        let r = 0, g = 0, b = 0;
//
//        for ( let i = 0; i < 9; i ++ ) state.probe[ i ].set( 0, 0, 0 );
//
//        let directionalLength = 0;
//        let pointLength = 0;
//        let spotLength = 0;
//        let rectAreaLength = 0;
//        let hemiLength = 0;
//
//        let numDirectionalShadows = 0;
//        let numPointShadows = 0;
//        let numSpotShadows = 0;
//
//        lights.sort( shadowCastingLightsFirst );
//
//        // artist-friendly light intensity scaling factor
//        const scaleFactor = ( physicallyCorrectLights !== true ) ? Math.PI : 1;
//
//        for ( let i = 0, l = lights.length; i < l; i ++ ) {
//
//            const light = lights[ i ];
//
//            const color = light.color;
//            const intensity = light.intensity;
//            const distance = light.distance;
//
//            const shadowMap = ( light.shadow && light.shadow.map ) ? light.shadow.map.texture : null;
//
//            if ( light.isAmbientLight ) {
//
//                r += color.r * intensity * scaleFactor;
//                g += color.g * intensity * scaleFactor;
//                b += color.b * intensity * scaleFactor;
//
//            } else if ( light.isLightProbe ) {
//
//                for ( let j = 0; j < 9; j ++ ) {
//
//                    state.probe[ j ].addScaledVector( light.sh.coefficients[ j ], intensity );
//
//                }
//
//            } else if ( light.isDirectionalLight ) {
//
//                const uniforms = cache.get( light );
//
//                uniforms.color.copy( light.color ).multiplyScalar( light.intensity * scaleFactor );
//
//                if ( light.castShadow ) {
//
//                    const shadow = light.shadow;
//
//                    const shadowUniforms = shadowCache.get( light );
//
//                    shadowUniforms.shadowBias = shadow.bias;
//                    shadowUniforms.shadowNormalBias = shadow.normalBias;
//                    shadowUniforms.shadowRadius = shadow.radius;
//                    shadowUniforms.shadowMapSize = shadow.mapSize;
//
//                    state.directionalShadow[ directionalLength ] = shadowUniforms;
//                    state.directionalShadowMap[ directionalLength ] = shadowMap;
//                    state.directionalShadowMatrix[ directionalLength ] = light.shadow.matrix;
//
//                    numDirectionalShadows ++;
//
//                }
//
//                state.directional[ directionalLength ] = uniforms;
//
//                directionalLength ++;
//
//            } else if ( light.isSpotLight ) {
//
//                const uniforms = cache.get( light );
//
//                uniforms.position.setFromMatrixPosition( light.matrixWorld );
//
//                uniforms.color.copy( color ).multiplyScalar( intensity * scaleFactor );
//                uniforms.distance = distance;
//
//                uniforms.coneCos = Math.cos( light.angle );
//                uniforms.penumbraCos = Math.cos( light.angle * ( 1 - light.penumbra ) );
//                uniforms.decay = light.decay;
//
//                if ( light.castShadow ) {
//
//                    const shadow = light.shadow;
//
//                    const shadowUniforms = shadowCache.get( light );
//
//                    shadowUniforms.shadowBias = shadow.bias;
//                    shadowUniforms.shadowNormalBias = shadow.normalBias;
//                    shadowUniforms.shadowRadius = shadow.radius;
//                    shadowUniforms.shadowMapSize = shadow.mapSize;
//
//                    state.spotShadow[ spotLength ] = shadowUniforms;
//                    state.spotShadowMap[ spotLength ] = shadowMap;
//                    state.spotShadowMatrix[ spotLength ] = light.shadow.matrix;
//
//                    numSpotShadows ++;
//
//                }
//
//                state.spot[ spotLength ] = uniforms;
//
//                spotLength ++;
//
//            } else if ( light.isRectAreaLight ) {
//
//                const uniforms = cache.get( light );
//
//                // (a) intensity is the total visible light emitted
//                //uniforms.color.copy( color ).multiplyScalar( intensity / ( light.width * light.height * Math.PI ) );
//
//                // (b) intensity is the brightness of the light
//                uniforms.color.copy( color ).multiplyScalar( intensity );
//
//                uniforms.halfWidth.set( light.width * 0.5, 0.0, 0.0 );
//                uniforms.halfHeight.set( 0.0, light.height * 0.5, 0.0 );
//
//                state.rectArea[ rectAreaLength ] = uniforms;
//
//                rectAreaLength ++;
//
//            } else if ( light.isPointLight ) {
//
//                const uniforms = cache.get( light );
//
//                uniforms.color.copy( light.color ).multiplyScalar( light.intensity * scaleFactor );
//                uniforms.distance = light.distance;
//                uniforms.decay = light.decay;
//
//                if ( light.castShadow ) {
//
//                    const shadow = light.shadow;
//
//                    const shadowUniforms = shadowCache.get( light );
//
//                    shadowUniforms.shadowBias = shadow.bias;
//                    shadowUniforms.shadowNormalBias = shadow.normalBias;
//                    shadowUniforms.shadowRadius = shadow.radius;
//                    shadowUniforms.shadowMapSize = shadow.mapSize;
//                    shadowUniforms.shadowCameraNear = shadow.camera.near;
//                    shadowUniforms.shadowCameraFar = shadow.camera.far;
//
//                    state.pointShadow[ pointLength ] = shadowUniforms;
//                    state.pointShadowMap[ pointLength ] = shadowMap;
//                    state.pointShadowMatrix[ pointLength ] = light.shadow.matrix;
//
//                    numPointShadows ++;
//
//                }
//
//                state.point[ pointLength ] = uniforms;
//
//                pointLength ++;
//
//            } else if ( light.isHemisphereLight ) {
//
//                const uniforms = cache.get( light );
//
//                uniforms.skyColor.copy( light.color ).multiplyScalar( intensity * scaleFactor );
//                uniforms.groundColor.copy( light.groundColor ).multiplyScalar( intensity * scaleFactor );
//
//                state.hemi[ hemiLength ] = uniforms;
//
//                hemiLength ++;
//
//            }
//
//        }
//
//        if ( rectAreaLength > 0 ) {
//
//            if ( capabilities.isWebGL2 ) {
//
//                // WebGL 2
//
//                state.rectAreaLTC1 = UniformsLib.LTC_FLOAT_1;
//                state.rectAreaLTC2 = UniformsLib.LTC_FLOAT_2;
//
//            } else {
//
//                // WebGL 1
//
//                if ( extensions.has( 'OES_texture_float_linear' ) === true ) {
//
//                    state.rectAreaLTC1 = UniformsLib.LTC_FLOAT_1;
//                    state.rectAreaLTC2 = UniformsLib.LTC_FLOAT_2;
//
//                } else if ( extensions.has( 'OES_texture_half_float_linear' ) === true ) {
//
//                    state.rectAreaLTC1 = UniformsLib.LTC_HALF_1;
//                    state.rectAreaLTC2 = UniformsLib.LTC_HALF_2;
//
//                } else {
//
//                    console.error( 'THREE.WebGLRenderer: Unable to use RectAreaLight. Missing WebGL extensions.' );
//
//                }
//
//            }
//
//        }
//
//        state.ambient[ 0 ] = r;
//        state.ambient[ 1 ] = g;
//        state.ambient[ 2 ] = b;
//
//        const hash = state.hash;
//
//        if ( hash.directionalLength !== directionalLength ||
//            hash.pointLength !== pointLength ||
//            hash.spotLength !== spotLength ||
//            hash.rectAreaLength !== rectAreaLength ||
//            hash.hemiLength !== hemiLength ||
//            hash.numDirectionalShadows !== numDirectionalShadows ||
//            hash.numPointShadows !== numPointShadows ||
//            hash.numSpotShadows !== numSpotShadows ) {
//
//            state.directional.length = directionalLength;
//            state.spot.length = spotLength;
//            state.rectArea.length = rectAreaLength;
//            state.point.length = pointLength;
//            state.hemi.length = hemiLength;
//
//            state.directionalShadow.length = numDirectionalShadows;
//            state.directionalShadowMap.length = numDirectionalShadows;
//            state.pointShadow.length = numPointShadows;
//            state.pointShadowMap.length = numPointShadows;
//            state.spotShadow.length = numSpotShadows;
//            state.spotShadowMap.length = numSpotShadows;
//            state.directionalShadowMatrix.length = numDirectionalShadows;
//            state.pointShadowMatrix.length = numPointShadows;
//            state.spotShadowMatrix.length = numSpotShadows;
//
//            hash.directionalLength = directionalLength;
//            hash.pointLength = pointLength;
//            hash.spotLength = spotLength;
//            hash.rectAreaLength = rectAreaLength;
//            hash.hemiLength = hemiLength;
//
//            hash.numDirectionalShadows = numDirectionalShadows;
//            hash.numPointShadows = numPointShadows;
//            hash.numSpotShadows = numSpotShadows;
//
//            state.version = nextVersion ++;
//
//        }
//
//    }
//
//    function setupView( lights, camera ) {
//
//        let directionalLength = 0;
//        let pointLength = 0;
//        let spotLength = 0;
//        let rectAreaLength = 0;
//        let hemiLength = 0;
//
//        const viewMatrix = camera.matrixWorldInverse;
//
//        for ( let i = 0, l = lights.length; i < l; i ++ ) {
//
//            const light = lights[ i ];
//
//            if ( light.isDirectionalLight ) {
//
//                const uniforms = state.directional[ directionalLength ];
//
//                uniforms.direction.setFromMatrixPosition( light.matrixWorld );
//                vector3.setFromMatrixPosition( light.target.matrixWorld );
//                uniforms.direction.sub( vector3 );
//                uniforms.direction.transformDirection( viewMatrix );
//
//                directionalLength ++;
//
//            } else if ( light.isSpotLight ) {
//
//                const uniforms = state.spot[ spotLength ];
//
//                uniforms.position.setFromMatrixPosition( light.matrixWorld );
//                uniforms.position.applyMatrix4( viewMatrix );
//
//                uniforms.direction.setFromMatrixPosition( light.matrixWorld );
//                vector3.setFromMatrixPosition( light.target.matrixWorld );
//                uniforms.direction.sub( vector3 );
//                uniforms.direction.transformDirection( viewMatrix );
//
//                spotLength ++;
//
//            } else if ( light.isRectAreaLight ) {
//
//                const uniforms = state.rectArea[ rectAreaLength ];
//
//                uniforms.position.setFromMatrixPosition( light.matrixWorld );
//                uniforms.position.applyMatrix4( viewMatrix );
//
//                // extract local rotation of light to derive width/height half vectors
//                matrix42.identity();
//                matrix4.copy( light.matrixWorld );
//                matrix4.premultiply( viewMatrix );
//                matrix42.extractRotation( matrix4 );
//
//                uniforms.halfWidth.set( light.width * 0.5, 0.0, 0.0 );
//                uniforms.halfHeight.set( 0.0, light.height * 0.5, 0.0 );
//
//                uniforms.halfWidth.applyMatrix4( matrix42 );
//                uniforms.halfHeight.applyMatrix4( matrix42 );
//
//                rectAreaLength ++;
//
//            } else if ( light.isPointLight ) {
//
//                const uniforms = state.point[ pointLength ];
//
//                uniforms.position.setFromMatrixPosition( light.matrixWorld );
//                uniforms.position.applyMatrix4( viewMatrix );
//
//                pointLength ++;
//
//            } else if ( light.isHemisphereLight ) {
//
//                const uniforms = state.hemi[ hemiLength ];
//
//                uniforms.direction.setFromMatrixPosition( light.matrixWorld );
//                uniforms.direction.transformDirection( viewMatrix );
//                uniforms.direction.normalize();
//
//                hemiLength ++;
//
//            }
//
//        }
//
//    }
//
//    return {
//        setup: setup,
//        setupView: setupView,
//        state: state
//    };
//
//}
//
//
//export { WebGLLights };

#endif //THREE_CPP_SRC_RENDERS_OPENGL_GL_LIGHTS_H
