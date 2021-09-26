#include "MSCrypto.h"

namespace MSCrypto{
    uint64_t bytes2num(uint8_t byteCount, uint8_t *bytes){
        uint64_t result = 0;
        for(uint8_t i = 0; i < byteCount; i++){
            result += bytes[byteCount - 1 - i] * (uint64_t)std::pow(256, i);
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