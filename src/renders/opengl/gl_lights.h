//
// Created by SeanR on 2022/12/30.
//

#ifndef THREE_CPP_SRC_RENDERS_OPENGL_GL_LIGHTS_H
#define THREE_CPP_SRC_RENDERS_OPENGL_GL_LIGHTS_H

#include "lights.h"
//#include "color.h"
//#include "vector2.h"

#include <memory>
#include <vector>
#include <string>

struct LightsHash {
    unsigned directionalLength = 0;
    unsigned pointLength = 0;
    unsigned spotLength = 0;
    unsigned rectAreaLength = 0;
    unsigned hemiLength = 0;
    unsigned shadowsLength = 0;
    unsigned numDirectionalShadows = 0;
    unsigned numPointShadows = 0;
    unsigned numSpotShadows = 0;

    LightsHash() {}

    LightsHash(unsigned directionalLength, unsigned pointLength,unsigned spotLength, unsigned rectAreaLength, unsigned hemiLength, unsigned shadowsLength,
               unsigned numDirectionalShadow,unsigned numPointShadows,unsigned numSpotShadows)
            : directionalLength(directionalLength), pointLength(pointLength), spotLength(spotLength),rectAreaLength(rectAreaLength), hemiLength(hemiLength), shadowsLength(shadowsLength),
              numDirectionalShadows(numDirectionalShadows),numPointShadows(numPointShadows),numSpotShadows(numSpotShadows){}

    bool operator ==(const LightsHash& other)
    {
        return directionalLength == other.directionalLength &&
               pointLength == other.pointLength &&
               spotLength == other.spotLength &&
               rectAreaLength == other.rectAreaLength &&
               hemiLength == other.hemiLength &&
               shadowsLength == other.shadowsLength &&
               numDirectionalShadows == other.numDirectionalShadows &&
               numPointShadows == other.numPointShadows &&
               numSpotShadows == other.numSpotShadows;
    }
    bool operator !=(const LightsHash& other)
    {
        return !this->operator==(other);
    }
};

class UniformsCache  {
public:
    std::unordered_map<unsigned, Light::sptr> lights;

    UniformsCache() {}

    virtual ~UniformsCache() = default;

    Light::sptr& get(Light::sptr light) {
        DirectionalLight::sptr directionalLight;
        SpotLight::sptr spotLight;
        PointLight::sptr pointLight;
        HemisphereLight::sptr hemisphereLight;
        RectAreaLight::sptr rectAreaLight;

        if (lights.find(light->id) != lights.end()) {
            return lights[light->id];
        }
        else {
            switch (light->lightType) {
                case LightType::DirectionalLight :
                    directionalLight = std::make_shared<DirectionalLight>();
                    lights[light->id] = directionalLight;
                    break;
                case LightType::SpotLight:
                    spotLight = std::make_shared<SpotLight>();
                    spotLight->distance = 0;
                    spotLight->coneCos = 0;
                    spotLight->penumbra = 0;
                    spotLight->decay = 0;
                    lights[light->id] = spotLight;
                    break;
                case LightType::PointLight :
                    pointLight = std::make_shared<PointLight>();
                    pointLight->distance = 0;
                    pointLight->decay = 0;
                    lights[light->id] = pointLight;
                    break;
                case LightType::HemisphereLight :
                    hemisphereLight = std::make_shared<HemisphereLight>();
                    lights[light->id] = hemisphereLight;
                    break;
                case LightType::RectAreaLight :
                    rectAreaLight = std::make_shared<RectAreaLight>();
                    lights[light->id] = rectAreaLight;
                    break;
            }
            return lights[light->id];
        }

    }
};

class ShadowUniformsCache  {
public:
    using sptr = std::shared_ptr<ShadowUniformsCache>;
    std::unordered_map<unsigned, LightShadow::sptr> shadows;

    ShadowUniformsCache(){}

    virtual ~ShadowUniformsCache() = default;

    LightShadow::sptr& get(Light::sptr light) {
        DirectionalLightShadow::sptr directionalLightShadow;
        SpotLightShadow::sptr spotLightShadow;
        PointLightShadow::sptr pointLightShadow;

        if (shadows.find(light->id) != shadows.end()) {
            return shadows[light->id];
        }
        else {
            switch (light->lightType) {
                case LightType::DirectionalLight:
                    directionalLightShadow = std::make_shared<DirectionalLightShadow>();
                    directionalLightShadow->shadowBias = 0;
                    directionalLightShadow->shadowNormalBias = 0;
                    directionalLightShadow->shadowRadius = 1;
                    shadows[light->id] = directionalLightShadow;
                    break;
                case LightType::SpotLight:
                    spotLightShadow = std::make_shared<SpotLightShadow>();
                    spotLightShadow->shadowBias = 0;
                    spotLightShadow->shadowNormalBias = 0;
                    spotLightShadow->shadowRadius = 1;
                    shadows[light->id] = spotLightShadow;
                    break;
                case LightType::PointLight:
                    pointLightShadow = std::make_shared<PointLightShadow>();
                    pointLightShadow->shadowBias = 0;
                    pointLightShadow->shadowNormalBias = 0;
                    pointLightShadow->shadowCameraNear = 1;
                    pointLightShadow->shadowCameraFar = 1000;
                    shadows[light->id] = pointLightShadow;
                    break;

            }
            return shadows[light->id];
        }
    }
};

struct GLLightsState {
    int version;
    LightsHash hash;
    Color ambient;
    std::vector<Vector3> probe;

    std::vector<DirectionalLight::sptr> directional;
    std::vector<DirectionalLightShadow::sptr> directionalShadow;
    std::vector<Texture::sptr> directionalShadowMap;
    std::vector<Matrix4> directionalShadowMatrix;

    std::vector<SpotLight::sptr> spot;
    std::vector<Texture::sptr> spotLightMap;
    std::vector<SpotLightShadow::sptr> spotShadow;
    std::vector<Texture::sptr> spotShadowMap;
    std::vector<Matrix4> spotShadowMatrix;

    std::vector<RectAreaLight::sptr> rectArea;

    std::vector<PointLight::sptr> point;
    std::vector<PointLightShadow::sptr> pointShadow;
    std::vector<Texture::sptr> pointShadowMap;
    std::vector<Matrix4> pointShadowMatrix;

    std::vector<HemisphereLight::sptr> hemi;
};

class GLLights {
private:
    unsigned nextVersion = 0;
    Vector3 vector3;

public:
    UniformsCache cache;
    ShadowUniformsCache shadowCache;

    GLLightsState state;

    GLLights();

    virtual ~GLLights() = default;

    void setup(std::vector<Light::sptr>& lights, const Camera::sptr& camera);

};

inline bool shadowCastingLightsFirst(const Light::sptr& lightA, const Light::sptr& lightB) {
    int result = (lightB->castShadow ? 1 : 0) - (lightA->castShadow ? 1 : 0);
    return result > 0 ? true : false;
}

//class UniformsCache{
//public:
//    std::map<int,std::map<std::string,std::any>> lights;
//
//    //加入缓存或者返回缓存中的值
//    std::map<std::string,std::any> get(Light &light){
//        if(lights.find(light.id)!=lights.end()){
//            return lights[light.id];
//        }
//        std::map<std::string,std::any> uniforms;
//
//        switch( light.type() ) {
//            case threecpp::LightType::Light:
//                uniforms = {
//                    {"direction", Vector3()},
//                    {"color",  Color()}
//                };
//                break;
//            case threecpp::LightType::SpotLight:
//                uniforms = {
//                    {"position",  Vector3()},
//                    {"direction",  Vector3()},
//                    {"color",  Color()},
//                    {"distance", 0},
//                    {"coneCos", 0},
//                    {"penumbraCos", 0},
//                    {"decay", 0}
//                };
//                break;
//
//            case threecpp::LightType::PointLight:
//                uniforms = {
//                    {"position",  Vector3()},
//                        {"color",  Color()},
//                            {"distance", 0},
//                    {"decay", 0}
//                };
//                break;
//
//            case threecpp::LightType::HemisphereLight:
//                uniforms = {
//                    {"direction",  Vector3()},
//                        {"skyColor", Color()},
//                            {"groundColor",  Color()}
//                };
//                break;
//
//            case threecpp::LightType::RectAreaLight:
//                uniforms = {
//                    {"color", Color()},
//                    {"position", Vector3()},
//                            {"halfWidth",Vector3()},
//                    {"halfHeight",Vector3()}
//                };
//                break;
//
//        }
//
//        lights[ light.id ] = uniforms;
//
//        return uniforms;
//    }
//
//
//};
//using UniformsCacheUptr = std::unique_ptr<UniformsCache>;
//using UniformsCacheSptr = std::shared_ptr<UniformsCache>;
//
//class ShadowUniformsCache{
//public:
//    std::map<int,std::map<std::string,std::any>> lights;
//
//    //加入缓存或者返回缓存中的值
//    std::map<std::string,std::any> get(Light &light){
//        if(lights.find(light.id)!=lights.end()){
//            return lights[light.id];
//        }
//        std::map<std::string,std::any> uniforms;
//
//        switch( light.type() ) {
//
//            case threecpp::LightType::DirectionalLight:
//                uniforms = {
//                    {"shadowBias", 0},
//                     {"shadowNormalBias", 0},
//                      {"shadowRadius", 1},
//                    {"shadowMapSize", Vector2()}
//                };
//                break;
//
//            case threecpp::LightType::SpotLight:
//                uniforms = {
//                    {"shadowBias",0},
//                     {"shadowNormalBias", 0},
//                      {"shadowRadius", 1},
//                    {"shadowMapSize", Vector2()}
//                };
//                break;
//
//            case threecpp::LightType::PointLight:
//                uniforms = {
//                    {"shadowBias", 0},
//                     {"shadowNormalBias", 0},
//                      {"shadowRadius", 1},
//                    {"shadowMapSize", Vector2()},
//                    {"shadowCameraNear", 1},
//                    {"shadowCameraFar", 1000}
//                };
//                break;
//
//        }
//
//        lights[ light.id ] = uniforms;
//        return uniforms;
//    }
//
//
//};
//
//using ShadowUniformsCacheUptr = std::unique_ptr<ShadowUniformsCache>;
//using ShadowUniformsCacheSptr = std::shared_ptr<ShadowUniformsCache>;
//
//int nextVersion = 0;
//
//int shadowCastingLightsFirst( Light& lightA,Light& lightB ) {
//    return ( lightB.castShadow ? 1 : 0 ) - ( lightA.castShadow ? 1 : 0 );
//}
//
//class GLLights{
//public:
//
//    struct GLLightsState{
//        int version = 0;
//        std::map<std::string,int> hash = {
//            {"directionalLength", - 1},
//            {"pointLength", - 1},
//            {"spotLength", - 1},
//            {"rectAreaLength", - 1},
//            {"hemiLength", - 1},
//            {"numDirectionalShadows", - 1},
//            {"numPointShadows", - 1},
//            {"numSpotShadows", - 1}
//        };
//
//        Vector3 ambient{ 0, 0, 0 };
//        std::vector<Vector3> probe;
//        std::vector<Vector3> directional;
////            directionalShadow: [],
////            directionalShadowMap: [],
////            directionalShadowMatrix: [],
////            spot: [],
////            spotShadow: [],
////            spotShadowMap: [],
////            spotShadowMatrix: [],
////            rectArea: [],
////            rectAreaLTC1: null,
////            rectAreaLTC2: null,
////            point: [],
////            pointShadow: [],
////            pointShadowMap: [],
////            pointShadowMatrix: [],
////            hemi: []
//
//    } state;
//
//    GLLights& setup( std::vector<std::shared_ptr<Light>> lights, bool physicallyCorrectLights ) {
//        int r = 0, g = 0, b = 0;
//
//        for ( int i = 0; i < 9; i ++ )
//            state.probe[ i ].set( 0, 0, 0 );
//
//        int directionalLength = 0;
//        int pointLength = 0;
//        int spotLength = 0;
//        int rectAreaLength = 0;
//        int hemiLength = 0;
//
//        int numDirectionalShadows = 0;
//        int numPointShadows = 0;
//        int numSpotShadows = 0;
//
//        //lights.sort( shadowCastingLightsFirst );
//
//        // artist-friendly light intensity scaling factor
//        const double scaleFactor = ( physicallyCorrectLights != true ) ? Number::PI : 1;
//
//        for ( int i = 0, l = lights.size(); i < l; i ++ ) {
//            std::shared_ptr<Light> light = lights[ i ];
//
//            ColorSptr color = light->color();
//            double intensity = light->intensity();
//            double distance = light->distance();
//
//            //todo:
//            //std::map<std::string,double> shadowMap = ( light->shadow() && light->shadow()->map.find("texture")!= light->shadow()->map.end() ) ? light->shadow()->map.texture : null;
//
//            if ( light->isLight() && light->isAmbientLight() ) {
//
//                r += color->r * intensity * scaleFactor;
//                g += color->g * intensity * scaleFactor;
//                b += color->b * intensity * scaleFactor;
//
//            } else if ( light->isLightProbe() ) {
//                //https://stackoverflow.com/questions/23795265/dynamic-cast-across-a-shared-ptr
//                LightProbeSptr lightProbe = std::dynamic_pointer_cast<LightProbe>(light);
//                for ( int j = 0; j < 9; j ++ ) {
//                    state.probe[ j ].addScaledVector( *lightProbe->sh()->coefficients[ j ], intensity );
//                }
//
//            } else if ( light->isDirectionalLight() ) {
//                std::map<std::string,std::any> uniforms = _cache->get( *light );
//
//                if(uniforms.find("color")!=uniforms.end())
//                    std::any_cast<Color>(uniforms.at("color")).copy( *light->color() ).multiplyScalar( light->intensity() * scaleFactor );
//
//                if ( light->castShadow ) {
//                    LightShadowSptr shadow = light->shadow();
//
//                    std::map<std::string,std::any> shadowUniforms = _shadowCache->get( *light );
//
//                    shadowUniforms["shadowBias"] = shadow->bias;
//                    shadowUniforms["shadowNormalBias"] = shadow->normalBias;
//                    shadowUniforms["shadowRadius"] = shadow->radius;
//                    shadowUniforms["shadowMapSize"] = shadow->mapSize;
//
////                    state.directionalShadow[ directionalLength ] = shadowUniforms;
////                    state.directionalShadowMap[ directionalLength ] = shadowMap;
////                    state.directionalShadowMatrix[ directionalLength ] = light.shadow.matrix;
//
//                    numDirectionalShadows ++;
//
//                }
//
////                state.directional[ directionalLength ] = uniforms;
//
//                directionalLength ++;
//
//            }
////            else if ( light.isSpotLight ) {
////
////                const uniforms = cache.get( light );
////
////                uniforms.position.setFromMatrixPosition( light.matrixWorld );
////
////                uniforms.color.copy( color ).multiplyScalar( intensity * scaleFactor );
////                uniforms.distance = distance;
////
////                uniforms.coneCos = Math.cos( light.angle );
////                uniforms.penumbraCos = Math.cos( light.angle * ( 1 - light.penumbra ) );
////                uniforms.decay = light.decay;
////
////                if ( light.castShadow ) {
////
////                    const shadow = light.shadow;
////
////                    const shadowUniforms = shadowCache.get( light );
////
////                    shadowUniforms.shadowBias = shadow.bias;
////                    shadowUniforms.shadowNormalBias = shadow.normalBias;
////                    shadowUniforms.shadowRadius = shadow.radius;
////                    shadowUniforms.shadowMapSize = shadow.mapSize;
////
////                    state.spotShadow[ spotLength ] = shadowUniforms;
////                    state.spotShadowMap[ spotLength ] = shadowMap;
////                    state.spotShadowMatrix[ spotLength ] = light.shadow.matrix;
////
////                    numSpotShadows ++;
////
////                }
////
////                state.spot[ spotLength ] = uniforms;
////
////                spotLength ++;
////
////            } else if ( light.isRectAreaLight ) {
////
////                const uniforms = cache.get( light );
////
////                // (a) intensity is the total visible light emitted
////                //uniforms.color.copy( color ).multiplyScalar( intensity / ( light.width * light.height * Math.PI ) );
////
////                // (b) intensity is the brightness of the light
////                uniforms.color.copy( color ).multiplyScalar( intensity );
////
////                uniforms.halfWidth.set( light.width * 0.5, 0.0, 0.0 );
////                uniforms.halfHeight.set( 0.0, light.height * 0.5, 0.0 );
////
////                state.rectArea[ rectAreaLength ] = uniforms;
////
////                rectAreaLength ++;
////
////            } else if ( light.isPointLight ) {
////
////                const uniforms = cache.get( light );
////
////                uniforms.color.copy( light.color ).multiplyScalar( light.intensity * scaleFactor );
////                uniforms.distance = light.distance;
////                uniforms.decay = light.decay;
////
////                if ( light.castShadow ) {
////
////                    const shadow = light.shadow;
////
////                    const shadowUniforms = shadowCache.get( light );
////
////                    shadowUniforms.shadowBias = shadow.bias;
////                    shadowUniforms.shadowNormalBias = shadow.normalBias;
////                    shadowUniforms.shadowRadius = shadow.radius;
////                    shadowUniforms.shadowMapSize = shadow.mapSize;
////                    shadowUniforms.shadowCameraNear = shadow.camera.near;
////                    shadowUniforms.shadowCameraFar = shadow.camera.far;
////
////                    state.pointShadow[ pointLength ] = shadowUniforms;
////                    state.pointShadowMap[ pointLength ] = shadowMap;
////                    state.pointShadowMatrix[ pointLength ] = light.shadow.matrix;
////
////                    numPointShadows ++;
////
////                }
////
////                state.point[ pointLength ] = uniforms;
////
////                pointLength ++;
////
////            } else if ( light.isHemisphereLight ) {
////
////                const uniforms = cache.get( light );
////
////                uniforms.skyColor.copy( light.color ).multiplyScalar( intensity * scaleFactor );
////                uniforms.groundColor.copy( light.groundColor ).multiplyScalar( intensity * scaleFactor );
////
////                state.hemi[ hemiLength ] = uniforms;
////
////                hemiLength ++;
////
////            }
//
//        }
//
////        if ( rectAreaLength > 0 ) {
////
////            if ( capabilities.isWebGL2 ) {
////
////                // WebGL 2
////
////                state.rectAreaLTC1 = UniformsLib.LTC_FLOAT_1;
////                state.rectAreaLTC2 = UniformsLib.LTC_FLOAT_2;
////
////            } else {
////
////                // WebGL 1
////
////                if ( extensions.has( 'OES_texture_float_linear' ) === true ) {
////
////                    state.rectAreaLTC1 = UniformsLib.LTC_FLOAT_1;
////                    state.rectAreaLTC2 = UniformsLib.LTC_FLOAT_2;
////
////                } else if ( extensions.has( 'OES_texture_half_float_linear' ) === true ) {
////
////                    state.rectAreaLTC1 = UniformsLib.LTC_HALF_1;
////                    state.rectAreaLTC2 = UniformsLib.LTC_HALF_2;
////
////                } else {
////
////                    console.error( 'THREE.WebGLRenderer: Unable to use RectAreaLight. Missing WebGL extensions.' );
////
////                }
////
////            }
////
////        }
//
//        state.ambient[ 0 ] = r;
//        state.ambient[ 1 ] = g;
//        state.ambient[ 2 ] = b;
//
////        const hash = state.hash;
////
////        if ( hash.directionalLength !== directionalLength ||
////            hash.pointLength !== pointLength ||
////            hash.spotLength !== spotLength ||
////            hash.rectAreaLength !== rectAreaLength ||
////            hash.hemiLength !== hemiLength ||
////            hash.numDirectionalShadows !== numDirectionalShadows ||
////            hash.numPointShadows !== numPointShadows ||
////            hash.numSpotShadows !== numSpotShadows ) {
////
////            state.directional.length = directionalLength;
////            state.spot.length = spotLength;
////            state.rectArea.length = rectAreaLength;
////            state.point.length = pointLength;
////            state.hemi.length = hemiLength;
////
////            state.directionalShadow.length = numDirectionalShadows;
////            state.directionalShadowMap.length = numDirectionalShadows;
////            state.pointShadow.length = numPointShadows;
////            state.pointShadowMap.length = numPointShadows;
////            state.spotShadow.length = numSpotShadows;
////            state.spotShadowMap.length = numSpotShadows;
////            state.directionalShadowMatrix.length = numDirectionalShadows;
////            state.pointShadowMatrix.length = numPointShadows;
////            state.spotShadowMatrix.length = numSpotShadows;
//
////            hash.directionalLength = directionalLength;
////            hash.pointLength = pointLength;
////            hash.spotLength = spotLength;
////            hash.rectAreaLength = rectAreaLength;
////            hash.hemiLength = hemiLength;
////
////            hash.numDirectionalShadows = numDirectionalShadows;
////            hash.numPointShadows = numPointShadows;
////            hash.numSpotShadows = numSpotShadows;
//
//            state.version = nextVersion ++;
//
//        //}
//
//    }
//
//private:
//    UniformsCacheUptr _cache = std::make_unique<UniformsCache>();
//    ShadowUniformsCacheUptr _shadowCache=std::make_unique<ShadowUniformsCache>();
//
//
//};




#endif //THREE_CPP_SRC_RENDERS_OPENGL_GL_LIGHTS_H
