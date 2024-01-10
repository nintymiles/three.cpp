//
// Created by SeanR on 1/10/2024.
//

#ifndef THREE_CPP_PROPERTY_MIXER_H
#define THREE_CPP_PROPERTY_MIXER_H

#include <memory>
#include <string>
#include <vector>

class PropertyBinding;

template<typename T>
class PropertyMixer {
    std::shared_ptr<PropertyBinding> binding;
    std::string typeName;
    size_t valueSize;
    std::vector<T> buffer;
    size_t _origIndex,_addIndex;
    size_t useCount,referenceCount;
    float cumulativeWeight,cumulativeWeightAdditive;

public:
    PropertyMixer(std::shared_ptr<PropertyBinding> binding,size_t valueSize):binding(binding),valueSize(valueSize) {

        // buffer layout: [ incoming | accu0 | accu1 | orig | addAccu | (optional work) ]
        //
        // interpolators can use .buffer as their .result
        // the data then goes to 'incoming'
        //
        // 'accu0' and 'accu1' are used frame-interleaved for
        // the cumulative result and are compared to detect
        // changes
        //
        // 'orig' stores the original state of the property
        //
        // 'add' is used for additive cumulative results
        //
        // 'work' is optional and is only present for quaternion types. It is used
        // to store intermediate quaternion multiplication results

        //        let mixFunction,
//                mixFunctionAdditive,
//                setIdentity;
//
//        switch ( typeName ) {
//
//            case 'quaternion':
//                mixFunction = this._slerp;
//                mixFunctionAdditive = this._slerpAdditive;
//                setIdentity = this._setAdditiveIdentityQuaternion;
//
//                this.buffer = new Float64Array( valueSize * 6 );
//                this._workIndex = 5;
//                break;
//
//            case 'string':
//            case 'bool':
//                mixFunction = this._select;
//
//                // Use the regular mix function and for additive on these types,
//                // additive is not relevant for non-numeric types
//                mixFunctionAdditive = this._select;
//
//                setIdentity = this._setAdditiveIdentityOther;
//
//                this.buffer = new Array( valueSize * 5 );
//                break;
//
//            default:
//                mixFunction = this._lerp;
//                mixFunctionAdditive = this._lerpAdditive;
//                setIdentity = this._setAdditiveIdentityNumeric;
//
//                this.buffer = new Float64Array( valueSize * 5 );
//
//        }
//
        this->buffer = std::vector<T>( valueSize * 5 );

        this->_origIndex = 3;
        this->_addIndex = 4;

        this->cumulativeWeight = 0;
        this->cumulativeWeightAdditive = 0;

        this->useCount = 0;
        this->referenceCount = 0;
    }

    void _mixBufferRegion( std::vector<T> buffer, size_t dstOffset, size_t srcOffset, float t, size_t stride ) {
        auto s = 1 - t;

        for ( size_t i = 0; i != stride; ++ i ) {

            auto j = dstOffset + i;

            buffer[ j ] = buffer[ j ] * s + buffer[ srcOffset + i ] * t;
        }

    }

    void _mixBufferRegionAdditive( std::vector<T> buffer, size_t dstOffset, size_t srcOffset, float t, size_t stride ) {

        for ( size_t i = 0; i != stride; ++ i ) {

            auto j = dstOffset + i;

            buffer[ j ] = buffer[ j ] + buffer[ srcOffset + i ] * t;
        }

    }

    void _setIdentity() {

        auto startIndex = this->_addIndex * this->valueSize;
        auto endIndex = startIndex + this->valueSize;

        for ( size_t i = startIndex; i < endIndex; i ++ ) {
            this->buffer[ i ] = 0;
        }
    }

    // accumulate data in the 'incoming' region into 'accu<i>'
    void accumulate( size_t accuIndex, float weight ) {
        // note: happily accumulating nothing when weight = 0, the caller knows
        // the weight and shouldn't have made the call in the first place

        auto buffer = this->buffer;
        auto stride = this->valueSize,
             offset = accuIndex * stride + stride;

        auto currentWeight = this->cumulativeWeight;

        if ( currentWeight == 0 ) {
            // accuN := incoming * weight
            for ( size_t i = 0; i != stride; ++ i ) {

                buffer[ offset + i ] = buffer[ i ];
            }

            currentWeight = weight;

        } else {
            // accuN := accuN + incoming * weight
            currentWeight += weight;
            auto mix = weight / currentWeight;
            this->_mixBufferRegion( buffer, offset, 0, mix, stride );

        }

        this->cumulativeWeight = currentWeight;

    }

    // accumulate data in the 'incoming' region into 'add'
    void accumulateAdditive( float weight ) {

        auto buffer = this->buffer;
        auto stride = this->valueSize,
             offset = stride * this->_addIndex;

        if ( this->cumulativeWeightAdditive == 0 ) {
            // add = identity
            this->_setIdentity();
        }

        // add := add + incoming * weight
        this->_mixBufferRegionAdditive( buffer, offset, 0, weight, stride );
        this->cumulativeWeightAdditive += weight;

    }

    // apply the state of 'accu<i>' to the binding when accus differ
    void apply( size_t accuIndex ) {
        auto stride = this->valueSize,
             offset = accuIndex * stride + stride;
        auto buffer = this->buffer;

        auto weight = this->cumulativeWeight,
             weightAdditive = this->cumulativeWeightAdditive;

        auto binding = this->binding;

        this->cumulativeWeight = 0;
        this->cumulativeWeightAdditive = 0;

        if ( weight < 1 ) {
            // accuN := accuN + original * ( 1 - cumulativeWeight )
            auto originalValueOffset = stride * this->_origIndex;

            this->_mixBufferRegion(
                    buffer, offset, originalValueOffset, 1 - weight, stride );
        }

        if ( weightAdditive > 0 ) {

            // accuN := accuN + additive accuN
            this->_mixBufferRegionAdditive( buffer, offset, this->_addIndex * stride, 1, stride );
        }

        for ( size_t i = stride, e = stride + stride; i != e; ++ i ) {

            if ( buffer[ i ] != buffer[ i + stride ] ) {
                // value has changed -> update scene graph
                binding.setValue( buffer, offset );
                break;
            }
        }
    }

//    // remember the state of the bound property and copy it to both accus
//    void saveOriginalState() {
//
//        auto binding = this->binding;
//
//        auto buffer = this->buffer;
//        auto stride = this->valueSize,
//             originalValueOffset = stride * this->_origIndex;
//
//        binding->getValue( buffer, originalValueOffset );
//
//        // accu[0..1] := orig -- initially detect changes against the original
//        for ( size_t i = stride, e = originalValueOffset; i != e; ++ i ) {
//            buffer[ i ] = buffer[ originalValueOffset + ( i % stride ) ];
//        }
//
//        // Add to identity for additive
//        this->_setIdentity();
//
//        this->cumulativeWeight = 0;
//        this->cumulativeWeightAdditive = 0;
//
//    }
//
//    // apply the state previously taken via 'saveOriginalState' to the binding
//    void restoreOriginalState() {
//
//        const auto originalValueOffset = this->valueSize * 3;
//        this->binding->setValue( this->buffer, originalValueOffset );
//    }

};


#endif //THREE_CPP_PROPERTY_MIXER_H
