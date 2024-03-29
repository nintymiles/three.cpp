//
// Created by SeanR on 2023/6/15.
//

#ifndef THREE_CPP_STRING_UTILS_H
#define THREE_CPP_STRING_UTILS_H

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

namespace string_utils{
    //string split operation
    std::vector<std::string> split(const std::string& str, const std::string& delim);

    inline void toUpperCase(std::string& str){
        std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    }

    inline void toLowerCase(std::string& str){
        std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    }

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

    //std::string join(const std::vector<std::string>& strVec,std::string delim){
    //    auto concatStrObj = std::for_each(strVec.cbegin(),strVec.cend(),(delim!="")?JoinOperation<std::string>(delim):JoinOperation<std::string>());
    //    return concatStrObj.getSum();
    //}

    //std::string join(const std::vector<std::string>& strVec){
    //    return join(strVec,"");
    //}

    inline std::vector<std::string> split(std::string str, char delimiter) {
        std::vector<std::string> internal;
        size_t size = str.find(delimiter);
        internal.reserve(size + 2);
        std::stringstream ss(str);
        std::string temp;
        while (std::getline(ss, temp, delimiter)) {
            if(temp.size()>0)
                internal.push_back(temp);
        }
        return internal;
    }

    // trim from start (in place)
    inline void ltrim(std::string& s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
            return !std::isspace(ch);
        }));
    }

    // trim from end (in place)
    inline void rtrim(std::string& s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    }

    // trim from both ends (in place)
    inline std::string& trim(std::string& s) {
        ltrim(s);
        rtrim(s);

        return s;
    }

    inline std::string replace_all(
            std::string& message,
            const std::string& pattern,
            const std::string& replace) {
        std::string result = message;
        std::string::size_type pos = 0;
        std::string::size_type offset = 0;

        while ((pos = result.find(pattern, offset)) != std::string::npos){

            result.replace(result.begin() + pos, result.begin() + pos + pattern.size(), replace);

            offset = pos + replace.size();

        }
        return result;
    }

    inline float parseFloat(std::string value){
        return atof(value.c_str());
    }
}

#endif //THREE_CPP_STRING_UTILS_H
