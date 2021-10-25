#include "MSCrypto.h"
#ifndef Arduino_h
    using namespace std;
#endif

namespace MSCrypto{
    uint64_t bytes2num(uint8_t byteCount, uint8_t *bytes){
        if(byteCount == 0){
            return 0;
        }
        uint64_t result = bytes[0];
        for(uint8_t i = 1; i < byteCount; i++){
            result = result << 8;
            result += bytes[i];
        }
        return result;
    };

    void num2bytes(uint64_t num, uint8_t byteCount, uint8_t *bytes){
        if(byteCount == 0){
            return;
        }
        for(uint8_t i = 0; i < byteCount; i++){
            uint16_t bitShift = (byteCount - 1 - i) * 8;
            uint8_t byte = num >> bitShift;
            bytes[i] = byte;
            // clear upper bits
            uint64_t clearingBits = 255;
            num &= ~(clearingBits << bitShift);
        }
    };

    uint64_t bits2num(uint8_t bitLen, uint8_t* bits){
        if(bitLen <= 0){
            return 0;
        }
        uint64_t result = bits[0];
        for(uint8_t i = 1; i < bitLen; i++){
            result = result << 1;
            result += bits[i];
        }
        return result;
    }

    void num2bits(uint64_t num, uint8_t bitLen, uint8_t* bits){
        if(bitLen <= 0){
            return;
        }
        for(uint8_t i = bitLen - 1; i >= 0 && i <= bitLen - 1; i--){
            bits[i] = num & 1;
            num = num >> 1;
        }
    }
}