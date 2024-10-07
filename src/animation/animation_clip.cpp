//
// Created by ultraman on 1/9/2024.
//

#include "animation_clip.h"
#include "keyframe_track_template.h"

AnimationClip& AnimationClip::resetDuration() {

    auto& tracks = this->tracks;
    float duration = 0;

    for ( size_t i = 0, n = tracks.size(); i != n; ++ i ) {
        auto& track = tracks[ i ];

        duration = math::max( duration, (float)track->getTimes()[ track->getTimes().size() - 1 ] );
    }

    this->duration = duration;

    return *this;
}