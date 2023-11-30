//
// Created by SeanR on 2023/10/18.
//

#include "uniform_values.h"

#include "common_types.h"

class UniformsLib {
    std::unordered_map<std::string, LibUniformValues,threecpp::StringHash> _uniforms;
    void add(const std::string& id, const LibUniformValues& value) {
        _uniforms.insert({ id,value });
    }

public:
    const LibUniformValues& uniforms(const std::string& id) {
        return _uniforms[id];
    }

    UniformsLib(){
        add("common",
            LibUniformValues({
                                     value("diffuse",Color(0xeeeeeeu)),
                                     value("opacity",1.0f),
                                     value<Texture::sptr>("map",nullptr),
                                     value("uvTransform",Matrix3()),
                                     value<Texture::sptr>("alphaMap",nullptr)
                             }));


        add("specularmap",
            LibUniformValues(
                    {
                            value<Texture::sptr>("specularMap",nullptr)
                    }));

        add("envmap", LibUniformValues(
                {
                        value("flipEnvMap",-1),
                        value("reflectivity",1.0f),
                        value("refractionRatio",0.98f),
                        value("maxMipLevel",0),
                        value<Texture::sptr>("envMap",nullptr)
                }));

        add("aomap", LibUniformValues(
                {
                        value("aoMapIntensity",1.0f),
                        value<Texture::sptr>("aoMap",nullptr),
                }));

        add("lightmap", LibUniformValues(
                {
                        value("lightMapIntensity",1.0f),
                        value<Texture::sptr>("lightMap",nullptr)
                }));

        add("emissivemap", LibUniformValues(
                {
                        value<Texture::sptr>("emissiveMap",nullptr),
                }));

        add("bumpmap", LibUniformValues(
                {
                        value("bumpScale",1.0f),
                        value<Texture::sptr>("bumpMap",nullptr),
                }));

        add("normalmap", LibUniformValues(
                {
                        value("normalScale",Vector2(1.0f)),
                        value<Texture::sptr>("normalMap",nullptr),
                }))
                ;

        add("displacementmap", LibUniformValues(
                {
                        value("displacementScale",1.0f),
                        value("displacementBias",1.0f),
                        value<Texture::sptr>("displacementMap",nullptr),
                }));

        add("roughnessmap", LibUniformValues(
                {
                        value<Texture::sptr>("roughnessMap",nullptr),
                }));


        add("metalnessmap", LibUniformValues(
                {
                        value<Texture::sptr>("metalnessMap",nullptr),
                }));


        add("gradientmap", LibUniformValues(
                {
                        value<Texture::sptr>("gradientMap",nullptr),
                }));




        add("fog", LibUniformValues(
                {
                        value("fogDensity",0.00025f),
                        value("fogNear",1.0f),
                        value("fogFar",2000.0f),
                        value("fogColor",Color(0xffffffu))
                }));


        add("lights", LibUniformValues(
                {
                        value("ambientLightColor",Color(0xffffffu)),
                        value("lightProbe",std::vector<Vector3>()),

                        value("directionalLights",CachedDirectionalLights(),{
                                value("direction",Vector3()),
                                value("color",Color()),
                        }),
                        value("directionalLightShadows",CachedDirectionalLightShadows(),{
                                value("shadowBias",0.0f),
                                value("shadowNormalBias",0.0f),
                                value("shadowRadius",0.0f),
                                value("shadowMapSize",Vector2())
                        }),
                        value("directionalShadowMap",std::vector<Texture::sptr>()),
                        value("directionalShadowMatrix",std::vector<Matrix4>()),

                        value("spotLights",CachedSpotLights(),{
                                value("position",Vector3()),
                                value("direction",Vector3()),
                                value("color",Color(0xffffffU)),
                                value("distance",0.0f),
                                value("coneCos",0.0f),
                                value("penumbraCos",0.0f),
                                value("decay",0.0f)
                        }),
                        value("spotLightShadows",CachedSpotLightShadows(),{
                                value("shadowBias",0.0f),
                                value("shadowNormalBias",0.0f),
                                value("shadowRadius",0.0f),
                                value("shadowMapSize",Vector2())
                        }),
                        value("spotLightMap",std::vector<Texture::sptr>()),
                        value("spotShadowMap",std::vector<Texture::sptr>()),
                        value("spotShadowMatrix",std::vector<Matrix4>()),

                        value("pointLights",CachedPointLights(),{
                                value("color", Color(0xffffffu)),
                                value("position", Vector3()),
                                value("decay", 0.0f),
                                value("distance", 0.0f)
                        }),
                        value("pointLightShadows",CachedPointLightShadows(),{
                                value("shadowBias", 0.0f),
                                value("shadowNormalBias", 0.0f),
                                value("shadowRadius", 0.0f),
                                value("shadowMapSize", Vector3()),
                                value("shadowCameraNear", 0.1f),
                                value("shadowCameraFar", 1000.0f)
                        }),
                        value("pointShadowMap",std::vector<Texture::sptr>()),
                        value("pointShadowMatrix",std::vector<Matrix4>()),

                        value("hemisphereLights",CachedHemisphereLights(),{
                                value("direction",Vector3()),
                                value("skyColor",Color()),
                                value("groundColor",Color())
                        }),


                        value("rectAreaLights",CachedRectAreaLights(),{
                                value("color",Color()),
                                value("position",Vector3()),
                                value("width",1.0f),
                                value("height",1.0f)
                        })
                }));

        add("points", LibUniformValues(
                {
                        value("diffuse",Color(0xeeeeeeu)),
                        value("opacity",1.0f),
                        value("size",1.0f),
                        value<Texture::sptr>("map",nullptr),
                        value<Texture::sptr>("alphaMap",nullptr),
                        value("uvTransform",Matrix3()),
                }));

        add("sprite", LibUniformValues(
                {
                        value("diffuse",Color(0xeeeeeeU)),
                        value("opacity",1.0f),
                        value("center",Vector2(0.5f,0.5f)),
                        value("rotation",0.0f),
                        value<Texture::sptr>("map",nullptr),
                        value<Texture::sptr>("alphaMap",nullptr),
                        value("uvTransform",Matrix3()),
                }));
    }
};

UniformValuesDelegate& UniformValuesDelegate::add(std::initializer_list<UniformValueDelegate> add){
    for (auto it = std::begin(add); it != std::end(add); it++) {
        values += (UniformValue::sptr) * it;
    }
    return *this;
}

const LibUniformValues& getLibUniformValues(const std::string id) {
    static UniformsLib uniformsLib;
    return uniformsLib.uniforms(id);
}

UniformValuesDelegate merged(std::initializer_list<std::string> ids){
    UniformValuesDelegate merged;

    for (auto it = std::begin(ids); it != std::end(ids); it++) {
        merged.values += getLibUniformValues(*it);
    }
    return merged;
}