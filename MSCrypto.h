#ifndef MS_CRYPTO
#define MS_CRYPTO

// include "Arduino.h"
#ifndef Arduino_h
    #include <cstdint>
    #include <cmath>
    using namespace std;
#endif

namespace MSCrypto{
    uint64_t bytes2num(uint8_t byteCount, uint8_t *bytes);
    void num2bytes(uint64_t num, uint8_t byteCount, uint8_t *bytes);
    template <typename T> 
    T rolloverDifference(T largerNum, T smallerNum);
};

template <typename T>
T MSCrypto::rolloverDifference(T largerNum, T smallerNum){
    if(largerNum >= smallerNum){
        return largerNum - smallerNum;
    }else{
        int byteCount = sizeof(largerNum);
        T maxSize = (T)(pow(2, 8 * byteCount) - 1);
        T difference = maxSize - smallerNum + 1 + largerNum;
        return difference;
    }
}

#endif