//
// Created by SeanR on 1/9/2024.
//

#ifndef THREE_CPP_ANIMATION_UTILS_H
#define THREE_CPP_ANIMATION_UTILS_H

#include <vector>
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
};

#endif //THREE_CPP_ANIMATION_UTILS_H
