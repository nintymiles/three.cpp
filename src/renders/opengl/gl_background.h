//
// Created by SeanR on 2023/10/18.
//

#ifndef THREE_CPP_GL_BACKGROUND_H
#define THREE_CPP_GL_BACKGROUND_H

#include "color.h"
#include "mesh.h"
#include "gl_state.h"
#include "gl_objects.h"


class Scene;
class Camera;
class GLRenderer;
class GLRenderList;
class GLRenderTarget;
class GLCubeMaps;
class GLCubeUVMaps;

class GLBackground{
private:
    void setClear(const Color& color, const float alpha);
public:
    using sptr = std::shared_ptr<GLBackground>;

    GLState::sptr state;

    GLObjects::sptr objects;

    Color clearColor{(unsigned)threecpp::Colors::black};

    float clearAlpha = 0;

    Mesh::sptr planeMesh = nullptr;

    Mesh::sptr boxMesh = nullptr;

    void* currentBackground = nullptr;

    int currentBackgroundVersion = 0;

    ToneMapping currentTonemapping = ToneMapping::NoToneMapping;

    bool premultipliedAlpha=false;

    std::shared_ptr<GLCubeMaps> cubeMaps;
    std::shared_ptr<GLCubeUVMaps> cubeUVMaps;

    GLBackground(std::shared_ptr<GLCubeMaps> cubeMaps,std::shared_ptr<GLCubeUVMaps> cubeUVMaps,GLState::sptr& state, GLObjects::sptr& objects, bool premultipliedAlpha) : cubeMaps(cubeMaps),cubeUVMaps(cubeUVMaps),state(state), objects(objects), premultipliedAlpha(premultipliedAlpha) {}

    virtual ~GLBackground() = default;

    Color& getClearColor();

    void setClearColor(const Color& color, const float alpha = 1);

    float getClearAlpha();

    void setClearAlpha(const float alpha);

    void render(GLRenderer& renderer, GLRenderList& renderList, Scene& scene, Camera& camera, bool forceClear = false);

    void beforeRender(GLRenderer& renderer, const std::shared_ptr<Scene>& scene, const std::shared_ptr<Camera>& camera, const std::shared_ptr<Object3D>& object,
                      const std::shared_ptr<BufferGeometry>& geometry, const std::shared_ptr<Material>& material, const std::shared_ptr<GLRenderTarget>& renderTarget,
                      threecpp::DrawRange* group = nullptr);
};

#endif //THREE_CPP_GL_BACKGROUND_H
