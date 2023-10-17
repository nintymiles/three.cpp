//
// Created by SeanR on 2023/10/11.
//

#ifndef THREE_CPP_GL_PROPERTIES_H
#define THREE_CPP_GL_PROPERTIES_H

#include <unordered_map>

#include "gl_headers.h"
#include "gl_programs.h"
#include "sole.h"

struct Properties{
    GLuint image_textureCube = 0;
    bool glInit = false;
    GLuint texture=0;
    float currentAnisotropy=0;
    GLint version = -1;
    int maxMipLevel = 0;
    GLuint framebuffer=0;
    GLuint depthbuffer=0;
    std::vector<GLint> framebuffers;
    std::vector<GLint> depthbuffers;
    GLint multisampledFramebuffer=0;
    GLint colorRenderbuffer=0;
    GLint depthRenderbuffer=0;
};

struct MaterialProperties {
    int version=-1;
    GLProgram::sptr program;
    int lightsStateVersion;
    Fog::ptr fog;
    std::vector<float> clippingState;
    bool needsLights = false;
    bool receiveShadow = false;
    UniformValues uniforms;
    LightsHash lightsHash;
    size_t numClippingPlanes = 0;
    size_t numIntersection = 0;
    std::string shaderID;
    GLShader shader;
    TextureEncoding outputEncoding;
    std::vector<GLUniform::sptr> uniformsList;
    Texture::ptr envMap;
    Texture::ptr environment;
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
