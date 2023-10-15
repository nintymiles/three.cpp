//
// Created by SeanR on 2023/10/15.
//

#ifndef THREE_CPP_UNIFORM_VALUES_H
#define THREE_CPP_UNIFORM_VALUES_H

#include "common_types.h"
#include "gl_uniforms.h"

#include <unordered_map>
#include <string>
#include <memory>

struct UniformValue {
    using sptr = std::shared_ptr<UniformValue>;
    bool needsUpdate = true;
    using UniformProperties = std::unordered_map<std::string, UniformValue::sptr,threecpp::StringHash>;
    UniformProperties properties;

    virtual ~UniformValue() = default;

    const std::string id;

    explicit UniformValue(std::string name) : id(name) {}
    explicit UniformValue(std::string name, UniformProperties properties) : id(name), properties(properties) {}

    template<typename T>
    UniformValue& operator = (T t);

    template<typename T>
    T& get();

    virtual sptr clone() const = 0;
    virtual void applyValue(std::shared_ptr<GLUniform> uniform,int index=0) = 0;
};

template<typename T>
struct UniformValueT;

#define UNIFORM_VALUE_T(Cls) \
template<> struct UniformValueT<Cls> : public UniformValue \
{ \
  Cls value; \
  explicit UniformValueT(std::string nm, const Cls &value) : UniformValue(nm), value(value) {} \
  explicit UniformValueT(std::string nm, const Cls &value, UniformProperties properties) : UniformValue(nm, properties), value(value) {} \
  static ptr create(std::string nm,const Cls& value, UniformProperties properties){\
    return std::make_shared<UniformValueT>(nm,value,properties);\
  }\
  static ptr create(std::string nm, const Cls &value) { \
    return std::make_shared<UniformValueT>(nm, value); \
  } \
  UniformValueT &operator = (const Cls &value) { \
    this->value = value; return *this; \
  } \
  ptr clone() const override { \
    return std::make_shared<UniformValueT>(id, value); \
  } \
  void applyValue(std::shared_ptr<GLUniform> uniform,int index=0) override { \
    uniform->setValue(value); \
  } \
};
UNIFORM_VALUE_T(float)
UNIFORM_VALUE_T(bool)
UNIFORM_VALUE_T(Color)
UNIFORM_VALUE_T(int)
UNIFORM_VALUE_T(unsigned)
UNIFORM_VALUE_T(Vector2)
UNIFORM_VALUE_T(Vector3)
UNIFORM_VALUE_T(Matrix3)
UNIFORM_VALUE_T(Matrix4)
UNIFORM_VALUE_T(Texture::ptr)
UNIFORM_VALUE_T(CubeTexture::ptr)
UNIFORM_VALUE_T(DataTexture::ptr)
UNIFORM_VALUE_T(std::vector<unsigned char>)
UNIFORM_VALUE_T(std::vector<float>)
UNIFORM_VALUE_T(std::vector<Texture::ptr>)
UNIFORM_VALUE_T(std::vector<Matrix4>)
UNIFORM_VALUE_T(std::vector<Vector3>)

#define UNIFORM_STRUCT_BODY(Cls) \
  Cls value; \
  explicit UniformValueT(std::string nm, const Cls &value) : UniformValue(nm), value(value) {} \
  explicit UniformValueT(std::string nm, const Cls &value, UniformProperties properties) : UniformValue(nm, properties), value(value) {} \
  UniformValueT &operator = (const Cls &value) { \
    this->value = value; return *this; \
  } \
  static ptr create(std::string nm, const Cls &value) { \
    return std::make_shared<UniformValueT>(nm, value); \
  } \
  ptr clone() const override { \
    return std::make_shared<UniformValueT>(id, value); \
  }

template<> struct UniformValueT<CachedSpotLights> : public UniformValue
{
    UNIFORM_STRUCT_BODY(CachedSpotLights)

    void applyValue(GLUniform::ptr uniform,int index=0) override
    {
        if (index < 0) index = 0;
        const auto& entry = value[index];
        if (uniform->id == "position") {
            uniform->setValue(entry->position);
        }
        else if (uniform->id == "direction") {
            uniform->setValue(entry->direction);
        }
        else if (uniform->id == "color") {
            uniform->setValue(entry->color);
        }
        else if (uniform->id == "distance") {
            uniform->setValue(entry->distance);
        }
        else if (uniform->id == "coneCos") {
            uniform->setValue(entry->coneCos);
        }
        else if (uniform->id == "penumbraCos") {
            uniform->setValue(entry->penumbra);
        }
        else if (uniform->id == "decay") {
            uniform->setValue(entry->decay);
        }
    }
};
template<> struct UniformValueT<CachedSpotLightShadows> : public UniformValue
{
    UNIFORM_STRUCT_BODY(CachedSpotLightShadows)

    void applyValue(GLUniform::ptr uniform,int index=0) override
    {
        if (index < 0) index = 0;
        const auto& entry = value[index];

        if (uniform->id == "shadowBias") {
            uniform->setValue(entry->shadowBias);
        }
        if (uniform->id == "shadowNormalBias") {
            uniform->setValue(entry->shadowNormalBias);
        }

        if (uniform->id == "shadowRadius") {
            uniform->setValue(entry->shadowRadius);
        }
        if (uniform->id == "shadowMapSize") {
            uniform->setValue(entry->shadowMapSize);
        }
    }
};
template<> struct UniformValueT<CachedDirectionalLights> : public UniformValue
{
    UNIFORM_STRUCT_BODY(CachedDirectionalLights)

    void applyValue(GLUniform::ptr uniform, int index = 0) override
    {

        if (index < 0) index = 0;
        const auto& entry = value[index];

        if (uniform->id == "direction") {
            uniform->setValue(entry->direction);
        }

        if (uniform->id == "color") {
            uniform->setValue(entry->color);
        }
    }
};
template<> struct UniformValueT<CachedDirectionalLightShadows> : public UniformValue
{
    UNIFORM_STRUCT_BODY(CachedDirectionalLightShadows)

    void applyValue(GLUniform::ptr uniform, int index = 0) override
    {
        if (index < 0) index = 0;
        const auto& entry = value[index];


        if (uniform->id == "shadowBias") {
            uniform->setValue(entry->shadowBias);
        }
        if (uniform->id == "shadowNormalBias") {
            uniform->setValue(entry->shadowNormalBias);
        }
        if (uniform->id == "shadowRadius") {
            uniform->setValue(entry->shadowRadius);
        }
        if (uniform->id == "shadowMapSize") {
            uniform->setValue(entry->shadowMapSize);
        }

    }
};
template<> struct UniformValueT<CachedHemisphereLights> : public UniformValue
{
    UNIFORM_STRUCT_BODY(CachedHemisphereLights)

    void applyValue(GLUniform::ptr uniform, int index = 0) override
    {
        if (index < 0) index = 0;
        const auto& entry = value[index];

        if (uniform->id == "direction") {
            uniform->setValue(entry->direction);
        }
        if (uniform->id == "skyColor") {
            uniform->setValue(entry->skyColor);
        }
        if (uniform->id == "groundColor") {
            uniform->setValue(entry->groundColor);
        }

    }
};

template<> struct UniformValueT<CachedRectAreaLights> : public UniformValue
{
    UNIFORM_STRUCT_BODY(CachedRectAreaLights)

    void applyValue(GLUniform::ptr uniform, int index = 0) override
    {
        if (index < 0) index = 0;
        const auto& entry = value[index];


        if (uniform->id == "color") {
            uniform->setValue(entry->color);
        }
        if (uniform->id == "direction") {
            uniform->setValue(entry->direction);
        }
        if (uniform->id == "width") {
            uniform->setValue(entry->width);
        }
        if (uniform->id == "height") {
            uniform->setValue(entry->height);
        }

    }
};
template<> struct UniformValueT<CachedPointLights> : public UniformValue
{
    UNIFORM_STRUCT_BODY(CachedPointLights)

    void applyValue(GLUniform::ptr uniform, int index = 0) override
    {

        if (index < 0) index = 0;
        const auto& entry = value[index];

        if (uniform->id == "position")
            uniform->setValue(entry->position);

        if (uniform->id == "color")
            uniform->setValue(entry->color);

        if (uniform->id == "distance")
            uniform->setValue(entry->distance);

        if (uniform->id == "decay")
            uniform->setValue(entry->decay);


    }
};
template<> struct UniformValueT<CachedPointLightShadows> : public UniformValue
{
    UNIFORM_STRUCT_BODY(CachedPointLightShadows)

    void applyValue(GLUniform::ptr uniform, int index = 0) override
    {
        if (index < 0) index = 0;
        const auto& entry = value[index];


        if (uniform->id == "shadowBias")
            uniform->setValue(entry->shadowBias);

        if (uniform->id == "shadowNormalBias")
            uniform->setValue(entry->shadowNormalBias);

        if (uniform->id == "shadowRadius")
            uniform->setValue(entry->shadowRadius);

        if (uniform->id == "shadowMapSize")
            uniform->setValue(entry->shadowMapSize);

        if (uniform->id == "shadowCameraNear")
            uniform->setValue(entry->shadowCameraNear);

        if (uniform->id == "shadowCameraFar")
            uniform->setValue(entry->shadowCameraFar);



    }
};

struct UniformValueDelegate {
    UniformValue::ptr value;
    template<typename T>
    UniformValueDelegate(std::string id, T t) {
        value = std::make_shared <UniformValueT<T>>(id, t);
    }
    operator UniformValue::ptr() const {
        return value;
    }
};

class UniformValues {
    std::unordered_map<std::string, std::shared_ptr<UniformValue>,threecpp::StringHash> values;
public:
    using ptr = std::shared_ptr<UniformValues>;

    UniformValues() {}

    UniformValues(const LibUniformValues& libUniforms) : values(libUniforms.cloneValues()) {}

    virtual ~UniformValues() = default;

    template<typename T>
    UniformValues& set(const std::string& name, const T& t)
    {
        if (values.count(name) > 0) {
            *values[name] = t;
        }
        else {
            values[name] = UniformValueT<T>::create(name, t);
        }
        return *this;
    }

    bool contains(const std::string& name) {
        return values.count(name) > 0;
    }
    void needsUpdate(const std::string& name, bool value) {
        if (values.count(name) > 0) values[name]->needsUpdate = value;
    }

    UniformValue& get(const std::string& name) {
        return *values.at(name);
    }

    UniformValue& operator[] (const std::string& name) {
        return *values.at(name);
    }

};

#endif //THREE_CPP_UNIFORM_VALUES_H
