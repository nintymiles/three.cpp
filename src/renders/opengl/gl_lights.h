//
// Created by SeanR on 2022/12/30.
//

#ifndef THREE_CPP_SRC_RENDERS_OPENGL_GL_LIGHTS_H
#define THREE_CPP_SRC_RENDERS_OPENGL_GL_LIGHTS_H

#include "light/light.h"
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

        switch( light.type ) {
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

class ShadowUniformsCache{
public:
    std::map<int,std::map<std::string,std::any>> lights;

    //加入缓存或者返回缓存中的值
    std::map<std::string,std::any> get(Light &light){
        if(lights.find(light.id)!=lights.end()){
            return lights[light.id];
        }
        std::map<std::string,std::any> uniforms;

        switch( light.type ) {

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

#endif //THREE_CPP_SRC_RENDERS_OPENGL_GL_LIGHTS_H
