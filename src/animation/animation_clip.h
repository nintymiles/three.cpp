//
// Created by ultraman on 1/9/2024.
//

#ifndef THREE_CPP_ANIMATION_CLIP_H
#define THREE_CPP_ANIMATION_CLIP_H

#include "constants.h"
#include "keyframe_track.h"

#include "object_3d.h"

#include <string>
#include "sole.h"

class AnimationClip {
public:
    std::string name;
    std::vector<KeyframeTrack::sptr> tracks;
    float duration;
    AnimationBlendMode blendMode;
    sole::uuid uuid;
public:
    using sptr = std::shared_ptr<AnimationClip>;

    AnimationClip(std::string name, std::vector<KeyframeTrack::sptr> tracks, float duration = - 1, AnimationBlendMode blendMode = AnimationBlendMode::NormalAnimationBlendMode):
                name(name),tracks(tracks),duration(duration),blendMode(blendMode),uuid(sole::uuid1()){
        // this means it should figure out its duration by scanning the tracks
        if ( this->duration < 0 ) {
            this->resetDuration();
        }
    }

    static Object3D::sptr findObjectByName( Object3D::sptr parentObj, std::string name ) {
        if(parentObj) {
            for (size_t i = 0; i < parentObj->children.size(); i++) {
                if (parentObj->children[i]->name == name) {
                    return parentObj->children[i];
                }
            }
        }

        return nullptr;

    }

    static std::shared_ptr<AnimationClip> findByName( std::vector<std::shared_ptr<AnimationClip>> clipArray, std::string name ) {

        for ( size_t i = 0; i < clipArray.size(); i ++ ) {

            if ( clipArray[ i ]->name == name ) {
                return clipArray[ i ];
            }

        }

        return nullptr;

    }

//    // parse the animation.hierarchy format
//    static std::shared_ptr<AnimationClip> parseAnimation( animation, bones ) {
//
//        if ( ! animation ) {
//
//            console.error( 'THREE.AnimationClip: No animation in JSONLoader data.' );
//            return null;
//
//        }
//
//        const addNonemptyTrack = function ( trackType, trackName, animationKeys, propertyName, destTracks ) {
//
//            // only return track if there are actually keys.
//            if ( animationKeys.length !== 0 ) {
//
//                const times = [];
//                const values = [];
//
//                AnimationUtils.flattenJSON( animationKeys, times, values, propertyName );
//
//                // empty keys are filtered out, so check again
//                if ( times.length !== 0 ) {
//
//                    destTracks.push( new trackType( trackName, times, values ) );
//
//                }
//
//            }
//
//        };
//
//        const tracks = [];
//
//        const clipName = animation.name || 'default';
//        const fps = animation.fps || 30;
//        const blendMode = animation.blendMode;
//
//        // automatic length determination in AnimationClip.
//        let duration = animation.length || - 1;
//
//        const hierarchyTracks = animation.hierarchy || [];
//
//        for ( let h = 0; h < hierarchyTracks.length; h ++ ) {
//
//            const animationKeys = hierarchyTracks[ h ].keys;
//
//            // skip empty tracks
//            if ( ! animationKeys || animationKeys.length === 0 ) continue;
//
//            // process morph targets
//            if ( animationKeys[ 0 ].morphTargets ) {
//
//                // figure out all morph targets used in this track
//                const morphTargetNames = {};
//
//                let k;
//
//                for ( k = 0; k < animationKeys.length; k ++ ) {
//
//                    if ( animationKeys[ k ].morphTargets ) {
//
//                        for ( let m = 0; m < animationKeys[ k ].morphTargets.length; m ++ ) {
//
//                            morphTargetNames[ animationKeys[ k ].morphTargets[ m ] ] = - 1;
//
//                        }
//
//                    }
//
//                }
//
//                // create a track for each morph target with all zero
//                // morphTargetInfluences except for the keys in which
//                // the morphTarget is named.
//                for ( const morphTargetName in morphTargetNames ) {
//
//                    const times = [];
//                    const values = [];
//
//                    for ( let m = 0; m !== animationKeys[ k ].morphTargets.length; ++ m ) {
//
//                        const animationKey = animationKeys[ k ];
//
//                        times.push( animationKey.time );
//                        values.push( ( animationKey.morphTarget === morphTargetName ) ? 1 : 0 );
//
//                    }
//
//                    tracks.push( new NumberKeyframeTrack( '.morphTargetInfluence[' + morphTargetName + ']', times, values ) );
//
//                }
//
//                duration = morphTargetNames.length * fps;
//
//            } else {
//
//                // ...assume skeletal animation
//
//                const boneName = '.bones[' + bones[ h ].name + ']';
//
//                addNonemptyTrack(
//                        VectorKeyframeTrack, boneName + '.position',
//                        animationKeys, 'pos', tracks );
//
//                addNonemptyTrack(
//                        QuaternionKeyframeTrack, boneName + '.quaternion',
//                        animationKeys, 'rot', tracks );
//
//                addNonemptyTrack(
//                        VectorKeyframeTrack, boneName + '.scale',
//                        animationKeys, 'scl', tracks );
//
//            }
//
//        }
//
//        if ( tracks.length === 0 ) {
//
//            return null;
//
//        }
//
//        const clip = new this( clipName, duration, tracks, blendMode );
//
//        return clip;
//
//    }

    AnimationClip& resetDuration() {

        auto& tracks = this->tracks;
        float duration = 0;

        for ( size_t i = 0, n = tracks.size(); i != n; ++ i ) {
            auto& track = tracks[ i ];

            duration = math::max( duration, (float)track->times[ track->times.size() - 1 ] );

        }

        this->duration = duration;

        return *this;
    }

    AnimationClip& trim() {

        for ( size_t i = 0; i < this->tracks.size(); i ++ ) {
            this->tracks[ i ]->trim( 0, this->duration );
        }

        return *this;
    }

    bool validate() {

        bool valid = true;

        for ( size_t i = 0; i < this->tracks.size(); i ++ ) {

            valid = valid && this->tracks[ i ]->validate();

        }

        return valid;
    }

    AnimationClip& optimize() {

        for ( size_t i = 0; i < this->tracks.size(); i ++ ) {
            this->tracks[ i ]->optimize();

        }

        return *this;

    }



};

//function getTrackTypeForValueTypeName( typeName ) {
//
//    switch ( typeName.toLowerCase() ) {
//
//        case 'scalar':
//        case 'double':
//        case 'float':
//        case 'number':
//        case 'integer':
//
//            return NumberKeyframeTrack;
//
//        case 'vector':
//        case 'vector2':
//        case 'vector3':
//        case 'vector4':
//
//            return VectorKeyframeTrack;
//
//        case 'color':
//
//            return ColorKeyframeTrack;
//
//        case 'quaternion':
//
//            return QuaternionKeyframeTrack;
//
//        case 'bool':
//        case 'boolean':
//
//            return BooleanKeyframeTrack;
//
//        case 'string':
//
//            return StringKeyframeTrack;
//
//    }
//
//    throw new Error( 'THREE.KeyframeTrack: Unsupported typeName: ' + typeName );
//
//}


#endif //THREE_CPP_ANIMATION_CLIP_H
