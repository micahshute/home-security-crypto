#ifndef LEHMER_STRATEGY
#define LEHMER_STRATEGY

#include "Arduino.h"
#ifndef Arduino_h
    #include <cstdint>
#endif

//NOTE: DO not use this with T = uint64_t 

namespace MSCrypto{
    class LehmerStrategy{
        private: 
            uint32_t mult; 
            uint32_t mod;
            uint32_t q;
            uint32_t r;
            uint32_t n_m_1;
            uint32_t seed;

        public: 
            LehmerStrategy();
            LehmerStrategy(uint32_t seed);
            uint32_t calculate();
            void revert(uint32_t state);
            void reset();
    };
};

#endif