#ifndef CRYPTO
#define CRYPTO

#include <cstdint>
#include <cmath>

namespace Crypto{
    uint64_t bytes2num(uint8_t byteCount, uint8_t *bytes);
    void num2bytes(uint64_t num, uint8_t byteCount, uint8_t *bytes);
};

#endif