#ifndef CRYPTO
#define CRYPTO

#include <cstdint>
#include <cmath>

namespace Crypto{
    uint64_t bytes2num(uint8_t byteCount, uint8_t *bytes);
    void num2bytes(uint64_t num, uint8_t byteCount, uint8_t *bytes);
    template <typename T> 
    T rolloverDifference(T largerNum, T smallerNum);
};

template <typename T>
T rolloverDifference(T largerNum, T smallerNum){
    if(largerNum > smallerNum){
        return largerNum - smallerNum;
    }else{
        int byteCount = sizeof(largerNum);
        T maxSize = (T)(std::pow(2, byteCount) - 1);
        T difference = maxSize - smallerNum + 1 + largerNum;
        return difference;
    }
}

#endif