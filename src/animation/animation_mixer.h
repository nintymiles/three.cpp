//
// Created by SeanR on 1/10/2024.
//

#ifndef THREE_CPP_ANIMATION_MIXER_H
#define THREE_CPP_ANIMATION_MIXER_H

#include "object_3d.h"
#include "sole.h"

class AnimationAction;
class PropertyMixer;
class Interpolant;
struct AnimationStatus;
class AnimationMixer {
    Object3D::sptr _root;
    size_t _accuIndex;
    float time;
    float timeScale;

    std::vector<std::shared_ptr<AnimationAction>> _actions;
    size_t _nActiveActions;
    std::map<sole::uuid,std::shared_ptr<AnimationAction>> _actionsByClip;
    std::vector<std::shared_ptr<AnimationAction>> _bindings;
    size_t _nActiveBindings;
    std::map<std::string,std::shared_ptr<PropertyMixer>> _bindingsByRootAndName;
    std::vector<std::shared_ptr<Interpolant>> _controlInterpolants;
    size_t _nActiveControlInterpolants;

    friend class AnimationStatus;

    std::shared_ptr<AnimationStatus> stat;

public:
    AnimationMixer( Object3D::sptr root ):_root(root),_accuIndex(0),time(0),timeScale(0){
        _initMemoryManager();
    }

    // Memory manager
    void _initMemoryManager();

    // Memory management for AnimationAction objects
    // 检查是否有排定的action index，并且索引小于激活的action数目，用以判断是否正在播放中
    bool _isActiveAction( AnimationAction& action );

    AnimationMixer& _addInactiveAction( AnimationAction& action, sole::uuid clipUuid, sole::uuid rootUuid ) {

        auto& actions = this->_actions;
        auto& actionsByClip = this->_actionsByClip;

        auto& actionsForClip = actionsByClip[ clipUuid ];

        if ( actionsForClip == nullptr ) {
//            actionsForClip = {
//                knownActions: [ action ],
//                actionByRoot: {}
//            };
//
//            action._byClipCacheIndex = 0;
//
//            actionsByClip[ clipUuid ] = actionsForClip;

        } else {

//            const knownActions = actionsForClip.knownActions;
//
//            action._byClipCacheIndex = knownActions.length;
//            knownActions.push( action );

        }

//        action._cacheIndex = actions.length;
//        actions.push( action );
//
//        actionsForClip.actionByRoot[ rootUuid ] = action;

        return *this;
    }
};

struct AnimationStatus{

    AnimationStatus( AnimationMixer* mixer ):actionObj(mixer){}

    size_t getTotalActions(){
        return actionObj->_actions.size();
    }

    size_t getInUseActions(){
        return actionObj->_nActiveActions;
    }

    size_t getTotalBindings(){
        return actionObj->_bindings.size();
    }

    size_t getInUseBindings(){
        return actionObj->_nActiveBindings;
    }

    size_t getTotalControlInterpolants(){
        return actionObj->_controlInterpolants.size();
    }

    size_t getInUseControlInterpolants(){
        return actionObj->_nActiveControlInterpolants;
    }

private:
    std::shared_ptr<AnimationMixer> actionObj;

};


#endif //THREE_CPP_ANIMATION_MIXER_H
