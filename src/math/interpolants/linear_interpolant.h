//
// Created by SeanR on 1/9/2024.
//

#ifndef THREE_CPP_LINEAR_INTERPOLANT_H
#define THREE_CPP_LINEAR_INTERPOLANT_H

#include "interpolant.h"
class LinearInterpolant:public Interpolant{
public:

    LinearInterpolant(std::vector<float> parameterPositions,std::vector<float> sampleValues,size_t sampleSize,std::vector<float> resultBuffer): Interpolant(parameterPositions,sampleValues,sampleSize,resultBuffer){

    }

    virtual Interpolant& interpolate_( size_t i1,float t0,float t,float t1/* i1, t0, t, t1 */ ) override{

        auto& result = this->resultBuffer;
        auto& values = this->sampleValues;
        auto stride = this->valueSize;

        auto offset1 = i1 * stride,offset0 = offset1 - stride;
        auto weight1 = ( t - t0 ) / ( t1 - t0 ),weight0 = 1 - weight1;

        for ( size_t i = 0; i != stride; ++ i ) {
            result[ i ] =
            values[ offset0 + i ] * weight0 +
            values[ offset1 + i ] * weight1;

        }

        return *this;
    }

};

#endif //THREE_CPP_LINEAR_INTERPOLANT_H
