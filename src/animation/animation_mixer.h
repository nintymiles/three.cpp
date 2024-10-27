//
// Created by SeanR on 1/10/2024.
//

#ifndef THREE_CPP_ANIMATION_MIXER_H
#define THREE_CPP_ANIMATION_MIXER_H

#include "object_3d.h"
#include "sole.h"
#include "animation_clip.h"


class AnimationAction;
class Interpolant;
template<typename T> class PropertyMixer;
struct AnimationStatus;
struct ActionsForClip;
class AnimationMixer:public std::enable_shared_from_this<AnimationMixer> {
    Object3D::sptr _root;
    size_t _accuIndex;
    float time;
    float timeScale;

    std::vector<std::shared_ptr<AnimationAction>> _actions;
    size_t _nActiveActions;
    std::map<sole::uuid,std::shared_ptr<ActionsForClip>> _actionsByClip;
    std::vector<std::shared_ptr<PropertyMixer<float>>> _bindings;
    size_t _nActiveBindings;
    std::map< std::string,std::map<std::string,std::shared_ptr<PropertyMixer<float>>>> _bindingsByRootAndName;
    std::vector<std::shared_ptr<Interpolant>> _controlInterpolants;
    size_t _nActiveControlInterpolants;

    friend class AnimationStatus;

    std::shared_ptr<AnimationStatus> stat;

public:
    AnimationMixer() = default;

    AnimationMixer( Object3D::sptr root ):_root(root),_accuIndex(0),time(0),timeScale(0){
        _initMemoryManager();
    }

    // Memory manager
    void _initMemoryManager();

    // Memory management for AnimationAction objects
    // 检查是否有排定的action index，并且索引小于激活的action数目，用以判断是否正在播放中
    bool _isActiveAction( AnimationAction& action );

    AnimationMixer& _activateAction( std::shared_ptr<AnimationAction> action );

    AnimationMixer& _addInactiveAction( std::shared_ptr<AnimationAction> action, sole::uuid clipUuid, sole::uuid rootUuid );

    AnimationMixer& _removeInactiveAction( std::shared_ptr<AnimationAction> action );

    AnimationMixer& _lendAction( std::shared_ptr<AnimationAction> action );

    AnimationMixer& _addInactiveBinding( std::shared_ptr<PropertyMixer<float>> binding, sole::uuid rootUuid, std::string trackName );
    AnimationMixer& _removeInactiveBindingsForAction( std::shared_ptr<AnimationAction> action );
    AnimationMixer& _removeInactiveBinding( std::shared_ptr<PropertyMixer<float>> binding );

    AnimationMixer& _lendBinding( std::shared_ptr<PropertyMixer<float>> binding );

    void _bindAction( std::shared_ptr<AnimationAction> action, std::shared_ptr<AnimationAction> prototypeAction );

public:
    std::shared_ptr<AnimationAction> clipAction(
            std::shared_ptr<AnimationClip> clip,
            Object3D::sptr optionalRoot,
    AnimationBlendMode blendMode
    );
    std::shared_ptr<AnimationAction> existingAction(std::shared_ptr<AnimationClip> clip,
                                                    Object3D::sptr root);
    std::shared_ptr<AnimationMixer> stopAllAction();
    AnimationMixer& update(float deltaTime);
    std::shared_ptr<AnimationMixer> setTime(int timeInSeconds);
    Object3D::sptr getRoot();
    void uncacheClip(std::shared_ptr<AnimationClip> clip);
    void uncacheRoot(Object3D::sptr root);
    void uncacheAction(std::shared_ptr<AnimationClip> clip,
        Object3D::sptr root);
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

struct ActionsForClip{
    std::vector<std::shared_ptr<AnimationAction>> knownActions;
    std::map<sole::uuid,std::shared_ptr<AnimationAction>> actionByRoot;

    //ActionsForClip(std::vector<std::shared_ptr<AnimationAction>> knownActions,std::map<sole::uuid,std::shared_ptr<AnimationAction>> actionByRoot):knownActions(knownActions),actionByRoot(actionByRoot){}
};


#endif //THREE_CPP_ANIMATION_MIXER_H
