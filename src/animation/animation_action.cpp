//
// Created by SeanR on 2024/1/9.
//

#include "animation_action.h"

#include "animation_mixer.h"

// State & Scheduling
AnimationAction& AnimationAction::play(){
    this->_mixer->_activateAction( shared_from_this() );

    return *this;
}
