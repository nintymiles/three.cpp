//
// Created by SeanR on 2023/10/18.
//

#ifndef THREE_CPP_GL_SHADOW_MAP_H
#define THREE_CPP_GL_SHADOW_MAP_H

#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "frustum.h"

#include "material.h"
#include "shader_material.h"
#include "mesh.h"
#include "scene.h"
#include "gl_objects.h"
#include "light.h"

#include <unordered_map>

class UniformValues;

class GLShadowMap {
private:
    Frustum _frustum;
    Vector2 _shadowMapSize;
    Vector2 _viewportSize;
    Vector4f _viewport;
    std::unordered_map<int, Material::sptr> _depthMaterials;
    std::unordered_map<int, Material::sptr> _distanceMaterials;

    std::unordered_map<std::string,std::unordered_map<std::string,Material::sptr>> _materialCache;

    std::vector<Side> shadowSide = { Side::BackSide,Side::FrontSide,Side::DoubleSide };

    ShaderMaterial::sptr shadowMaterialVertical = nullptr;

    ShaderMaterial::sptr shadowMaterialHorizontal = nullptr;

    int maxTextureSize;

    GLObjects::sptr objects;
    Mesh::sptr fullScreenMesh = nullptr;

    //std::function<Material::ptr(int useMorphing, int useSkinning, int useInstancing)> getMaterialVariant;
    //Material::sptr getDepthMaterialVariant(int useMorphing, int useSkinning, int useInstancing);
    //Material::sptr getDistanceMaterialVariant(int useMorphing, int useSkinning, int useInstancing);

    Material::sptr getDepthMaterial(GLRenderer& renderer,Object3D& object3D,const Geometry::sptr& geometry, const Material::sptr& material,
                                    Light& light, float shadowCameraNear,float shadowCameraFar, ShadowMapType type);

    void renderObject(GLRenderer& renderer, const Object3D::sptr& object, const Camera::sptr& camera, const Camera::sptr& shadowCamera, Light& light,
                      ShadowMapType type);

    void VSMPass(GLRenderer& renderer,LightShadow& shadow, const Camera::sptr& camera);

    template<typename T>
    void addUniformsValue(UniformValues& uniforms, const std::string& key,const T& val){
        //GLUniform::ptr uniform = GLUniform::create("value", val);
        uniforms.set(key, val);
    }

public:
    using sptr = std::shared_ptr<GLShadowMap>;

    bool enabled = false;

    bool autoUpdate = true;

    bool needsUpdate = false;

    ShadowMapType type;

    GLShadowMap(GLObjects::sptr& objects, int maxTextureSize);

    virtual ~GLShadowMap() = default;

    void render(GLRenderer& renderer,const std::vector<Light::sptr>& lights,Scene::sptr& scene, const Camera::sptr& camera);

};

#endif //THREE_CPP_GL_SHADOW_MAP_H
