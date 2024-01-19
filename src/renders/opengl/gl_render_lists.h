//
// Created by SeanR on 2023/10/18.
//

#ifndef THREE_CPP_GL_RENDER_LISTS_H
#define THREE_CPP_GL_RENDER_LISTS_H

#include "object_3d.h"
#include "group.h"
#include "buffer_geometry.h"
#include "material.h"
#include "gl_program.h"
#include "common_types.h"

#include <functional>

class Scene;
class Camera;

struct RenderItem {
    using sptr = std::shared_ptr<RenderItem>;

    unsigned id;

    Object3D::sptr object;

    BufferGeometry::sptr geometry;

    Material::sptr material;

    GLProgram::sptr program;

    int groupOrder;

    int renderOrder;

    float z;

    threecpp::DrawRange* group;

    RenderItem(unsigned id,const Object3D::sptr& object, const BufferGeometry::sptr& geometry, const Material::sptr& material, const GLProgram::sptr& program,
               threecpp::DrawRange* group, float z, int groupOrder = 0, int renderOrder = 0)
            : id(id),object(object), geometry(geometry), material(material), program(program), group(group), z(z), groupOrder(groupOrder), renderOrder(renderOrder) {}
};

class GLRenderList {
private:
    const RenderItem::sptr& getNextRenderItem(const Object3D::sptr& object, const BufferGeometry::sptr& geometry, const Material::sptr& material,
                                              int groupOrder, float z, threecpp::DrawRange* group);
public:
    int renderItemsIndex = 0;

    std::vector<RenderItem::sptr> opaque;

    std::vector<RenderItem::sptr> transparent;

    std::vector<RenderItem::sptr> renderItems;

    GLProgram::sptr defaultProgram = nullptr;

    GLRenderList() {}

    std::function<bool(RenderItem::sptr, RenderItem::sptr)> customOpaqueSort;

    std::function<bool(RenderItem::sptr, RenderItem::sptr) > customTransparentSort;

    using sptr = std::shared_ptr<GLRenderList>;

    virtual ~GLRenderList() = default;

    void push(const Object3D::sptr& object, const BufferGeometry::sptr& geometry, const Material::sptr& material, int groupOrder, float z, threecpp::DrawRange* group);

    void unshift(const Object3D::sptr& object, const BufferGeometry::sptr& geometry, const Material::sptr& material, int groupOrder, float z, threecpp::DrawRange* group);

    void init();

    void sort(std::function<bool(const RenderItem::sptr&, const RenderItem::sptr&)> customOpaqueSort=nullptr, std::function<bool(const RenderItem::sptr&,
            const RenderItem::sptr&)> customTransparentSort=nullptr);

    void finish();
};

inline bool painterSortStable(const RenderItem::sptr& a, const RenderItem::sptr& b) {
    if (a->groupOrder != b->groupOrder)
    {
        return b->groupOrder > a->groupOrder;
    }
    else if (a->renderOrder != b->renderOrder)
    {
        return b->renderOrder > a->renderOrder;
    }
    else if (a->program!=nullptr && b->program!=nullptr &&( a->program->id != b->program->id))
    {
        return b->program->id > a->program->id;
    }
    else if (a->material->id != b->material->id)
    {
        return b->material->id > a->material->id;
    }
    else if (a->z != b->z)
    {
        return b->z > a->z;
    }
    else
    {
        return b->id > a->id;
    }
}

inline bool reversePainterSortStable(const RenderItem::sptr& a, const RenderItem::sptr& b) {
    if (a->groupOrder != b->groupOrder){
        return b->groupOrder > a->groupOrder;
    }else if (a->renderOrder != b->renderOrder){
        return b->renderOrder > a->renderOrder;
    }else if (a->program != nullptr && b->program != nullptr && (a->program->id != b->program->id)){
        return b->program->id > a->program->id;
    }else if (a->material->id != b->material->id){
        return b->material->id > a->material->id;
    }else if (a->z != b->z){
        return b->z > a->z;
    }else{
        return b->id > a->id;
    }
}

using glRenderListMapPtr = std::shared_ptr<std::unordered_map<sole::uuid, GLRenderList::sptr>>;

class GLRenderLists : public std::unordered_map<sole::uuid, glRenderListMapPtr> {
public:
    using ScenePtr = std::shared_ptr<Scene>;
    using CameraPtr = std::shared_ptr<Camera>;

    using sptr = std::shared_ptr<GLRenderLists>;

    GLRenderLists() :std::unordered_map<sole::uuid, glRenderListMapPtr>() {}

    virtual ~GLRenderLists() = default;

    void onSceneDispose(const Scene& scene);

    const GLRenderList::sptr& get(ScenePtr scene,CameraPtr camera);
};

#endif //THREE_CPP_GL_RENDER_LISTS_H
