//
// Created by SeanR on 2023/6/15.
//

#ifndef THREE_CPP_STRING_UTILS_H
#define THREE_CPP_STRING_UTILS_H

#include <string>
#include <vector>

namespace string_utils{
    //string split operation
    std::vector<std::string> split(const std::string& str, const std::string& delim);

    template <typename T>
    class JoinOperation{
    public:
        JoinOperation()=default;
        JoinOperation(std::string delim):delim(delim){}
        void operator()(T t){
            num++;
            sum+= t;
            if(delim!="")
                sum+=delim;
        }
        T getSum() const{ return sum; }
        int getSize() const{ return num; }
        double getMean() const{ return static_cast<double>(sum)/static_cast<double>(num); }
    private:
        T sum{};
        int num{0};
        std::string delim;
    };

    std::string join(const std::vector<std::string>& strVec,std::string delim){
        auto concatStrObj = std::for_each(strVec.cbegin(),strVec.cend(),(delim!="")?JoinOperation<std::string>(delim):JoinOperation<std::string>());
        return concatStrObj.getSum();
    }

    std::string join(const std::vector<std::string>& strVec){
        return join(strVec,"");
    }
}

#endif //THREE_CPP_STRING_UTILS_H
