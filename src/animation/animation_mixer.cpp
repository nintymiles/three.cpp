//
// Created by SeanR on 1/10/2024.
//

#include "animation_mixer.h"

#include "animation_action.h"
#include "animation_clip.h"
#include "property_mixer.h"

#include <cmath>

std::shared_ptr<AnimationAction> AnimationMixer::clipAction(std::shared_ptr<AnimationClip> clip, Object3D::sptr optionalRoot,
                                                            AnimationBlendMode blendMode) {
    // return an action for a clip optionally using a custom root target
    // object (this method allocates a lot of dynamic memory in case a
    // previously unknown clip/root combination is specified)

    Object3D::sptr root;
    if(optionalRoot)
        root = optionalRoot;
    else
        root = this->_root;

    auto rootUuid = root->uuid;

    auto clipObject = clip;

    const auto clipUuid = (clipObject != nullptr) ? clipObject->uuid : sole::uuid0();

    const auto actionsForClip = this->_actionsByClip[ clipUuid ];
    std::shared_ptr<AnimationAction> prototypeAction;

    if ( blendMode == AnimationBlendMode::Unknown ) {

        if ( clipObject ) {

            blendMode = clipObject->blendMode;

        } else {

            blendMode = AnimationBlendMode::NormalAnimationBlendMode;

        }

    }

    if ( actionsForClip ) {

        const auto existingAction = actionsForClip->actionByRoot[ rootUuid ];

        if ( existingAction && existingAction->blendMode == blendMode ) {
            return existingAction;
        }

        // we know the clip, so we don't have to parse all
        // the bindings again but can just copy
        prototypeAction = actionsForClip->knownActions[ 0 ];

        // also, take the clip from the prototype action
        if (!clipObject)
            clipObject = prototypeAction->_clip;

    }

    // clip must be known when specified via string
    if ( !clipObject ) return nullptr;

    // allocate all resources required to run it
    auto newAction = std::make_shared<AnimationAction>( std::shared_ptr<AnimationMixer>(this), clipObject, optionalRoot, blendMode );

    //this->_bindAction( newAction, prototypeAction );

    // and make the action known to the memory manager
    this->_addInactiveAction( newAction, clipUuid, rootUuid );

    return newAction;
//    return nullptr;

}

void _bindAction( std::shared_ptr<AnimationClip> clip, std::shared_ptr<AnimationAction> prototypeAction ) {

//    const root = action._localRoot || this._root,
//            tracks = action._clip.tracks,
//            nTracks = tracks.length,
//            bindings = action._propertyBindings,
//            interpolants = action._interpolants,
//            rootUuid = root.uuid,
//            bindingsByRoot = this._bindingsByRootAndName;
//
//    let bindingsByName = bindingsByRoot[ rootUuid ];
//
//    if ( bindingsByName === undefined ) {
//
//    bindingsByName = {};
//    bindingsByRoot[ rootUuid ] = bindingsByName;
//
//    }
//
//    for ( let i = 0; i !== nTracks; ++ i ) {
//
//    const track = tracks[ i ],
//            trackName = track.name;
//
//    let binding = bindingsByName[ trackName ];
//
//    if ( binding !== undefined ) {
//
//    ++ binding.referenceCount;
//    bindings[ i ] = binding;
//
//    } else {
//
//    binding = bindings[ i ];
//
//    if ( binding !== undefined ) {
//
//    // existing binding, make sure the cache knows
//
//    if ( binding._cacheIndex === null ) {
//
//    ++ binding.referenceCount;
//    this._addInactiveBinding( binding, rootUuid, trackName );
//
//    }
//
//    continue;

//}
//
//const path = prototypeAction && prototypeAction.
//        _propertyBindings[ i ].binding.parsedPath;
//
//binding = new PropertyMixer(
//        PropertyBinding.create( root, trackName, path ),
//        track.ValueTypeName, track.getValueSize() );
//
//++ binding.referenceCount;
//this._addInactiveBinding( binding, rootUuid, trackName );
//
//bindings[ i ] = binding;
//
//}
//
//interpolants[ i ].resultBuffer = binding.buffer;
//
//}

}

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

    this->_removeInactiveBindingsForAction( action );

    return *this;
}

AnimationMixer& AnimationMixer::_removeInactiveBindingsForAction( std::shared_ptr<AnimationAction> action ) {

    auto bindings = action->_propertyBindings;

    for ( size_t i = 0, n = bindings.size(); i != n; ++ i ) {
        auto binding = bindings[ i ];

        if ( -- binding->referenceCount == 0 ) {
            this->_removeInactiveBinding( binding );
        }
    }
    return *this;
}

AnimationMixer& AnimationMixer::_removeInactiveBinding( std::shared_ptr<PropertyMixer<float>> binding ) {

        auto& bindings = this->_bindings;
        auto propBinding = binding->binding;
//        auto rootUuid = propBinding->rootNode->uuid,
//        trackName = propBinding.path,
//        bindingsByRoot = this._bindingsByRootAndName,
//        bindingByName = bindingsByRoot[ rootUuid ],
//
//        lastInactiveBinding = bindings[ bindings.length - 1 ],
//        cacheIndex = binding._cacheIndex;
//
//        lastInactiveBinding._cacheIndex = cacheIndex;
//        bindings[ cacheIndex ] = lastInactiveBinding;
//        bindings.pop();
//
//        delete bindingByName[ trackName ];
//
//        if ( Object.keys( bindingByName ).length === 0 ) {
//
//            delete bindingsByRoot[ rootUuid ];
//
//        }

    return *this;

}

AnimationMixer& AnimationMixer::update(float deltaTime) {
    auto deltaTimeScale = deltaTime * this->timeScale;

    const auto& actions = this->_actions;
    const auto& nActions = this->_nActiveActions;

    auto time = this->time += deltaTime;
    auto timeDirection = sign( deltaTime );
    auto accuIndex = this->_accuIndex ^= 1;

    // run active actions
    for ( size_t i = 0; i != nActions; ++ i ) {
        auto& action = actions[ i ];

        action->_update( time, deltaTime, timeDirection, accuIndex );
    }

    // update scene graph
//    const bindings = this._bindings,
//            nBindings = this._nActiveBindings;

//    for ( size_t i = 0; i != this->nBindings; ++ i ) {
//        bindings[ i ].apply( accuIndex );
//    }

    return *this;

}