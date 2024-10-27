//
// Created by SeanR on 2024/1/9.
//

#ifndef THREE_CPP_ANIMATION_ACTION_H
#define THREE_CPP_ANIMATION_ACTION_H

#include "object_3d.h"
#include "animation_clip.h"
#include "constants.h"
#include "interpolant.h"

#include <memory>

struct InterpolantSettings{
    InterpolateParam endingStart = InterpolateParam::ZeroCurvatureEnding;
    InterpolateParam endingEnd = InterpolateParam::ZeroCurvatureEnding;
};

class AnimationMixer;
template<typename T> class PropertyMixer;
class AnimationAction:public std::enable_shared_from_this<AnimationAction>{
private:

    LoopMode loop;
    int _loopCount = - 1;
    int repetitions;

    InterpolantSettings _interpolantSettings;

    float weight = 1;
    float _effectiveWeight = 1;

    std::shared_ptr<Interpolant>  _timeScaleInterpolant;
    std::shared_ptr<Interpolant> _weightInterpolant = nullptr;

    // global mixer time when the action is to be started
    // it's set back to 'null' upon start of the action
    float _startTime = 0;

    // scaled local time of the action
    // gets clamped or wrapped to 0..clip.duration according to loop
    float time = 0;

    float timeScale = 1;
    float _effectiveTimeScale = 1;

    bool paused = false; // true -> zero effective time scale
    bool enabled = true; // false -> zero effective weight

    bool clampWhenFinished = false;// keep feeding the last frame?

    bool zeroSlopeAtStart = true;// for smooth interpolation w/o separate
    bool zeroSlopeAtEnd = true;// clips for start, loop and end

public:
    using sptr = std::shared_ptr<AnimationAction>;
    std::shared_ptr<AnimationMixer> _mixer;
    std::vector<std::shared_ptr<Interpolant>> _interpolants;
    AnimationClip::sptr _clip;
    Object3D::sptr _localRoot;
    AnimationBlendMode blendMode;
    size_t _cacheIndex; // for the memory manager
    size_t _byClipCacheIndex; // for the memory manager


    // inside: PropertyMixer (managed by the mixer)
    std::vector<std::shared_ptr<PropertyMixer<float>>> _propertyBindings;

    AnimationAction(std::shared_ptr<AnimationMixer> mixer, AnimationClip::sptr clip, Object3D::sptr localRoot, AnimationBlendMode blendMode):
                        _mixer(mixer),_clip(clip),_localRoot(localRoot),blendMode(blendMode){
//        if(blendMode == AnimationBlendMode::Unknown)
//            blendMode =

        auto& tracks = clip->tracks;
        auto nTracks = tracks.size();
        std::vector<std::shared_ptr<Interpolant>> interpolants{};

        for ( size_t i = 0; i != nTracks; ++ i ) {

            auto track = tracks[ i ];
            if(track) {
                InterpolantFactoryObject interpolantFactoryObject = track->getInterpolant();
                auto interpolant = interpolantFactoryObject({});
                interpolants.push_back(interpolant);
                //todo:fixit
                //interpolant->settings = interpolantSettings;
            }

        }

        this->_interpolantSettings = InterpolantSettings();

        this->_interpolants = interpolants; // bound by the mixer
    }

    // State & Scheduling
    AnimationAction& play();

    AnimationAction& stop(){
        //todo:fix this
        return *this;
    }

    AnimationAction& reset(){
        //todo:fix this
        return *this;
    }

    bool isRunning(){
        //todo:fixthis
        return true;
    }

    // return true when play has been called
    bool isScheduled(){
        //todo:fixthis
        return false;
    }

    AnimationAction& startAt( float time ){
        //todo:fix this
        return *this;
    }

    AnimationAction& setLoop( LoopMode mode, int repetitions ) {
        this->loop = mode;
        this->repetitions = repetitions;

        return *this;
    }
    AnimationAction& setEffectiveWeight(float weight){
        //todo:fix this
        return *this;
    }
    //todo:fix this
    //float getEffectiveWeight();

    AnimationAction& fadeIn(float duration){
        //todo:fix this
        return *this;
    }
    AnimationAction& fadeOut(float duration){
        //todo:fix this
        return *this;
    }
    AnimationAction& crossFadeFrom(AnimationAction::sptr fadeOutAction, float duration, bool warp){
        //todo:fix this
        return *this;
    }
    AnimationAction& crossFadeTo(AnimationAction::sptr fadeInAction, float duration, bool warp){
        //todo:fix this
        return *this;
    }
    AnimationAction& stopFading(){
        //todo:fix this
        return *this;
    }
    AnimationAction& setEffectiveTimeScale(float timeScale){
        //todo:fix this
        return *this;
    }

    //todo: fix this
//    float getEffectiveTimeScale();

    AnimationAction& setDuration(float duration){
        //todo:fix this
        return *this;
    }
    AnimationAction& syncWith(AnimationAction::sptr action){
        //todo:fix this
        return *this;
    }
    AnimationAction& halt(float duration){
        //todo:fix this
        return *this;
    }
    AnimationAction& warp(float statTimeScale,float endTimeScale,float duration){
        //todo:fix this
        return *this;
    }
    AnimationAction& stopWarping(){
        //todo:fix this
        return *this;
    }

    //todo:fix this
//    std::shared_ptr<AnimationMixer> getMixer();
//    std::shared_ptr<AnimationClip> getClip();
//    Object3D::sptr getRoot();


    // Interna
    AnimationAction& _update( float time, float deltaTime, int timeDirection, int accuIndex ) {
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
                        //propertyMixers[ j ].accumulate( accuIndex, weight );

                    }

            }

        }

    }

    float _updateWeight( float time ) {
            float weight = 0;

            if ( this->enabled ) {
                weight = this->weight;
                auto interpolant = this->_weightInterpolant;

                if ( interpolant != nullptr ) {
                    //todo:fixit return interpolated value
                    //auto interpolantValue = interpolant->evaluate( time );
                    auto interpolantValue = 0.3f;

                    weight *= interpolantValue;

                    //todo:fixit public parameterPositions
                    if ( time > interpolant->parameterPositions[ 1 ] ) {

                        this->stopFading();

                        if ( interpolantValue == 0 ) {
                            // faded out, disable
                            this->enabled = false;
                        }

                    }

                }

            }

            this->_effectiveWeight = weight;
            return weight;

    }

    float _updateTimeScale( float time ) {

            float timeScale = 0;

            if ( ! this->paused ) {

                timeScale = this->timeScale;

                auto interpolant = this->_timeScaleInterpolant;

                if ( interpolant != nullptr ) {

                    //todo:fix it
                    //const interpolantValue = interpolant.evaluate( time )[ 0 ];
                    float interpolantValue = .3f;

                    timeScale *= interpolantValue;

                    if ( time > interpolant->parameterPositions[ 1 ] ) {

                        this->stopWarping();

                        if ( timeScale == 0 ) {
                            // motion has halted, pause
                            this->paused = true;
                        } else {
                            // warp done - apply final time scale
                            this->timeScale = timeScale;
                        }
                    }
                }

            }

            this->_effectiveTimeScale = timeScale;
            return timeScale;

    }

    float _updateTime( float deltaTime ) {

            const auto duration = this->_clip->duration;
            const auto loop = this->loop;

            auto time = this->time + deltaTime;
            auto loopCount = this->_loopCount;

            const auto pingPong = ( loop == LoopMode::LoopPingPong );

            if ( deltaTime == 0 ) {
                if ( loopCount == - 1 ) return time;

                return ( pingPong && ( loopCount & 1 ) == 1 ) ? duration - time : time;
            }

            if ( loop == LoopMode::LoopOnce ) {

                if ( loopCount == - 1 ) {
                    // just started
                    this->_loopCount = 0;
                    this->_setEndings( true, true, false );
                }

                //handle_stop:{}
                {

                    if ( time >= duration ) {
                        time = duration;
                    } else if ( time < 0 ) {
                        time = 0;
                    } else {
                        this->time = time;
                    }

                    if ( this->clampWhenFinished ) this->paused = true;
                    else this->enabled = false;

                    this->time = time;

    //                this._mixer.dispatchEvent( {
    //                                                   type: 'finished', action: this,
    //                                                   direction: deltaTime < 0 ? - 1 : 1
    //                                           } );

                }

            } else { // repetitive Repeat or PingPong

                if ( loopCount == - 1 ) {
                    // just started
                    if ( deltaTime >= 0 ) {

                        loopCount = 0;

                        this->_setEndings( true, this->repetitions == 0, pingPong );

                    } else {

                        // when looping in reverse direction, the initial
                        // transition through zero counts as a repetition,
                        // so leave loopCount at -1

                        this->_setEndings( this->repetitions == 0, true, pingPong );

                    }

                }

                if ( time >= duration || time < 0 ) {

                    // wrap around
                    const auto loopDelta = floor( time / duration ); // signed
                    time -= duration * loopDelta;

                    loopCount += abs( loopDelta );

                    const auto pending = this->repetitions - loopCount;

                    if ( pending <= 0 ) {

                        // have to stop (switch state, clamp time, fire event)
                        if ( this->clampWhenFinished ) this->paused = true;
                        else this->enabled = false;

                        time = deltaTime > 0 ? duration : 0;

                        this->time = time;

//                        this._mixer.dispatchEvent( {
//                                                           type: 'finished', action: this,
//                                                           direction: deltaTime > 0 ? 1 : - 1
//                                                   } );

                    } else {

                        // keep running
                        if ( pending == 1 ) {

                            // entering the last round
                            const auto atStart = deltaTime < 0;
                            this->_setEndings( atStart, ! atStart, pingPong );

                        } else {

                            this->_setEndings( false, false, pingPong );

                        }

                        this->_loopCount = loopCount;

                        this->time = time;

//                        this._mixer.dispatchEvent( {
//                                                           type: 'loop', action: this, loopDelta: loopDelta
//                                                   } );

                    }

                } else {

                    this->time = time;

                }

                if ( pingPong && ( loopCount & 1 ) == 1 ) {

                    // invert time for the "pong round"

                    return duration - time;

                }

            }

            return time;

    }

    void _setEndings( bool atStart, bool atEnd, bool pingPong ) {

        auto settings = this->_interpolantSettings;

        if ( pingPong ) {
            settings.endingStart = InterpolateParam::ZeroSlopeEnding;
            settings.endingEnd = InterpolateParam::ZeroSlopeEnding;
        } else {
            // assuming for LoopOnce atStart == atEnd == true
            if ( atStart ) {
                settings.endingStart = this->zeroSlopeAtStart ? InterpolateParam::ZeroSlopeEnding : InterpolateParam::ZeroCurvatureEnding;
            } else {
                settings.endingStart = InterpolateParam::WrapAroundEnding;
            }

            if ( atEnd ) {
                settings.endingEnd = this->zeroSlopeAtEnd ? InterpolateParam::ZeroSlopeEnding : InterpolateParam::ZeroCurvatureEnding;
            } else {
                settings.endingEnd 	 = InterpolateParam::WrapAroundEnding;
            }
        }

    }

    AnimationAction& _scheduleFading( float duration, float weightNow, float weightThen ) {

        const auto mixer = this->_mixer;

        //todo: fix this
        //const now = mixer->time;
//        let interpolant = this._weightInterpolant;
//
//        if ( interpolant === null ) {
//
//            interpolant = mixer._lendControlInterpolant();
//            this._weightInterpolant = interpolant;
//
//        }
//
//        const times = interpolant.parameterPositions,
//                values = interpolant.sampleValues;
//
//        times[ 0 ] = now;
//        values[ 0 ] = weightNow;
//        times[ 1 ] = now + duration;
//        values[ 1 ] = weightThen;

        return *this;

    }

};


#endif //THREE_CPP_ANIMATION_ACTION_H
