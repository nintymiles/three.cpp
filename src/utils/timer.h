//
// Created by SeanR on 2023/10/18.
//

#ifndef THREE_CPP_TIMER_H
#define THREE_CPP_TIMER_H

#include <ctime>
class Timer{
private:
    clock_t start_time;
    clock_t old_time;

public:
    Timer() : start_time(clock()),old_time(start_time) { }

    float get_elapsed_time() const { return (float)(clock()- start_time); }

    float getDelta() {
        float diff = 0;

        clock_t newTime = clock();
        diff = (newTime - start_time) / 1000.0;
        old_time = newTime;

        return diff;
    }

    void reset() { start_time = clock(); }
};

#endif //THREE_CPP_TIMER_H
