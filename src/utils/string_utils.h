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

    // trim from start (in place)
    static inline void ltrim(std::string& s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
            return !std::isspace(ch);
        }));
    }

    // trim from end (in place)
    static inline void rtrim(std::string& s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    }

    // trim from both ends (in place)
    static inline std::string& trim(std::string& s) {
        ltrim(s);
        rtrim(s);

        return s;
    }
    static inline std::string replace_all(
            std::string& message,
            const std::string& pattern,
            const std::string& replace) {
        std::string result = message;
        std::string::size_type pos = 0;
        std::string::size_type offset = 0;

        while ((pos = result.find(pattern, offset)) != std::string::npos)
        {

            result.replace(result.begin() + pos, result.begin() + pos + pattern.size(), replace);

            offset = pos + replace.size();

        }
        return result;
    }
}

#endif //THREE_CPP_STRING_UTILS_H
