//
// Created by SeanR on 1/9/2024.
//

#include "keyframe_track.h"

#include "interpolant.h"
#include "linear_interpolant.h"

std::shared_ptr<Interpolant> KeyframeTrack::InterpolantFactoryMethodLinear(std::vector<float>& result) {
    return std::make_shared<LinearInterpolant>(times,values,getValueSize(),result);
}

std::shared_ptr<Interpolant> KeyframeTrack::InterpolantFactoryMethodDiscrete(std::vector<float>& result) {
    return std::make_shared<LinearInterpolant>(times,values,getValueSize(),result);
}

std::shared_ptr<Interpolant> KeyframeTrack::InterpolantFactoryMethodSmooth(std::vector<float>& result) {
    return std::make_shared<LinearInterpolant>(times,values,getValueSize(),result);
}