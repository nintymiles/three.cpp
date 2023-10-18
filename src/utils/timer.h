//
// Created by SeanR on 2023/10/18.
//

#ifndef THREE_CPP_TIMER_H
#define THREE_CPP_TIMER_H

#include <ctime>
class Timer{
private:
    clock_t start_time;

public:
    Timer() : start_time(clock()) { }

    float get_elapsed_time() const { return (float)(clock()- start_time); }

    void reset() { start_time = clock(); }
};

#endif //THREE_CPP_TIMER_H
