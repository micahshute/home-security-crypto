#ifndef MS_CRYPTO_TRIVIUM
#define MS_CRYPTO_TRIVIUM

#include <cstdint>
#include "ShiftRegister.h"

namespace MSCrypto{
    class Trivium{
        private: 
            ShiftRegister<uint8_t, 93> register1;
            ShiftRegister<uint8_t, 84> register2;
            ShiftRegister<uint8_t, 111> register3;
            uint8_t iv[80];
            uint8_t secretKey[80];
            void setupTaps();
            struct RegisterTaps{
                uint8_t output;
                uint8_t feedback;
                uint8_t leftAnd;
                uint8_t rightAnd;
            };
            RegisterTaps registerTaps1;
            RegisterTaps registerTaps2;
            RegisterTaps registerTaps3;
            void initialize();
        public:
            // NOTE: key and iv should both be of length 80 
            // for a bit array but 81 for a char array
            Trivium(uint8_t *key, uint8_t *iv);
            Trivium(char *key, char *iv);
            struct RegisterState{
                uint8_t register1[93];
                uint8_t register2[84];
                uint8_t register3[111];
            };
            uint8_t getBit();
            uint8_t getByte();
            uint8_t calculate();
            RegisterState getState();
            void reset();
            void revert(RegisterState &state);

    };
};

#endif