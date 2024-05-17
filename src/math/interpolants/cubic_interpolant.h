//
// Created by SeanR on 2024/5/15.
//

#ifndef THREE_CPP_CUBIC_INTERPOLANT_H
#define THREE_CPP_CUBIC_INTERPOLANT_H

#include "interpolant.h"
#include "constants.h"

/**
 * Fast and simple cubic spline interpolant.
 *
 * It was derived from a Hermitian construction setting the first derivative
 * at each sample position to the linear slope between neighboring positions
 * over their parameter interval.
 */
class CubicInterpolant: public Interpolant{

public:
//    struct CubicInterpolantSetting{
//        InterpolateParam endingStart;
//        InterpolateParam endingEnd;
//    };
    CubicInterpolant(std::vector<size_t> parameterPositions,std::vector<float> sampleValues,size_t sampleSize,std::vector<float> resultBuffer): Interpolant(parameterPositions,sampleValues,sampleSize,resultBuffer){

        _weightPrev = - 0;
        _offsetPrev = - 0;
        _weightNext = - 0;
        _offsetNext = - 0;

        defaultSettings_ = {InterpolateParam::ZeroCurvatureEnding,InterpolateParam::ZeroCurvatureEnding};
    }

//    virtual Interpolant& interpolate_( size_t i1,float t0,float t,float t1/* i1, t0, t, t1 */ ) override{
//
//        auto& result = this->resultBuffer;
//        auto& values = this->sampleValues;
//        auto stride = this->valueSize;
//
//        auto offset1 = i1 * stride,offset0 = offset1 - stride;
//        auto weight1 = ( t - t0 ) / ( t1 - t0 ),weight0 = 1 - weight1;
//
//        for ( size_t i = 0; i != stride; ++ i ) {
//            result[ i ] =
//                    values[ offset0 + i ] * weight0 +
//                    values[ offset1 + i ] * weight1;
//
//        }
//
//        return *this;
//    }
    CubicInterpolant& interpolate_(size_t i1,float t0,float t,float t1) {

        auto& result = this->resultBuffer;
        auto& values = this->sampleValues;
        auto stride = this->valueSize;

        auto o1 = i1 * stride, o0 = o1 - stride,
        oP = _offsetPrev, oN = _offsetNext;

        auto wP = _weightPrev, wN = _weightNext,
        p = (t - t0) / (t1 - t0),
        pp = p * p,
        ppp = pp * p;

        // evaluate polynomials
        const float sP = -wP * ppp + 2 * wP * pp - wP * p;
        const float s0 = (1 + wP) * ppp + (-1.5 - 2 * wP) * pp + (-0.5 + wP) * p + 1;
        const float s1 = (-1 - wN) * ppp + (1.5 + wN) * pp + 0.5 * p;
        const float sN = wN * ppp - wN * pp;

        // combine data linearly
        for (size_t i = 0; i != stride; ++i) {
            result[i] =
                    sP * values[oP + i] +
                    s0 * values[o0 + i] +
                    s1 * values[o1 + i] +
                    sN * values[oN + i];

        }

        return *this;

    }

    CubicInterpolant& intervalChanged_( size_t i1,float t0,float t1) {

        auto &pp = this->parameterPositions;
        int iPrev = (int) i1 - 2;
        size_t iNext = i1 + 1, tNext = pp[iNext];

        size_t tPrev;
        if (iPrev < 0) {
            switch (this->getSettings_().endingStart) {
                case InterpolateParam::ZeroSlopeEnding:
                    // f'(t0) = 0
                    iPrev = i1;
                    tPrev = 2 * t0 - t1;

                    break;

                case InterpolateParam::WrapAroundEnding:
                    // use the other end of the curve
                    iPrev = pp.size() - 2;
                    tPrev = t0 + pp[iPrev] - pp[iPrev + 1];

                    break;

                default: // ZeroCurvatureEnding
                    // f''(t0) = 0 a.k.a. Natural Spline
                    iPrev = i1;
                    tPrev = t1;
            }

        } else {
            tPrev = pp[iPrev];
        }

//        size_t tNext;
//        if (iNext < 0) {
//            switch (this->getSettings_().endingEnd) {
//
//                case InterpolateParam::ZeroSlopeEnding:
//                    // f'(tN) = 0
//                    iNext = i1;
//                    tNext = 2 * t1 - t0;
//
//                    break;
//
//                case InterpolateParam::WrapAroundEnding:
//                    // use the other end of the curve
//                    iNext = 1;
//                    tNext = t1 + pp[1] - pp[0];
//
//                    break;
//
//                default: // ZeroCurvatureEnding
//                    // f''(tN) = 0, a.k.a. Natural Spline
//                    iNext = i1 - 1;
//                    tNext = t0;
//            }
//
//        }else{
//                tNext = pp[iNext];
//        }

        const float halfDt = (t1 - t0) * 0.5;
        auto stride = this->valueSize;

        this->_weightPrev = halfDt / (t0 - tPrev);
        this->_weightNext = halfDt / (tNext - t1);
        this->_offsetPrev = iPrev * stride;
        this->_offsetNext = iNext * stride;

    }




private:
    size_t  _offsetPrev,_offsetNext;
    float _weightPrev,_weightNext;


};



//class CubicInterpolant extends Interpolant {
//
//constructor( parameterPositions, sampleValues, sampleSize, resultBuffer ) {
//
//super( parameterPositions, sampleValues, sampleSize, resultBuffer );
//
//this._weightPrev = - 0;
//this._offsetPrev = - 0;
//this._weightNext = - 0;
//this._offsetNext = - 0;
//
//this.DefaultSettings_ = {
//
//        endingStart: ZeroCurvatureEnding,
//        endingEnd: ZeroCurvatureEnding
//
//};
//
//}
//
//intervalChanged_( i1, t0, t1 ) {
//
//const pp = this.parameterPositions;
//let iPrev = i1 - 2,
//        iNext = i1 + 1,
//
//        tPrev = pp[ iPrev ],
//        tNext = pp[ iNext ];
//
//if ( tPrev === undefined ) {
//
//switch ( this.getSettings_().endingStart ) {
//
//case ZeroSlopeEnding:
//
//// f'(t0) = 0
//iPrev = i1;
//tPrev = 2 * t0 - t1;
//
//break;
//
//case WrapAroundEnding:
//
//// use the other end of the curve
//iPrev = pp.length - 2;
//tPrev = t0 + pp[ iPrev ] - pp[ iPrev + 1 ];
//
//break;
//
//default: // ZeroCurvatureEnding
//
//// f''(t0) = 0 a.k.a. Natural Spline
//iPrev = i1;
//tPrev = t1;
//
//}
//
//}
//
//if ( tNext === undefined ) {
//
//switch ( this.getSettings_().endingEnd ) {
//
//case ZeroSlopeEnding:
//
//// f'(tN) = 0
//iNext = i1;
//tNext = 2 * t1 - t0;
//
//break;
//
//case WrapAroundEnding:
//
//// use the other end of the curve
//iNext = 1;
//tNext = t1 + pp[ 1 ] - pp[ 0 ];
//
//break;
//
//default: // ZeroCurvatureEnding
//
//// f''(tN) = 0, a.k.a. Natural Spline
//iNext = i1 - 1;
//tNext = t0;
//
//}
//
//}
//
//const halfDt = ( t1 - t0 ) * 0.5,
//        stride = this.valueSize;
//
//this._weightPrev = halfDt / ( t0 - tPrev );
//this._weightNext = halfDt / ( tNext - t1 );
//this._offsetPrev = iPrev * stride;
//this._offsetNext = iNext * stride;
//
//}
//
//interpolate_( i1, t0, t, t1 ) {
//
//const result = this.resultBuffer,
//        values = this.sampleValues,
//        stride = this.valueSize,
//
//        o1 = i1 * stride,		o0 = o1 - stride,
//        oP = this._offsetPrev, 	oN = this._offsetNext,
//        wP = this._weightPrev,	wN = this._weightNext,
//
//        p = ( t - t0 ) / ( t1 - t0 ),
//        pp = p * p,
//        ppp = pp * p;
//
//// evaluate polynomials
//
//const sP = - wP * ppp + 2 * wP * pp - wP * p;
//const s0 = ( 1 + wP ) * ppp + ( - 1.5 - 2 * wP ) * pp + ( - 0.5 + wP ) * p + 1;
//const s1 = ( - 1 - wN ) * ppp + ( 1.5 + wN ) * pp + 0.5 * p;
//const sN = wN * ppp - wN * pp;
//
//// combine data linearly
//
//for ( let i = 0; i !== stride; ++ i ) {
//
//result[ i ] =
//sP * values[ oP + i ] +
//s0 * values[ o0 + i ] +
//s1 * values[ o1 + i ] +
//sN * values[ oN + i ];
//
//}
//
//return result;
//
//}
//
//}


#endif //THREE_CPP_CUBIC_INTERPOLANT_H
