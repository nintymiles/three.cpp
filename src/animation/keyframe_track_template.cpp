//
// Created by SeanR on 1/9/2024.
//

#include "keyframe_track_template.h"

#include "interpolant.h"
#include "linear_interpolant.h"

//template<typename ValueBufferType, typename DefaultInterpolation>
//std::shared_ptr<Interpolant> KeyframeTrackTemplate::InterpolantFactoryMethodLinear(std::vector<float>& result) {
//    return std::make_shared<LinearInterpolant>(times,values,getValueSize(),result);
//}

//std::shared_ptr<Interpolant> KeyframeTrackTemplate::InterpolantFactoryMethodDiscrete(std::vector<float>& result) {
//    return std::make_shared<LinearInterpolant>(times,values,getValueSize(),result);
//}
//
//std::shared_ptr<Interpolant> KeyframeTrackTemplate::InterpolantFactoryMethodSmooth(std::vector<float>& result) {
//    return std::make_shared<LinearInterpolant>(times,values,getValueSize(),result);
//}