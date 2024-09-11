//
// Created by SeanR on 09/09/2024.
//

#ifndef THREE_CPP_KEYFRAME_TRACK__H
#define THREE_CPP_KEYFRAME_TRACK__H

#endif //THREE_CPP_KEYFRAME_TRACK__H

#include <memory>
#include <vector>

class KeyframeTrack{
public:
    using sptr = std::shared_ptr<KeyframeTrack>;

    int getValueSize(){}
    std::vector<int> getTimes();

    std::shared_ptr<KeyframeTrack> shift(int timeOffset);
    std::shared_ptr<KeyframeTrack> scale(int  timeScale);
    std::shared_ptr<KeyframeTrack> trim(int startTime, int endTime);
    bool validate();
    std::shared_ptr<KeyframeTrack> optimize();
    //clone();
};
