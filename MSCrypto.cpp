#include "MSCrypto.h"

#ifndef Arduino_h
    using namespace std;
#endif

namespace MSCrypto{
    uint64_t bytes2num(uint8_t byteCount, uint8_t *bytes){
        uint64_t result = 0;
        for(uint8_t i = 0; i < byteCount; i++){
            uint64_t power = 1;
            for(int j = 0; j < i; j++){
                power *= 256;
            }
            result += bytes[byteCount - 1 - i] * power;
        }
        return result;
    };

    void num2bytes(uint64_t num, uint8_t byteCount, uint8_t *bytes){
        for(uint8_t i = 0; i < byteCount; i++){
            uint16_t bitShift = (byteCount - 1 - i) * 8;
            uint8_t byte = num >> bitShift;
            bytes[i] = byte;
            // clear upper bits
            uint64_t clearingBits = 255;
            num &= ~(clearingBits << bitShift);
        }
    };
}