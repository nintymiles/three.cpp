//
// Created by SeanR on 2024/1/16.
//

#ifndef THREE_CPP_CONTAINER_UTILS_H
#define THREE_CPP_CONTAINER_UTILS_H

#include <vector>

namespace container_utils {

    template<typename T>
    std::vector<T>& fillTripleIntoVector(std::vector<T>& vector,T a,T b,T c){
        vector.push_back(a);
        vector.push_back(b);
        vector.push_back(c);

        return vector;
    }

    template<typename T>
    std::vector<T>& fillSingleIntoVector(std::vector<T>& vector,T a){
        vector.push_back(a);
        return vector;
    }

    template<typename T>
    std::vector<T>& fillDoubleIntoVector(std::vector<T>& vector,T a,T b){
        vector.push_back(a);
        vector.push_back(b);
        return vector;
    }

};


#endif //THREE_CPP_CONTAINER_UTILS_H
