//
// Created by SeanR on 16/05/2024.
//

#ifndef THREE_CPP_DISCRETEINTERPOLANT_H
#define THREE_CPP_DISCRETEINTERPOLANT_H

#include "interpolant.h"


class DiscreteInterpolant: public Interpolant{
public:
    DiscreteInterpolant(std::vector<size_t> parameterPositions,std::vector<float> sampleValues,size_t sampleSize,std::vector<float> resultBuffer):
                                                                                    Interpolant(parameterPositions,sampleValues,sampleSize,resultBuffer){

    }

    Interpolant& interpolate_(size_t i1, float t0, float t, float t1) override{
        copySampleValue_(i1);
        return *this;
    }
};


#endif //THREE_CPP_DISCRETEINTERPOLANT_H
