//
// Created by SeanR on 17/05/2024.
//

#ifndef THREE_CPP_QUATERNIONLINEARINTERPOLANT_H
#define THREE_CPP_QUATERNIONLINEARINTERPOLANT_H

#include "interpolant.h"
#include "quaternion.h"


class QuaternionLinearInterpolant: public Interpolant{
public:
    QuaternionLinearInterpolant(std::vector<float> parameterPositions,std::vector<float> sampleValues,size_t sampleSize,std::vector<float> resultBuffer):
            Interpolant(parameterPositions,sampleValues,sampleSize,resultBuffer){

    }

    Interpolant& interpolate_(size_t i1, float t0, float t, float t1) override{
        auto& result = resultBuffer,
        values = sampleValues;
        auto stride = valueSize;
        auto alpha = ( t - t0 ) / ( t1 - t0 );

        auto offset = i1 * stride;

        for ( auto end = offset + stride; offset != end; offset += 4 ) {
            Quaternion::slerpFlat( result, 0, values, offset - stride, values, offset, alpha );
        }

        return *this;
    }
};

#endif //THREE_CPP_QUATERNIONLINEARINTERPOLANT_H
