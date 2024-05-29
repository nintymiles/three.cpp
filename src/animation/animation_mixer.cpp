//
// Created by SeanR on 1/10/2024.
//

#include "animation_mixer.h"

#include "animation_action.h"
#include "animation_clip.h"

void AnimationMixer::_initMemoryManager() {

    this->_actions = {}; // 'nActiveActions' followed by inactive ones
    this->_nActiveActions = 0;

    this->_actionsByClip = {};
    // inside:
    // {
    // 	knownActions: Array< AnimationAction > - used as prototypes
    // 	actionByRoot: AnimationAction - lookup
    // }


    this->_bindings = {}; // 'nActiveBindings' followed by inactive ones
    this->_nActiveBindings = 0;

    this->_bindingsByRootAndName = {}; // inside: Map< name, PropertyMixer >


    this->_controlInterpolants = {}; // same game as above
    this->_nActiveControlInterpolants = 0;

    this->stat = std::make_shared<AnimationStatus>(this);



}

bool AnimationMixer::_isActiveAction(AnimationAction &action) {
        const auto index = action._cacheIndex;
        return /**index >= 0  && **/index < this->_nActiveActions;
}

AnimationMixer& AnimationMixer::_addInactiveAction( std::shared_ptr<AnimationAction> action, sole::uuid clipUuid, sole::uuid rootUuid ) {

    auto& actions = this->_actions;
    auto& actionsByClip = this->_actionsByClip;

    //此处应该是一个AnimationClip对象
    auto& actionsForClip = actionsByClip[ clipUuid ];

    if ( actionsForClip == nullptr ) {
        actionsForClip = std::make_shared<ActionsForClip>();
        actionsForClip->knownActions = {action};
        actionsForClip->actionByRoot = {};

        action->_byClipCacheIndex = 0;
        actionsByClip[ clipUuid ] = actionsForClip;

    } else {

        auto knownActions = actionsForClip->knownActions;

        action->_byClipCacheIndex = knownActions.size();
        knownActions.push_back( action );
    }

    action->_cacheIndex = actions.size();
    actions.push_back( action );

    actionsForClip->actionByRoot[ rootUuid ] = action;

    return *this;
}

AnimationMixer& AnimationMixer::_removeInactiveAction( std::shared_ptr<AnimationAction> action ) {
    auto& actions = this->_actions;
    auto& lastInactiveAction = actions[ actions.size() - 1 ];
    auto cacheIndex = action->_cacheIndex;

    lastInactiveAction->_cacheIndex = cacheIndex;
    actions[ cacheIndex ] = lastInactiveAction;
    actions.pop_back();

    action->_cacheIndex = 0;


    auto& clipUuid = action->_clip->uuid;
    auto& actionsByClip = this->_actionsByClip;
    auto& actionsForClip = actionsByClip[ clipUuid ];
    auto& knownActionsForClip = actionsForClip->knownActions;

    auto& lastKnownAction =
            knownActionsForClip[ knownActionsForClip.size() - 1 ];

    auto byClipCacheIndex = action->_byClipCacheIndex;

    lastKnownAction->_byClipCacheIndex = byClipCacheIndex;
    knownActionsForClip[ byClipCacheIndex ] = lastKnownAction;
    knownActionsForClip.pop_back();

    action->_byClipCacheIndex = 0;


    auto& actionByRoot = actionsForClip->actionByRoot;
    auto rootUuid = (action->_localRoot != nullptr)? action->_localRoot->uuid : this->_root->uuid;

    //delete actionByRoot[ rootUuid ];
    actionByRoot[rootUuid] = nullptr;

    if ( knownActionsForClip.size() == 0 ) {
        //delete actionsByClip[ clipUuid ];
        actionsByClip[ clipUuid ] = nullptr;
    }

    //todo: implement _removeInactiveBindingsForAction
    //this->_removeInactiveBindingsForAction( action );

}