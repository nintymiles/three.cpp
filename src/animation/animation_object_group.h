//
// Created by SeanR on 2024/1/10.
//

#ifndef THREE_CPP_ANIMATION_OBJECT_GROUP_H
#define THREE_CPP_ANIMATION_OBJECT_GROUP_H

/**
 *
 * A group of objects that receives a shared animation state.
 *
 * Usage:
 *
 *  - Add objects you would otherwise pass as 'root' to the
 *    constructor or the .clipAction method of AnimationMixer.
 *
 *  - Instead pass this object as 'root'.
 *
 *  - You can also add and remove objects later when the mixer
 *    is running.
 *
 * Note:
 *
 *    Objects of this class appear as one object to the mixer,
 *    so cache control of the individual objects must be done
 *    on the group.
 *
 * Limitation:
 *
 *  - The animated properties must be compatible among the
 *    all objects in the group.
 *
 *  - A single property can either be controlled through a
 *    target group or directly, but not both.
 */

#include "sole.h"
#include "object_3d.h"

class AnimationObjectGroupStat;
class PropertyBinding;
class AnimationObjectGroup {
    friend class AnimationObjectGroupStat;

    bool isAnimationObjectGroup = true;

    sole::uuid uuid;
    size_t nCachedObjects_; // threshold
    std::map<sole::uuid,size_t> _indicesByUUID;
    std::vector<Object3D::sptr> _objects;
    std::vector<Object3D::sptr> objects;
    std::vector<std::string> _paths,_parsedPaths;
    std::vector<std::shared_ptr<PropertyBinding>> _bindings;
    std::map<std::string,sole::uuid> _bindingsIndicesByPath;

    std::shared_ptr<AnimationObjectGroupStat> stats;


public:
    AnimationObjectGroup(std::initializer_list<Object3D::sptr> objs);

    void add();

};

struct AnimationObjectGroupStat{

    AnimationObjectGroupStat(AnimationObjectGroup* animationObjectGroup):objectGroup(animationObjectGroup){}

    size_t getTotalObjects(){
        return objectGroup->_objects.size();
    }

    size_t getInUseObjects(){
        return objectGroup->nCachedObjects_;
    }

    size_t getBindingsPerObject(){
        return objectGroup->_bindings.size();
    }

private:
    AnimationObjectGroup* objectGroup;

};



#endif //THREE_CPP_ANIMATION_OBJECT_GROUP_H
