//
// Created by SeanR on 2024/1/9.
//

#include "animation_action.h"

#include "animation_mixer.h"
#include "property_mixer.h"

// State & Scheduling
AnimationAction& AnimationAction::play(){
    this->_mixer->_activateAction( shared_from_this() );

    return *this;
}

AnimationAction& AnimationAction::_update( float time, float deltaTime, int timeDirection, int accuIndex ) {
    // called by the mixer
    if ( !this->enabled ) {
        // call ._updateWeight() to update ._effectiveWeight
        this->_updateWeight( time );
        return *this;
    }

    const auto startTime = this->_startTime;

    if ( startTime != 0 ) {
        // check for scheduled start of action

        const auto timeRunning = ( time - startTime ) * timeDirection;
        if ( timeRunning < 0 || timeDirection == 0 ) {
            deltaTime = 0;
        } else {
            this->_startTime = 0; // unschedule
            deltaTime = timeDirection * timeRunning;
        }
    }

    // apply time scale and advance time
    deltaTime *= this->_updateTimeScale( time );
    const auto clipTime = this->_updateTime( deltaTime );

    // note: _updateTime may disable the action resulting in
    // an effective weight of 0

    const auto weight = this->_updateWeight( time );

    if ( weight > 0 ) {

        auto interpolants = this->_interpolants;
        auto propertyMixers = this->_propertyBindings;

        switch ( this->blendMode ) {

            case AnimationBlendMode::AdditiveAnimationBlendMode:

                for ( size_t j = 0, m = interpolants.size(); j != m; ++ j ) {
                    interpolants[ j ]->evaluate( clipTime );
                    //propertyMixers[ j ].accumulateAdditive( weight );

                }

                break;

            case AnimationBlendMode::NormalAnimationBlendMode:
            default:

                for ( size_t j = 0, m = interpolants.size(); j != m; ++ j ) {
                    interpolants[ j ]->evaluate( clipTime );
                    //todo:fix this
                    //propertyMixers[ j ]->accumulate( accuIndex, weight );

                }

        }

    }
    return *this;

}
