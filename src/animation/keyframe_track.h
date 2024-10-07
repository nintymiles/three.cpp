//
// Created by SeanR on 09/09/2024.
//

#ifndef THREE_CPP_KEYFRAME_TRACK__H
#define THREE_CPP_KEYFRAME_TRACK__H



#include <memory>
#include <vector>

class KeyframeTrack{
public:
    //using sptr = std::shared_ptr<KeyframeTrack>;

    virtual int getValueSize() = 0;
    virtual std::vector<float> getTimes() = 0;

    virtual KeyframeTrack& shift(float timeOffset) = 0;
    virtual KeyframeTrack& scale(float timeScale) = 0;
    virtual KeyframeTrack& trim(float startTime, float endTime) = 0;
    virtual bool validate() = 0;
    virtual KeyframeTrack& optimize() = 0;
    //clone();
};

#endif //THREE_CPP_KEYFRAME_TRACK__H
