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