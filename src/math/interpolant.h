//
// Created by SeanR on 1/9/2024.
//

#ifndef THREE_CPP_INTERPOLANT_H
#define THREE_CPP_INTERPOLANT_H

/**
 * Abstract base class of interpolants over parametric samples.
 *
 * The parameter domain is one dimensional, typically the time or a path
 * along a curve defined by the data.
 *
 * The sample values can have any dimensionality and derived classes may
 * apply special interpretations to the data.
 *
 * This class provides the interval seek in a Template Method, deferring
 * the actual interpolation to derived classes.
 *
 * Time complexity is O(1) for linear access crossing at most two points
 * and O(log N) for random access, where N is the number of positions.
 *
 * References:
 *
 * 		http://www.oodesign.com/template-method-pattern.html
 *
 */

#include <vector>
#include <iostream>

#include "constants.h"

struct InterpolantSetting{
    InterpolateParam endingStart;
    InterpolateParam endingEnd;
};

class Interpolant {
protected:
    size_t _cachedIndex = 0;

    std::vector<float> sampleValues;
    size_t valueSize = 0;
    InterpolantSetting defaultSettings_;
    InterpolantSetting settings;
public:
    std::vector<float> parameterPositions;
    std::vector<float> resultBuffer;

    Interpolant(std::vector<float> parameterPositions,std::vector<float> sampleValues,size_t sampleSize,std::vector<float> resultBuffer):parameterPositions(parameterPositions),sampleValues(sampleValues),valueSize(sampleSize),resultBuffer(resultBuffer){
        if(resultBuffer.empty())
            resultBuffer = std::vector<float>(sampleSize);

    }


    Interpolant& evaluate( float t ) {

        auto& pp = this->parameterPositions;
        auto i1 = this->_cachedIndex;
        size_t i0 = (i1<=0)?0:(i1-1);
        auto t1 = pp[ i1 ],t0 = pp[ i0 ];

        if(validate_interval(t,t0,t1,i1)) {
            this->interpolate_(i1, t0, t, t1);
        }

        return *this;
    }

    InterpolantSetting& getSettings_() {
        return defaultSettings_;
    }

    // Template methods for derived classes:
    virtual Interpolant& interpolate_( size_t i1,float t0,float t,float t1/* i1, t0, t, t1 */ ) {
        //throw new Error( 'call to abstract method' );
        std::cerr << "call to abstract method" << std::endl;
        // implementations shall return this.resultBuffer
        return *this;
    }

    virtual Interpolant& intervalChanged_( size_t i1,float t0,float t1/* i1, t0, t1 */ ) {
        // empty
        return *this;
    }

protected:
    std::vector<float>& copySampleValue_( size_t index ) {
        // copies a sample value to the result buffer
        auto& result = this->resultBuffer;
        auto& values = this->sampleValues;
        auto stride = this->valueSize;
        auto offset = index * stride;

        for ( int i = 0; i != stride; ++ i ) {
            result[ i ] = values[ offset + i ];
        }

        return result;
    }

private:
    //- See http://jsperf.com/comparison-to-undefined/3
    //- slower code:
    //-
    //- 				if ( t >= t1 || t1 === undefined ) {
    //auto forward_scan = [&](){
    bool forward_scan(float &t,float &t0,float &t1,size_t &right,size_t &i1)
    {
        auto& pp = this->parameterPositions;
        //auto i1 = this->_cachedIndex;

        if ( ! ( t < t1 ) ) {

            for (auto giveUpAt = i1 + 2;;) {

                if (t1 < 0) {
                    if (t < t0) break; //forward_scan;

                    // after end
                    i1 = pp.size()-1;
                    this->_cachedIndex = i1;
                    this->copySampleValue_((i1<=0)?0:(i1-1));
                }

                if (i1 == giveUpAt) break; // this loop

                t0 = t1;
                ++i1;
                t1 = pp[(i1>=pp.size())?pp.size()-1:i1];

                if (t < t1) {
                    // we have arrived at the sought interval
                    //break; //seek;
                    return true;
                }

            }

            // prepare binary search on the right side of the index
            right = pp.size();
            //return *this;//std::vector<float>{};
            return false;
            //linear_scan;
        }
        return false;
    } //forward_scan();

    bool linear_scan(float &t,float &t0,float &t1,size_t &right,size_t &i1){
        auto& pp = this->parameterPositions;
        //auto i1 = this->_cachedIndex;

        if(!forward_scan(t,t0,t1,right,i1)){

            //- slower code:
            //-	if ( t < t0 || t0 === undefined ) {
            if (!(t >= t0)) {

                // looping?
                auto t1global = pp[1];

                if (t < t1global) {
                    i1 = 2; // + 1, using the scan for the details
                    t0 = t1global;

                }

                // linear reverse scan
                for (auto giveUpAt = i1 - 2;;) {

                    if (t0 < 0) {
                        // before start
                        this->_cachedIndex = 0;
                        this->copySampleValue_(0);

                    }

                    if (i1 == giveUpAt) break; // this loop

                    t1 = t0;
                    t0 = pp[--i1 - 1];

                    if (t >= t0) {
                        // we have arrived at the sought interval
                        break;
                    }

                }

                // prepare binary search on the left side of the index
                right = i1;
                i1 = 0;
                //break linear_scan;
                return false;//std::vector<float>();
            }
            return false;
            // the interval is valid
            //break validate_interval;
            //return *this;//std::vector<float>();
        }

    } // linear scan
    //linear_scan();

    bool seek(float &t,float &t0,float &t1,size_t &i1){
        auto& pp = this->parameterPositions;
        //auto i1 = this->_cachedIndex;

        size_t right = 0;
        if(!linear_scan(t,t0,t1,right,i1)) {

            // binary search
            while (i1 < right) {
                auto mid = (i1 + right) >> 1;

                if (t < pp[mid]) {
                    right = mid;
                } else {
                    i1 = mid + 1;
                }
            }
            //todo:fix it
            if (i1 >= pp.size()) i1 = pp.size() - 1;
            t1 = pp[i1];
            t0 = pp[i1 - 1];

            // check boundary cases, again
            if (t0 < 0) {
                this->_cachedIndex = 0;
                this->copySampleValue_(0);
                return true;
            }

            if (t1 < 0) {
                i1 = pp.size();
                this->_cachedIndex = i1;
                this->copySampleValue_(i1 - 1);
                return true;
            }
        }
        return false;
    } // seek
    //seek();

    bool validate_interval(float &t,float &t0,float &t1,size_t &i1){
        auto& pp = this->parameterPositions;
        //auto i1 = this->_cachedIndex;

        if(!seek(t,t0,t1,i1)) {
            this->_cachedIndex = i1;
            this->intervalChanged_(i1, t0, t1);
            //return false;
        }
        return true;
    }; // validate_interval
    //validate_interval();

};


#endif //THREE_CPP_INTERPOLANT_H
