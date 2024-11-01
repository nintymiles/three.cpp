//
// Created by SeanR on 09/09/2024.
//

#ifndef THREE_CPP_KEYFRAME_TRACK__H
#define THREE_CPP_KEYFRAME_TRACK__H

#include <memory>
#include <vector>

#include "linear_interpolant.h"

//template<typename ValueBufferType>
class InterpolantFactoryObject{
private:
    std::vector<float> times;
    std::vector<float> values;
    size_t sampleSize;
    Interpolate interpolation;
public:
    InterpolantFactoryObject() = default;

    InterpolantFactoryObject(const std::vector<float> times, std::vector<float> values,size_t valueSize,Interpolate interpolation)
            :times(times),values(values),sampleSize(valueSize),interpolation(interpolation){}

    std::shared_ptr<Interpolant> operator()(std::vector<float> buffer) {
        switch ( interpolation ) {
            case Interpolate::InterpolateDiscrete:
                return std::make_shared<LinearInterpolant>(times,values,sampleSize,buffer);
                break;
            case Interpolate::InterpolateLinear:
                return std::make_shared<LinearInterpolant>(times,values,sampleSize,buffer);
                break;
            case Interpolate::InterpolateSmooth:
                return std::make_shared<LinearInterpolant>(times,values,sampleSize,buffer);
                break;

        }

    }

    Interpolate getInterpolation(){
        return interpolation;
    }
};


class KeyframeTrack{
public:
    //using sptr = std::shared_ptr<KeyframeTrack>;

    virtual std::string getName() = 0;
    virtual int getValueSize() = 0;
    virtual std::vector<float> getTimes() = 0;

    virtual KeyframeTrack& shift(float timeOffset) = 0;
    virtual KeyframeTrack& scale(float timeScale) = 0;
    virtual KeyframeTrack& trim(float startTime, float endTime) = 0;
    virtual bool validate() = 0;
    virtual KeyframeTrack& optimize() = 0;

    virtual InterpolantFactoryObject getInterpolant() = 0;

    //clone();
};

#endif //THREE_CPP_KEYFRAME_TRACK__H
