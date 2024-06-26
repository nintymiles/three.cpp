//
// Created by SeanR on 2023/10/11.
//

#ifndef THREE_CPP_GL_PROPERTIES_H
#define THREE_CPP_GL_PROPERTIES_H

#include <unordered_map>

#include "gl_headers.h"
#include "gl_programs.h"
#include "gl_lights.h"
#include "sole.h"
#include "common_types.h"
#include "fog.h"
#include "gl_shader.h"
#include "uniform_values.h"
#include "gl_shader.h"
#include "gl_uniforms.h"

struct Properties{
    GLuint image_textureCube = 0;
    bool glInit = false;
    GLuint texture = 0;
    float currentAnisotropy = 0;
    GLint version = -1;
    int maxMipLevel = 0;
    GLint framebuffer = -1;
    GLuint depthbuffer = 0;
    std::vector<GLint> framebuffers;
    std::vector<GLint> depthbuffers;
    GLint multisampledFramebuffer = 0;
    GLint colorRenderbuffer = 0;
    GLint depthRenderbuffer = 0;
};

class GLProgram;
class GLUniform;

struct MaterialProperties {
    int version=-1;
    std::shared_ptr<GLProgram> program;
    int lightsStateVersion;
    Fog::sptr fog;
    std::vector<float> clippingState;
    bool needsLights = false;
    bool receiveShadow = false;
    //UniformValues uniforms;
    std::shared_ptr<UniformValues> uniforms;
    LightsHash lightsHash;
    size_t numClippingPlanes = 0;
    size_t numIntersection = 0;
    std::string shaderID;
    //GLShader shader;
    std::shared_ptr<GLShader> shader;
    TextureEncoding outputEncoding;
    std::vector<std::shared_ptr<GLUniform>> uniformsList;
    Texture::sptr envMap;
    Texture::sptr environment;
    std::unordered_map<std::string,std::shared_ptr<GLProgram>> programs;

//    bool instancing = false;
};

class GLProperties {
    std::unordered_map<sole::uuid, Properties> properties;
    std::unordered_map<sole::uuid, MaterialProperties> materialProperties;

public :

    using sptr = std::shared_ptr<GLProperties>;

    GLProperties() {}
    virtual ~GLProperties() = default;

    Properties& getProperties(const sole::uuid& uuid) {
        return properties[uuid];
    }
    MaterialProperties& getMaterialProperties(const sole::uuid& uuid) {
        return materialProperties[uuid];
    }
    void remove(const sole::uuid& uuid) {
        properties.erase(uuid);
    }
    //template<typename T>
    //void update(const std::string& objectKey, const std::string& key, const T& val) {
    //

    //	if (contains(key)) {

    //		/*std::shared_ptr<T> temp = std::any_cast<std::shared_ptr<T>>(table->at(key));
    //
    //		delete temp.get();
    //
    //		temp.reset();*/
    //		remove(key);
    //	}

    //	//table->insert({ key,std::shared_ptr(new T(val)) });
    //	add(val);

    //}

};

#endif //THREE_CPP_GL_PROPERTIES_H
