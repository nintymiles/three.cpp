//
// Created by SeanR on 1/9/2024.
//

#ifndef THREE_CPP_ANIMATION_UTILS_H
#define THREE_CPP_ANIMATION_UTILS_H

#include <vector>
#include <algorithm>

struct AnimationUtils{
    template<class T>
    static std::vector<T> arraySlice(std::vector<T> arr,size_t from = 0,size_t to = 0){
        if(to == 0)
            to = arr.size();
        size_t newArrSize = to-from > 0? to-from:0;
        std::vector<T> newArr(newArrSize);
        std::copy(arr.begin()+from,arr.begin()+to,newArr.begin());

        return newArr;
    }

   static std::vector<int> getKeyframeOrder(std::vector<int> times){

       const auto n = times.size();
       std::vector<int> result = {};
       for ( auto i = 0; i != n; ++ i ) result.push_back(i);

       std::sort(result.begin(),result.end(),[](int a, int b){
           return a > b;
       });

       return result;
   }

    template<class T>
    static std::vector<T> sortedArray(std::vector<T> values, int stride, std::vector<int> order){
        const auto nValues = values.size();
        std::vector<T> result(nValues);

        for ( auto i = 0, dstOffset = 0; dstOffset != nValues; ++i ) {
            const auto srcOffset = order[ i ] * stride;

            for ( auto j = 0; j != stride; ++j ) {
                result[ dstOffset ++ ] = values[ srcOffset + j ];
            }
        }

        return result;
    }
};

#endif //THREE_CPP_ANIMATION_UTILS_H
