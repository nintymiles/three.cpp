//
// Created by ultraman on 1/9/2024.
//

#ifndef THREE_CPP_KEYFRAME_TRACK_TEMPLATE_H
#define THREE_CPP_KEYFRAME_TRACK_TEMPLATE_H

#include "constants.h"
#include "math_utils.h"
#include "animation_utils.h"

#include <string>
#include <vector>
#include <memory>
#include <functional>

#include "keyframe_track.h"
#include "linear_interpolant.h"

template<typename ValueBufferType>
class InterpolantFactoryObject{
private:
    std::vector<float> times;
    std::vector<ValueBufferType> values;
    size_t sampleSize;
    Interpolate interpolation;
public:
    InterpolantFactoryObject() = default;

    InterpolantFactoryObject(const std::vector<float> &times, std::vector<ValueBufferType> values,size_t valueSize,Interpolate interpolation)
                                                                                            :times(times),values(values),sampleSize(valueSize),interpolation(interpolation){}
    std::shared_ptr<Interpolant> operator()(std::vector<float> buffer) const{
//        switch ( interpolation ) {
//            case Interpolate::InterpolateDiscrete:
//                return std::make_shared<LinearInterpolant>(times,values,getValueSize(),result);
//                break;
//            case Interpolate::InterpolateLinear:
//                factoryMethod = &KeyframeTrackTemplate::InterpolantFactoryMethodLinear;
//                break;
//            case Interpolate::InterpolateSmooth:
//                factoryMethod = &KeyframeTrackTemplate::InterpolantFactoryMethodSmooth;
//                break;
//
//        }

        //todo:fixit return concrete Interpolant?
        return std::make_shared<LinearInterpolant>(times,values,sampleSize,buffer);

    }

    Interpolate getInterpolation(){
        return interpolation;
    }
};

template<typename ValueBufferType>
class KeyframeTrackTemplate:public KeyframeTrack {
public:
    std::string name;
    std::vector<float> times;
    std::vector<ValueBufferType> values;
    using sptr = std::shared_ptr<KeyframeTrackTemplate>;

    InterpolantFactoryObject<ValueBufferType> createInterpolantObj;

    KeyframeTrackTemplate() = default;

    KeyframeTrackTemplate(std::string name,const std::vector<float> &times, std::vector<ValueBufferType> values,Interpolate interpolation):name(name),times(times),values(values){
        //this->setInterpolation(interpolation);
        this->createInterpolantObj =
                InterpolantFactoryObject<ValueBufferType>(times,values,this->getValueSize(),interpolation);
    }
//    using InterpolantFactoryMethodType = std::shared_ptr<Interpolant> (KeyframeTrack::*)(std::vector<float>&);
//    InterpolantFactoryMethodType createInterpolant;

    static sptr create(std::string name,const std::vector<float> &times,const std::vector<float> &values,Interpolate interpolation){
        return std::make_shared<KeyframeTrackTemplate>(name,times,values,interpolation);
    }

    static sptr create(){
        return std::make_shared<KeyframeTrackTemplate>();
    }

//    KeyframeTrack& setInterpolation( Interpolate interpolation ) {
//        InterpolantFactoryMethodType factoryMethod;
//
//        switch ( interpolation ) {
//            case Interpolate::InterpolateDiscrete:
//                factoryMethod = &KeyframeTrackTemplate::InterpolantFactoryMethodDiscrete;
//                break;
//            case Interpolate::InterpolateLinear:
//                factoryMethod = &KeyframeTrackTemplate::InterpolantFactoryMethodLinear;
//                break;
//            case Interpolate::InterpolateSmooth:
//                factoryMethod = &KeyframeTrackTemplate::InterpolantFactoryMethodSmooth;
//                break;
//
//        }
//
//
//
////        if ( factoryMethod == nullptr ) {
////
//////            const message = 'unsupported interpolation for ' +
//////                            this.ValueTypeName + ' keyframe track named ' + this.name;
//////
//////            if ( this.createInterpolant == undefined ) {
//////
//////                // fall back to default, unless the default itself is messed up
//////                if ( interpolation != this.DefaultInterpolation ) {
//////
//////                    this.setInterpolation( this.DefaultInterpolation );
//////
//////                } else {
//////
//////                    throw new Error( message ); // fatal, in this case
//////
//////                }
//////
//////            }
//////
//////            console.warn( 'THREE.KeyframeTrack:', message );
////            return *this;
////
////        }
//
//        createInterpolant = factoryMethod;
//
//        return *this;
//
//    }

    Interpolate getInterpolation() {
//        if ( createInterpolant == &KeyframeTrackTemplate::InterpolantFactoryMethodDiscrete  )
//            return Interpolate::InterpolateDiscrete;
//        else if ( createInterpolant == &KeyframeTrackTemplate::InterpolantFactoryMethodLinear  )
//            return Interpolate::InterpolateLinear;
//        else if ( createInterpolant == &KeyframeTrackTemplate::InterpolantFactoryMethodSmooth )
//            return Interpolate::InterpolateSmooth;
//        else
//            return Interpolate::Unknown;
        return this->createInterpolantObj.getInterpolation();
    }

    int getValueSize(){
        return this->values.size() / this->times.size();
    }

    // move all keyframes either forwards or backwards in time
    KeyframeTrackTemplate& shift( float timeOffset ){

        if ( timeOffset != 0.0 ) {
            auto& times = this->times;

            for ( int i = 0, n = times.size(); i != n; ++i ) {
                times[ i ] += timeOffset;
            }

        }

        return *this;
    }

    // scale all keyframe times by a factor (useful for frame <-> seconds conversions)
    KeyframeTrackTemplate& scale( float timeScale ){

        if ( timeScale != 1.0 ) {
            auto& times = this->times;

            for ( int i = 0, n = times.size(); i != n; ++ i ) {
                times[ i ] *= timeScale;

            }

        }

        return *this;
    }

    // removes keyframes before and after animation without changing any values within the range [startTime, endTime].
    // IMPORTANT: We do not shift around keys to the start of the track time, because for interpolated keys this will change their values
    KeyframeTrackTemplate& trim( float startTime, float endTime ){

        auto& times = this->times;
        auto nKeys = times.size();

        int from = 0,
            to = nKeys - 1;

        while ( from != nKeys && times[ from ] < startTime ) {
            ++ from;
        }

        while ( to != - 1 && times[ to ] > endTime ) {
            -- to;
        }

        ++ to; // inclusive -> exclusive bound

        if ( from != 0 || to != nKeys ) {
            // empty tracks are forbidden, so keep at least one keyframe
            if ( from >= to ) {
                to = math::max( to, 1 );
                from = to - 1;

            }

            auto stride = this->getValueSize();
            this->times = AnimationUtils::arraySlice( times, from, to );
            this->values = AnimationUtils::arraySlice( this->values, from * stride, to * stride );

        }

        return *this;

    }

    // ensure we do not get a GarbageInGarbageOut situation, make sure tracks are at least minimally viable
    bool validate(){
        bool valid = true;

        auto valueSize = this->getValueSize();
        if ( valueSize - math::floor( valueSize ) != 0 ) {
            std::cerr << "THREE.KeyframeTrack: Invalid value size in track." << std::endl;
            valid = false;
        }

        auto& times = this->times;
        auto& values = this->values;
        auto nKeys = times.size();

        if ( nKeys == 0 ) {
            std::cerr << "THREE.KeyframeTrack: Track is empty." << std::endl;
            valid = false;
        }

        float prevTime = 0;

        for ( size_t i = 0; i != nKeys; i ++ ) {
            auto currTime = times[ i ];

            if ( prevTime != 0 && prevTime > currTime ) {
                std::cerr << "THREE.KeyframeTrack: Out of order keys." << std::endl; //, this, i, currTime, prevTime );
                valid = false;
                break;
            }

            prevTime = currTime;
        }

        if ( !values.empty() ) {

            for ( size_t i = 0, n = values.size(); i != n; ++ i ) {
                auto value = values[ i ];

                if ( std::isnan<float>( value ) ) {
                    std::cerr << "THREE.KeyframeTrack: Value is not a valid number." << value << std::endl;//, this, i, value );
                    valid = false;
                    break;

                }

            }

        }

        return valid;

    }

    std::vector<float> getTimes(){
        return times;
    }

    // removes equivalent sequential keys as common in morph target sequences
    // (0,0,0,0,1,1,1,0,0,0,0,0,0,0) --> (0,0,1,1,0,0)
    KeyframeTrackTemplate& optimize(){
        // times or values may be shared with other tracks, so overwriting is unsafe
        auto times = AnimationUtils::arraySlice( this->times );
        auto values = AnimationUtils::arraySlice( this->values );
        auto stride = this->getValueSize();

        auto smoothInterpolation = true; //todo:fixit (this->getInterpolation() == Interpolate::InterpolateSmooth);

        auto lastIndex = times.size() - 1;

        size_t writeIndex = 1;

        for ( size_t i = 1; i < lastIndex; ++ i ) {
            bool keep = false;

            auto time = times[ i ];
            auto timeNext = times[ i + 1 ];

            // remove adjacent keyframes scheduled at the same time
            if ( time != timeNext && ( i != 1 || time != times[ 0 ] ) ) {

                if ( ! smoothInterpolation ) {

                    // remove unnecessary keyframes same as their neighbors
                    auto offset = i * stride,
                            offsetP = offset - stride,
                            offsetN = offset + stride;

                    for ( size_t j = 0; j != stride; ++ j ) {

                        auto value = values[ offset + j ];

                        if ( value != values[ offsetP + j ] ||
                                       value != values[ offsetN + j ] ) {
                            keep = true;
                            break;
                        }

                    }

                } else {
                    keep = true;
                }

            }

            // in-place compaction
            if ( keep ) {

                if ( i != writeIndex ) {
                    times[ writeIndex ] = times[ i ];

                    auto readOffset = i * stride,
                            writeOffset = writeIndex * stride;

                    for ( size_t j = 0; j != stride; ++ j ) {

                        values[ writeOffset + j ] = values[ readOffset + j ];
                    }

                }

                ++ writeIndex;
            }

        }

        // flush last keyframe (compaction looks ahead)
        if ( lastIndex > 0 ) {

            times[ writeIndex ] = times[ lastIndex ];

            for ( size_t readOffset = lastIndex * stride, writeOffset = writeIndex * stride, j = 0; j != stride; ++ j ) {

                values[ writeOffset + j ] = values[ readOffset + j ];
            }

            ++ writeIndex;

        }

        if ( writeIndex != times.size() ) {
            this->times = AnimationUtils::arraySlice( times, 0, writeIndex );
            this->values = AnimationUtils::arraySlice( values, 0, writeIndex * stride );
        } else {
            this->times = times;
            this->values = values;
        }

        return *this;
    }


    std::shared_ptr<Interpolant> InterpolantFactoryMethodDiscrete( std::vector<float>& result ){
        return std::make_shared<LinearInterpolant>(times,values,getValueSize(),result);
    }

    std::shared_ptr<Interpolant> InterpolantFactoryMethodLinear( std::vector<float>& result ){
        return std::make_shared<LinearInterpolant>(times,values,getValueSize(),result);
    }

    std::shared_ptr<Interpolant> InterpolantFactoryMethodSmooth( std::vector<float>& result ){
        return std::make_shared<LinearInterpolant>(times,values,getValueSize(),result);
    }

};

using BooleanKeyframeTrack = KeyframeTrackTemplate<bool>;
using ColorKeyframeTrack = KeyframeTrackTemplate<float>;
using NumberKeyframeTrack = KeyframeTrackTemplate<int>;
using QuaternionKeyframeTrack = KeyframeTrackTemplate<float>;
using VectorKeyframeTrack = KeyframeTrackTemplate<float>;


#endif //THREE_CPP_KEYFRAME_TRACK_TEMPLATE_H
