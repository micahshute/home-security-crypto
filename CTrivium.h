#ifndef MS_CRYPTO_CTRIVIUM
#define MS_CRYPTO_CTRIVIUM

#include "Arduino.h"
#ifndef Arduino_h
    #include <cstdint>
#endif
#include "CShiftRegister.h"

namespace MSCrypto{
    class CTrivium{
        public:
            // NOTE: key and iv should both be of length 80 
            // for a bit array but 81 for a char array
            CTrivium();
            CTrivium(uint8_t *key, uint8_t *iv);
            CTrivium(uint8_t *key, uint8_t *iv, bool isCompressed);
            CTrivium(char *key, char *iv);
            struct RegisterState{
                uint8_t register1[12];
                uint8_t register2[11];
                uint8_t register3[14];
            };
            uint8_t getBit();
            uint8_t get();
            uint8_t getByte();
            uint8_t calculate();
            void cacheState();
            RegisterState getState();
            void reset();
            void revert(RegisterState &state);
            bool revert();
        private: 
            CShiftRegister<93, 12> register1;
            CShiftRegister<84, 11> register2;
            CShiftRegister<111, 14> register3;
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
            RegisterState cachedState;
            bool hasCachedState;
            void initialize();

    };
};

#endif